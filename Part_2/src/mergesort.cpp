#include "../include/mergesort.h"

#include <vector>

namespace mapra
{

  static void Merge(std::vector<unsigned int> &a, std::size_t lo, std::size_t mid,
                    std::size_t hi, std::vector<unsigned int> &tmp)
  {
    std::size_t i = lo, j = mid, k = lo;
    while (i < mid && j < hi)
    {
      tmp[k++] = (a[i] <= a[j] ? a[i++] : a[j++]);
    }
    while (i < mid)
      tmp[k++] = a[i++];
    while (j < hi)
      tmp[k++] = a[j++];
    for (std::size_t x = lo; x < hi; ++x)
    {
      a[x] = tmp[x];
    }
  }

  static void MS(std::vector<unsigned int> &a, std::size_t lo, std::size_t hi,
                 std::vector<unsigned int> &tmp)
  {
    if (hi - lo <= 1)
      return;
    std::size_t mid = lo + (hi - lo) / 2;
    MS(a, lo, mid, tmp);
    MS(a, mid, hi, tmp);
    Merge(a, lo, mid, hi, tmp);
  }

  void Mergesort(std::vector<unsigned int> &array)
  {
    std::vector<unsigned int> tmp(array.size());
    MS(array, 0, array.size(), tmp);
  }

} // namespace mapra
