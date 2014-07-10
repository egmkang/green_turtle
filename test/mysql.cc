#include <mysqlcxx.h>
#include <system.h>
#include <iostream>
#include <stdint.h>

using namespace std;
using namespace green_turtle;

int main()
{
  MySqlConnection conn;
  conn.Connect("192.168.1.250", 3306, "game", "game", "slug");

  int64_t begin_time = System::GetMilliSeconds();

  cout << conn.ExecSql("BEGIN;                                           ") << endl;
  cout << conn.ExecSql("delete testtestslug from test5;                  ") << endl;
  cout << conn.ExecSql("update test1 set money=money+10 where uin=1;     ") << endl;
  cout << conn.ExecSql("update test2 set money=money+400 where uin=2;    ") << endl;
  cout << conn.ExecSql("insert into test4 (uin,money) values (101, 3000);") << endl;
  cout << conn.ExecSql("COMMIT;                                          ") << endl;

  cout << "cost time : " << System::GetMilliSeconds() - begin_time << "ms" << endl;

  begin_time = System::GetMilliSeconds();

  for(int i = 0; i < 300; ++i)
    conn.ExecSql("insert into test5 (money) values (3000);");
  cout << "cost time : " << System::GetMilliSeconds() - begin_time << "ms" << endl;

  return 0;
}
