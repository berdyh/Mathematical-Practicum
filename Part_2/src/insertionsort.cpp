#include "../include/insertionsort.h"

namespace mapra
{

  void Insertionsort(std::vector<unsigned int> &array)
  {
    const std::size_t n = array.size();
    for (std::size_t i = 1; i < n; ++i)
    {
      unsigned int pivotValue = array[i];
      std::size_t j = i;
      // Shift larger elements rightward
      while (j > 0 && array[j - 1] > pivotValue)
      {
        array[j] = array[j - 1];
        --j;
      }
      array[j] = pivotValue;
    }
  }

} // namespace mapra
