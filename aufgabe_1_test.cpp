#include "mapra_test.h"
#include "prime_printer.h"

void TestPrintPrimes() {
  mapra::MapraTest test("PrimePrintTester");

  std::ofstream out("YourPrint.txt");
  std::streambuf *coutbuf = std::cout.rdbuf();
  std::cout.rdbuf(out.rdbuf()); // cout auf Datei "YourPrint.txt" umleiten
  print();
  out.close();

  std::ifstream goldFile("PerfectPrint.txt");
  std::ifstream leadFile("YourPrint.txt");

  std::string goldLine;
  std::string leadLine;
  int i = 1;
  // Zeile fuer Zeile vergleichen
  while (std::getline(goldFile, goldLine)) {
    std::stringstream ss;
    ss << "Line " << std::to_string(i) << "\t:";
    std::getline(leadFile, leadLine);
    test.AssertEq(ss.str(), goldLine, leadLine);
    i++;
  }
  std::getline(leadFile, leadLine);
  test.Assert("No more lines :", leadFile.eof());
  remove("YourPrint.txt");
  std::cout.rdbuf(coutbuf); // cout wieder auf Bildschirm leiten
}

int main() {
  TestPrintPrimes();
  return 0;
}
