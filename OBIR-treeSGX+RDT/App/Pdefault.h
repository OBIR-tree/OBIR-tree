#ifndef PDEFAULT_H
#define PDEFAULT_H

#include <map>
#include <vector>
#include <string>
#include <random>


constexpr auto MAX_SIZE = 4;
constexpr auto MIN_SIZE = 2;
constexpr auto inf = 2e+9;

const int eviction_max = 1;
const int B = 4096;
const int maxsize = 1000;
const int dic_size = 1000;
const int search_K = 1;
const double alpha = 0.5;
const int Dimonsion = 2;
extern int search_num;
extern std::mt19937 gen;
extern std::vector<std::string> dic_str;
extern std::map<int, std::string> dic_map;

#endif // !PDEFAULT_H

