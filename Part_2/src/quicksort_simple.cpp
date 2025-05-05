#include "../include/quicksort_simple.h"

#include <utility>

namespace mapra {

static void QS_Simple(std::vector<unsigned int>& a, std::size_t lo,
                      std::size_t hi) {
  if (lo < hi) {
    // Partition around last element
    unsigned int pivot = a[hi];
    std::size_t i = lo;
    for (std::size_t j = lo; j < hi; ++j) {
      if (a[j] < pivot) {
        std::swap(a[i], a[j]);
        ++i;
      }
    }
    std::swap(a[i], a[hi]);
    if (i > 0) QS_Simple(a, lo, i - 1);
    QS_Simple(a, i + 1, hi);
  }
}

void QuicksortSimple(std::vector<unsigned int>& array) {
  if (!array.empty()) QS_Simple(array, 0, array.size() - 1);
}

}  // namespace mapra
