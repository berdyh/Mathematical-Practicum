#include "../include/bubblesort.h"

#include <utility> // for std::swap

namespace mapra
{

  void Bubblesort(std::vector<unsigned int> &array)
  {
    const std::size_t n = array.size(); // size_t is an unsigned integer type

    for (std::size_t end = n; end > 1; --end)
    {
      bool swapped = false;
      for (std::size_t i = 0; i + 1 < end; ++i)
      {
        if (array[i] > array[i + 1])
        {
          std::swap(array[i], array[i + 1]);
          swapped = true;
        }
      }
      if (!swapped) // already sorted
        break;
    }
  }

} // namespace mapra
