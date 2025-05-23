#include "prime_printer.h"

#include <iomanip>
#include <iostream>
#include <string>

bool isPrime(int number, const std::vector<int> &primes,
             std::vector<int> &multiples, int current_ord) {
  if (number == 2) return true;
  if (number % 2 == 0) return false;

  int mult_index = 2;
  bool is_prime = true;

  while (mult_index < current_ord && is_prime) {
    while (multiples[mult_index] < number)
      multiples[mult_index] += primes[mult_index] + primes[mult_index];

    if (multiples[mult_index] == number) {
      is_prime = false;
    }
    mult_index++;
  }

  return is_prime;
}

std::vector<int> generatePrimeNumbers(int num_primes) {
  const int kMaxOrd = 30;

  std::vector<int> multiples(kMaxOrd + 1);
  std::vector<int> primes(num_primes + 1);

  int current_number = 1;
  int prime_count = 1;
  int current_ord = 2;
  int current_square = 9;

  primes[1] = 2;

  while (prime_count < num_primes) {
    current_number += 2;
    if (current_number == current_square) {
      current_ord++;
      current_square = primes[current_ord] * primes[current_ord];
      multiples[current_ord - 1] = current_number;
    }
    if (isPrime(current_number, primes, multiples, current_ord)) {
      prime_count++;
      primes[prime_count] = current_number;
    }
  }
  return primes;
}

void printPrimeNumbersByLine(const std::vector<int> &primes, int row_offset,
                             int num_primes) {
  for (int column_idx = 0; column_idx < kColumnsPerRow; column_idx++) {
    int index = row_offset + column_idx * kRowsPerPage;
    if (index <= num_primes) {
      std::cout << std::setw(10) << primes[index];
    }
  }
  std::cout << std::endl;
}

void printPrimeNumbers(std::vector<int> &primes, int num_primes) {
  int page_number = 1;
  int page_offset = 1;
  while (page_offset <= num_primes) {
    std::cout << "The First ";
    std::cout << num_primes;
    std::cout << " Prime Numbers --- Page ";
    std::cout << page_number;
    std::cout << std::endl;

    for (int row = 0; row < kRowsPerPage; row++) {
      int row_offset = page_offset + row;
      if (row_offset <= num_primes)  // Only print rows that have primes
      {
        printPrimeNumbersByLine(primes, row_offset, num_primes);
      }
    }

    std::cout << "\f" << std::endl;
    page_number++;
    page_offset += kRowsPerPage * kColumnsPerRow;
  }
}
