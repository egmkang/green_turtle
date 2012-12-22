#include <assert.h>
#include <stdio.h>
#include <vector>
#include <random>
#include <unordered_map>
#include <string>
#include <utility>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <system.h>
#include "hash_table.h"

const int DataCount = 40*10000;

std::vector<std::pair<int, std::string>> int_to_str;
std::vector<std::pair<std::string, std::string>> str_to_str;
std::mt19937    mt(time(NULL));
std::uniform_int_distribution<> rand_index(0, DataCount - 1);
using namespace std;

size_t GetMilliSeconds()
{
  green_turtle::System::UpdateTime();
  return green_turtle::System::GetMilliSeconds();
}

namespace green_turtle
{
    template<class _Ty>
    struct equal_to
        : public std::binary_function<_Ty, _Ty, bool>
    {
        bool operator()(const _Ty& _Left, const _Ty& _Right) const
        {
            return (_Left == _Right);
        }
    };
    template<>
    struct equal_to<std::string>
    {
        bool operator()(const std::string& l, const std::string& r) const
        {
            return (l.size() == r.size()) && (l == r);
        }
    };
}

std::string BuildSimpleString(int i)
{
    char str[40] = {0};
    
    static const char* prefix[] = 
    {
        "prefix_0_12445_%d",
        "prefix_1_dsdsda_%d",
        "prefix_2__hsgysknms_%d",
        "prefix_3_hsgyaj_%d",
        "prefix_4_haisioksn_%d"
    };
    static std::uniform_int_distribution<> dis(0,sizeof(prefix)/sizeof(prefix[0]) - 1);
    snprintf(str, sizeof(str), prefix[dis(mt)], i);
    return str;
}


void BuildTestData()
{
    for(int i = 0; i < DataCount; ++i)
    {
        int_to_str.push_back(std::make_pair(mt(), BuildSimpleString(i)));
        str_to_str.push_back(std::make_pair(BuildSimpleString(i),BuildSimpleString(i)));
    }
    std::random_shuffle(int_to_str.begin(), int_to_str.end());
    std::random_shuffle(str_to_str.begin(), str_to_str.end());
}

size_t GetHashMapInt2StrCostTime() 
{
    size_t hash_map_begin_time = GetMilliSeconds();

    green_turtle::hash_map<int, std::string, green_turtle::hash<int>> map_(2<<18);

    for(auto const& pair : int_to_str)
    {
        auto find = map_.find(pair.first);
        if(!find)
        {
            map_.insert(pair.first, pair.second);
        }
    }

    size_t hash_map_end_time = GetMilliSeconds();

    return hash_map_end_time - hash_map_begin_time;
}

size_t GetHashMapStr2StrCostTime() 
{
    size_t hash_map_begin_time = GetMilliSeconds();

    green_turtle::hash_map<std::string, std::string, std::hash<std::string>, green_turtle::equal_to<std::string>> map_(2<<18);

    for(auto const& pair : str_to_str)
    {
        auto find = map_.find(pair.first);
        if(!find)
        {
            map_.insert(pair.first, pair.second);
        }
    }

    size_t hash_map_end_time = GetMilliSeconds();

    return hash_map_end_time - hash_map_begin_time;
}

size_t GetUnorderedMapStr2StrCostTime() 
{
    size_t hash_map_begin_time = GetMilliSeconds();

    unordered_map<std::string, std::string> map_;

    for(auto const& pair : str_to_str)
    {
        auto find = map_.find(pair.first);
        if(find == map_.end())
        {
            map_.insert(std::make_pair(pair.first, pair.second));
        }
    }

    size_t hash_map_end_time = GetMilliSeconds();

    return hash_map_end_time - hash_map_begin_time;
}
size_t GetUnorderedMapInt2StrCostTime() 
{
    size_t hash_map_begin_time = GetMilliSeconds();

    unordered_map<int, std::string> map_int_2_str;

    for(auto const& pair : int_to_str)
    {
        auto find = map_int_2_str.find(pair.first);
        if(find == map_int_2_str.end())
        {
            map_int_2_str.insert(std::make_pair(pair.first, pair.second));
        }
    }

    size_t hash_map_end_time = GetMilliSeconds();

    return hash_map_end_time - hash_map_begin_time;
}


int main()
{
    srand(time(NULL));
    BuildTestData();

    std::cout << "unordered_map<int, std::string> cost " << GetUnorderedMapInt2StrCostTime() << "ms" << std::endl;
    std::cout << "hash_map<int, std::string> cost " << GetHashMapInt2StrCostTime() << "ms" << std::endl;

    std::cout << "unordered_map<std::string, std::string> cost " << GetUnorderedMapStr2StrCostTime() << "ms" << std::endl;
    std::cout << "hash_map<std::string, std::string> cost " << GetHashMapStr2StrCostTime() << "ms" << std::endl;

	return 0;
}

