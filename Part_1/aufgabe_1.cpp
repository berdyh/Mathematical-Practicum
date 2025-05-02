#include "prime_printer.h"

int main() {
  std::vector<int> primes = generatePrimeNumbers();
  printPrimeNumbers(primes);

  return 0;
}
