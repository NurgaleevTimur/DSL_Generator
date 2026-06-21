#ifndef SORT_H
#define SORT_H

#include <vector>
#include <unordered_map>

#include "types.h"

void top_sort ( std::vector<block>, std::vector<int>&, std::unordered_map<int, int>& );
void top_sort_kahn ( std::vector<block>, std::vector<int>&, std::unordered_map<int, int>& );

#endif /* SORT_H */