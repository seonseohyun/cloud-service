#include "DBManager.h"
#include <iostream>
using namespace std;

//MYSQL_STMT 스마트 포인터 가드
struct StmtGuard {
    MYSQL_STMT* stmt;

    StmtGuard(MYSQL* conn) {
        stmt = mysql_stmt_init(conn);
    }

    ~StmtGuard() {
        if (stmt) {
            mysql_stmt_free_result(stmt); //결과 메모리 해제
            mysql_stmt_close(stmt);       //핸들 닫기
        }
    }

    MYSQL_STMT* get() { return stmt; }
    operator MYSQL_STMT*() { return stmt; }
};

//생성자
DBManager::DBManager() {
    //1) MYSQL 핸들 (연결을 다루는 구조체) 초기화
    conn = mysql_init(nullptr);
    if (!conn) {
        cerr << "mysql_init failed!" << endl;
        //conn이 nullptr이면 연결 시도 자체가 불가함
    }
    //2) 초기 연결 시도
    connect();
}

//소멸자
DBManager::~DBManager() {
    if (conn) {
        mysql_close(conn); //연결해제
        conn = nullptr; //초기화
    }
}

//DB 연결 함수
bool DBManager::connect() {
    // mysql_ping == 0 연결 정상, 이미 연결이 살아있으면 재연결 안 함
    if (mysql_ping(conn)==0)
        return true;

    //실제 MariaDB 서버에 붙기    
    if (!mysql_real_connect(
        conn, "localhost",
        "seonseo", // 이름
        "1234",    // 비번
        "cloud",   // DB 이름
        3306,nullptr,0)) //포트, 소켓, 옵션여부
        {
        cerr << "DB connection failed: " << mysql_error(conn) << endl;
        return false;
    }
    std::cout << "DB Connected" << endl; 
    return true;
}

//로그인 체크 함수
bool DBManager::checkLogin(const string& id, const string& pw) {
     if (!connect())
        return false;

    // 1) 스테이트먼트 초기화
    StmtGuard stmt(conn);
    if (!stmt.get()){
        cerr << "[DB] stmt init failed: " << mysql_error(conn) << endl;
        return false;
    }

    //2) SQL 준비
    const char* sql =
        "SELECT COUNT(*) FROM users WHERE id=? AND pw=? LIMIT 1;";
        if (mysql_stmt_prepare(stmt, sql, strlen(sql)) != 0) {
            cerr << "[DB] prepare failed: "<< mysql_stmt_error(stmt) << "\n";
        return false;
    }

    // 3) 파라미터 바인딩
    MYSQL_BIND param[2];
    memset(param, 0, sizeof(param));

    unsigned long id_len = id.size();
    unsigned long pw_len = pw.size();

    // Parameter 0 : id
    param[0].buffer_type   = MYSQL_TYPE_STRING;
    param[0].buffer        = (void*)id.c_str();
    param[0].buffer_length = id_len;
    param[0].length        = &id_len;

    // Parameter 1 : pw
    param[1].buffer_type   = MYSQL_TYPE_STRING;
    param[1].buffer        = (void*)pw.c_str();
    param[1].buffer_length = pw_len;
    param[1].length        = &pw_len;

    if (mysql_stmt_bind_param(stmt, param) != 0) {
        std::cerr << "[DB] bind_param 오류: "
                  << mysql_stmt_error(stmt) << "\n";
        return false;
    }

    // 4) 실행
    if (mysql_stmt_execute(stmt) != 0) {
        std::cerr << "[DB] 실행 오류: "
                  << mysql_stmt_error(stmt) << "\n";
        return false;
    }

    // 5) 결과 바인딩
    int count = 0;
    unsigned long count_len = 0;

    MYSQL_BIND result[1];
    memset(result, 0, sizeof(result));

    result[0].buffer_type   = MYSQL_TYPE_LONG;
    result[0].buffer        = &count;
    result[0].buffer_length = sizeof(count);
    result[0].length        = &count_len;

    if (mysql_stmt_bind_result(stmt, result) != 0) {
        std::cerr << "[DB] bind_result 오류: " 
                  << mysql_stmt_error(stmt) << "\n";
        return false;
    }

    if (mysql_stmt_store_result(stmt) != 0) {
        std::cerr << "[DB] store_result 오류: "
                  << mysql_stmt_error(stmt) << "\n";
        return false;
    }

    if (mysql_stmt_fetch(stmt) != 0) {
        std::cerr << "[DB] fetch 오류: "
                  << mysql_stmt_error(stmt) << "\n";
        return false;
    }

    return count > 0;
}  

bool DBManager::createUser(const string& id, const string& pw, const string& name, const string& email) {
    if (!connect())
       return false;

   // 1) 스테이트먼트 초기화
   StmtGuard stmt(conn);
   if (!stmt.get()){
       cerr << "[DB] stmt init failed: " << mysql_error(conn) << endl;
       return false;
   }

   //2) SQL 준비
   const char* sql =
       "INSERT INTO users (id, pw, name, email) VALUES (?, ?, ?, ?);";
       if (mysql_stmt_prepare(stmt, sql, strlen(sql)) != 0) {
           cerr << "[DB] prepare failed: "<< mysql_stmt_error(stmt) << "\n";
       return false;
   }

   // 3) 파라미터 바인딩
   MYSQL_BIND param[4];
   memset(param, 0, sizeof(param));

   unsigned long id_len = id.size();
   unsigned long pw_len = pw.size();
   unsigned long name_len = name.size();
   unsigned long email_len = email.size();

   // Parameter 0 : id
   param[0].buffer_type   = MYSQL_TYPE_STRING;
   param[0].buffer        = (void*)id.c_str();
   param[0].buffer_length = id_len;
   param[0].length        = &id_len;

   // Parameter 1 : pw
   param[1].buffer_type   = MYSQL_TYPE_STRING;
   param[1].buffer        = (void*)pw.c_str();
   param[1].buffer_length = pw_len;
   param[1].length        = &pw_len;

   // Parameter 2 : name
   param[2].buffer_type   = MYSQL_TYPE_STRING;
   param[2].buffer        = (void*)name.c_str();
   param[2].buffer_length = name_len;
   param[2].length        = &name_len;

   // Parameter 3 : email
   param[3].buffer_type   = MYSQL_TYPE_STRING;
   param[3].buffer        = (void*)email.c_str();
   param[3].buffer_length = email_len;
   param[3].length        = &email_len;

    if (mysql_stmt_bind_param(stmt, param) != 0) {
        cerr << "[DB] bind_param 오류: "<< mysql_stmt_error(stmt) << "\n";  
        return false;
    }

    // 4) 실행
    //mysql_stmt_execute는 0이 정상
    if (mysql_stmt_execute(stmt) != 0) {
        unsigned int err = mysql_stmt_errno(stmt);

        // 1062: Duplicate entry - unique 제약조건 위반
        if (err == 1062) {
            cerr << "[DB] 중복된 ID입니다: " << mysql_stmt_error(stmt) << "\n";
            return false;
        }
        else {
            cerr << "[DB] 실행 오류: " << mysql_stmt_error(stmt) << "\n";
            return false;
        }
    }
    return true;
}

