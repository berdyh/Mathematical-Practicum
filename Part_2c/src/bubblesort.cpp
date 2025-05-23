#include "../include/bubblesort.h"

#include <string>
#include <utility> // for std::swap
#include <vector>

#include "../include/student.h"

namespace mapra
{

  template <typename T>
  void Bubblesort(std::vector<T> &array)
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

  // Explicit template instantiations
  template void Bubblesort(std::vector<double> &);
  template void Bubblesort(std::vector<std::string> &);
  template void Bubblesort(std::vector<Student> &);

} // namespace mapra
