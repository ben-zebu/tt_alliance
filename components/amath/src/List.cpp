#include <algorithm>

#include "List.h"

std::vector<double> amath::merge_and_sort_unique(const std::vector<double>& list1, const std::vector<double>& list2) {
    // Merge the two lists
    std::vector<double> merged_list = list1;
    merged_list.insert(merged_list.end(), list2.begin(), list2.end());

    // Sort the merged list
    std::sort(merged_list.begin(), merged_list.end());

    // Remove duplicates
    auto last = std::unique(merged_list.begin(), merged_list.end());
    merged_list.erase(last, merged_list.end());

    return merged_list;
}