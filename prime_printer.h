#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>

void print() {
  int M = 300;
  int RR = 50;
  int CC = 4;
  int ORDMAX = 30;
  int P[M+1];
  int PAGENUMBER;
  int PAGEOFFSET;
  int ROWOFFSET;
  int C;
  int J;
  int K;
  bool JPRIME;
  int ORD;
  int SQUARE;
  int N=0;
  int MULT[ORDMAX+1];

  J=1;
  K=1;
  P[1] = 2;
  ORD = 2;
  SQUARE = 9;

  while (K < M) {
  do {
  J += 2;
  if (J == SQUARE) {
  ORD++;  SQUARE=P[ORD]*P[ORD];   MULT[ORD-1]=J;
  }
  N=2;
  JPRIME=true;
  while (N < ORD && JPRIME) { while (MULT[N] < J)
  MULT[N] += P[N] + P[N]; 
  if (MULT[N] == J) 
  {JPRIME=false;} N++;}
  } while (!JPRIME);
  K++;
  P[K]=J;
  }
  PAGENUMBER = 1;
  PAGEOFFSET = 1;
  while (PAGEOFFSET <= M) {
    std::cout << "The First ";
    std::cout << M ;
    std::cout << " Prime Numbers --- Page ";
    std::cout << PAGENUMBER;
    std::cout << std::endl;
    for (ROWOFFSET=PAGEOFFSET; ROWOFFSET <= PAGEOFFSET+RR-1; ROWOFFSET++) {
    for (C=0; C <= CC-1; C++)
        if (ROWOFFSET+C*RR <= M)
          std::cout << std::setw(10) << P[ROWOFFSET+C*RR];
    std::cout << std::endl;
    }
    std::cout << "\f" << std::endl;
    PAGENUMBER++;
    PAGEOFFSET += RR*CC;
  }
}
