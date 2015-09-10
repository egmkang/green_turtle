#include <mysqlcxx.h>
#include <system.h>
#include <iostream>
#include <stdint.h>

using namespace std;
using namespace green_turtle;

int main() {
  MySqlConnection conn;
  conn.Connect("192.168.16.151", 3306, "root", "1q2w3e", "zhanjian");
  conn.SelectCharset("utf8");

  std::string name = "哈哈哈";
  char out[128] = {0};
  {
    int result = conn.EscapeString(out, name.c_str(), name.length());
    cout << result << ":" << out << endl;
  }

  int64_t begin_time = System::GetMilliSeconds();

  std::shared_ptr<ResultSet> result = conn.ExecSelect("select * from role_0");

  while (result->IsValid()) {
    uint32_t uin = result->at(0).to_uint32();
    uint32_t server = result->at(1).to_uint32();
    uint32_t sirdar_id = result->at(2).to_uint32();
    std::string name = result->at(3).to_c_str();

    cout << uin << "," << server << "," << sirdar_id << "," << name << endl;

    result->Next();
  }

  cout << "cost time : " << System::GetMilliSeconds() - begin_time << "ms"
       << endl;

  return 0;
}
