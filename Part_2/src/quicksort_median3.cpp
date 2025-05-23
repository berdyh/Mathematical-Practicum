#include "../include/quicksort_median3.h"

#include <utility> // for std::swap

namespace mapra
{

  static unsigned int MedianIndex(const std::vector<unsigned int> &a,
                                  std::size_t i, std::size_t j, std::size_t k)
  {
    // Returns the index of the median of a[i], a[j], a[k].
    if ((a[i] <= a[j] && a[j] <= a[k]) || (a[k] <= a[j] && a[j] <= a[i]))
      return j;
    if ((a[j] <= a[i] && a[i] <= a[k]) || (a[k] <= a[i] && a[i] <= a[j]))
      return i;
    return k;
  }

  static void QS_Median3(std::vector<unsigned int> &a, std::size_t lo,
                         std::size_t hi)
  {
    if (lo < hi)
    {
      std::size_t mid = lo + (hi - lo) / 2;
      std::size_t m = MedianIndex(a, lo, mid, hi);
      std::swap(a[m], a[hi]); // move pivot to end
      unsigned int pivot = a[hi];

      std::size_t i = lo;
      for (std::size_t j = lo; j < hi; ++j)
      {
        if (a[j] < pivot)
        {
          std::swap(a[i], a[j]);
          ++i;
        }
      }
      std::swap(a[i], a[hi]);
      if (i > 0)
        QS_Median3(a, lo, i - 1);
      QS_Median3(a, i + 1, hi);
    }
  }

  void QuicksortMedian3(std::vector<unsigned int> &array)
  {
    if (!array.empty())
      QS_Median3(array, 0, array.size() - 1);
  }

} // namespace mapra
