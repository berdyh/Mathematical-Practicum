#include "../include/mergesort.h"

#include <string>
#include <vector>

#include "../include/student.h"

namespace mapra
{

  template <typename T>
  static void Merge(std::vector<T> &a, std::size_t lo, std::size_t mid,
                    std::size_t hi, std::vector<T> &tmp)
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

  template <typename T>
  static void MS(std::vector<T> &a, std::size_t lo, std::size_t hi,
                 std::vector<T> &tmp)
  {
    if (hi - lo <= 1)
      return;
    std::size_t mid = lo + (hi - lo) / 2;
    MS(a, lo, mid, tmp);
    MS(a, mid, hi, tmp);
    Merge(a, lo, mid, hi, tmp);
  }

  template <typename T>
  void Mergesort(std::vector<T> &array)
  {
    std::vector<T> tmp(array.size());
    MS(array, 0, array.size(), tmp);
  }

  // Explicit template instantiations
  template void Mergesort(std::vector<double> &);
  template void Mergesort(std::vector<std::string> &);
  template void Mergesort(std::vector<Student> &);

} // namespace mapra
