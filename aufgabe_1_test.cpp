#include "mapra_test.h"
#include "prime_printer.h"

void TestPrintPrimes()
{
  const int kNumPrimes = 300;

  mapra::MapraTest test("PrimePrintTester");

  std::ofstream output_file("YourPrint.txt");
  std::streambuf *original_cout_buffer = std::cout.rdbuf();
  std::cout.rdbuf(output_file.rdbuf()); // Redirect cout to "YourPrint.txt"
  std::vector<int> primes = generatePrimeNumbers(kNumPrimes);
  printPrimeNumbers(primes, kNumPrimes);
  output_file.close();

  std::ifstream reference_file("PerfectPrint.txt");
  std::ifstream test_file("YourPrint.txt");

  std::string reference_line;
  std::string test_line;
  int line_number = 1;

  // Compare line by line
  while (std::getline(reference_file, reference_line))
  {
    std::stringstream ss;
    ss << "Line " << std::to_string(line_number) << "\t:";
    std::getline(test_file, test_line);
    test.AssertEq(ss.str(), reference_line, test_line);
    line_number++;
  }

  std::getline(test_file, test_line);
  test.Assert("No more lines :", test_file.eof());
  remove("YourPrint.txt");
  std::cout.rdbuf(original_cout_buffer); // Restore cout to console
}

int main()
{
  TestPrintPrimes();
  return 0;
}