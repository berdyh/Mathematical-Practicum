#include "../include/selectionsort.h"

#include <string>
#include <utility> // for std::swap
#include <vector>

#include "../include/student.h"

namespace mapra
{

  template <typename T>
  void Selectionsort(std::vector<T> &array)
  {
    const std::size_t n = array.size();
    for (std::size_t i = 0; i + 1 < n; ++i)
    {
      std::size_t min_idx = i;
      for (std::size_t j = i + 1; j < n; ++j)
      {
        if (array[j] < array[min_idx])
        {
          min_idx = j;
        }
      }
      if (min_idx != i)
      {
        std::swap(array[i], array[min_idx]);
      }
    }
  }

  // Explicit template instantiations
  template void Selectionsort(std::vector<double> &);
  template void Selectionsort(std::vector<std::string> &);
  template void Selectionsort(std::vector<Student> &);

} // namespace mapra
