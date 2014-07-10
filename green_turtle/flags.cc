#include <assert.h>
#include <algorithm>
#include <flags.h>

using namespace green_turtle;

CommandFlags::CommandFlags(int argc, const char *argv[])
{
  assert(argc > 0 && argv);
  for(int i = 0 ; i < argc; ++i)
  {
    this->argv_.push_back(argv[i]);
  }
}

static inline
std::vector<std::string>::const_iterator FindFlag(const std::vector<std::string>& argv, const std::string& flag)
{
  for(auto iter = argv.begin();
      iter != argv.end();
      ++iter)
  {
    const std::string& s = *iter;
    if(s[0] == '-' && s.substr(1, s.size() - 1) == flag)
    {
      return iter;
    }
  }
  return argv.end();
}

bool CommandFlags::CheckFlag(const std::string& flag)
{
  return FindFlag(this->argv_, flag) != this->argv_.end();
}

std::string CommandFlags::GetFlags(const std::string& flag)
{
  auto iter = FindFlag(this->argv_, flag);
  if(iter != this->argv_.end() && iter + 1 != this->argv_.end() && *(iter+1)->begin() != '-')
  {
    return *(iter+1);
  }
  return std::string();
}
