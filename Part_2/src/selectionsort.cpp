#include "../include/selectionsort.h"

#include <utility> // for std::swap

namespace mapra
{

  void Selectionsort(std::vector<unsigned int> &array)
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

} // namespace mapra
