#pragma once

#include <vector>

namespace amath {

    /// @brief Merge and sort two lists of double values and remove duplicates
    /// @param list1 First list of double values
    /// @param list2 Second list of double values
    /// @return Merged and sorted list of unique double values
    std::vector<double> merge_and_sort_unique(const std::vector<double>& list1, const std::vector<double>& list2);

}