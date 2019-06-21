#include <cstdio>
#include "NCO.h"

using namespace std;

int main() {
  FILE * OUT = fopen("NCO.bin", "w");
  unsigned idx, tbl;
  char num;
  NCO n(5, 38.192e6);
  //NCO m(5, 38.192e6);
  n.SetFrequency(9.5486e6);
  //m.frequency(9.9486e6);
 
  for (idx = 0; idx < 1 << 4; idx++) {
    //num = round(n() * 18);
	tbl = n.clk();
    printf("%10.6f %10.6f\n", n.sine(tbl), n.cosine(tbl));
  }   
  for (idx = 0; idx<38192; idx++) {
    num = (char) n.sine(n.clk());
    fputc(num, OUT);
   if (idx < 100) {
    if (idx % 25 == 0) printf("\n");
     printf("%2d ", num);
	  }
    }
  printf("\n");
 fclose(OUT);
 return 0;
}
