#include "Pdefault.h"

int search_num = 0;
std::random_device rd;
std::mt19937 gen(rd());
std::vector<std::string> dic_str(400);
std::map<int, std::string> dic_map;