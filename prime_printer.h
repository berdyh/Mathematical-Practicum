#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>

void print()
{
  int max_primes = 300;
  int rows_per_page = 50;
  int columns_per_row = 4;
  int max_ord = 30;
  int primes[max_primes + 1];
  int page_number;
  int page_offset;
  int row_offset;
  int column;
  int current_number;
  int prime_count;
  bool is_prime;
  int current_ord;
  int current_square;
  int mult_index = 0;
  int multiples[max_ord + 1];

  current_number = 1;
  prime_count = 1;
  primes[1] = 2;
  current_ord = 2;
  current_square = 9;

  while (prime_count < max_primes)
  {
    do
    {
      current_number += 2;
      if (current_number == current_square)
      {
        current_ord++;
        current_square = primes[current_ord] * primes[current_ord];
        multiples[current_ord - 1] = current_number;
      }
      mult_index = 2;
      is_prime = true;
      while (mult_index < current_ord && is_prime)
      {
        while (multiples[mult_index] < current_number)
          multiples[mult_index] += primes[mult_index] + primes[mult_index];
        if (multiples[mult_index] == current_number)
        {
          is_prime = false;
        }
        mult_index++;
      }
    } while (!is_prime);
    prime_count++;
    primes[prime_count] = current_number;
  }

  page_number = 1;
  page_offset = 1;
  while (page_offset <= max_primes)
  {
    std::cout << "The First ";
    std::cout << max_primes;
    std::cout << " Prime Numbers --- Page ";
    std::cout << page_number;
    std::cout << std::endl;
    for (row_offset = page_offset; row_offset <= page_offset + rows_per_page - 1; row_offset++)
    {
      for (column = 0; column <= columns_per_row - 1; column++)
        if (row_offset + column * rows_per_page <= max_primes)
          std::cout << std::setw(10) << primes[row_offset + column * rows_per_page];
      std::cout << std::endl;
    }
    std::cout << "\f" << std::endl;
    page_number++;
    page_offset += rows_per_page * columns_per_row;
  }
}