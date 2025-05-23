#ifndef MAPRA_IO_H_
#define MAPRA_IO_H_

#include <fstream>
#include <iostream>
#include <vector>

namespace mapra
{

  // Reads all T’s from ifs into the back of array via operator>>
  template <typename T>
  void Read(std::ifstream &ifs, std::vector<T> &array)
  {
    T tmp;
    while (ifs >> tmp)
    {
      array.push_back(tmp);
    }
  }

  // Prints each element of array via operator<<, one per line
  template <typename T>
  void Print(std::ostream &os, const std::vector<T> &array)
  {
    for (const auto &x : array)
    {
      os << x << "\n";
    }
  }

} // namespace mapra

#endif // MAPRA_IO_H_
