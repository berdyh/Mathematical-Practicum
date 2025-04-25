#pragma once

#include <string>
#include <vector>

// Constants for printing format
const int kRowsPerPage = 50;
const int kColumnsPerRow = 4;

bool isPrime(int number, const std::vector<int> &primes,
             std::vector<int> &multiples, int current_ord);

std::vector<int> generatePrimeNumbers(int num_primes = 300);

void printPrimeNumbersByLine(const std::vector<int> &primes, int row_offset,
                             int num_primes);

void printPrimeNumbers(std::vector<int> &primes, int num_primes = 300);
