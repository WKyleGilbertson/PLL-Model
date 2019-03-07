/* Filename: FLLPLL.cpp                                           2019-03-05 */
//
#include <cstdio>
#include <cmath>
#include "NCO.h"

using namespace std;

int main() {
  FILE * outfile = fopen("FLLPLL.dat", "w");
  long int idx=0, n, ref_i, out_i, index = 0;
  float FSample = 38192000, RefFreq = 9548000, OutFreq = 9548005, PDItime = 0.001;
  double dot = 0.0, cross = 0.0, FreqError = 5000.0, NewFreq = 0.0, LastFreq = 0.0;
  double Error = 0.0, LastError = 0.0, Phi = 0.0, LastPhi = 0.0;
  double  Wn = 47.14, Tau1 = 1/(47.14*47.14), Tau2 = 2 * 0.707 / 47.14;
  long int I1, I2, Q1, Q2;
  I1 = I2 = Q1 = Q2 = 1;
  NCO out (5, FSample);
  NCO ref (5, FSample);
  NCO a(5, FSample);
  a.SetFrequency(9.5485e6);
  out.SetFrequency(OutFreq);
  ref.SetFrequency(RefFreq);
NewFreq = OutFreq;
LastFreq = RefFreq;
  printf("Tau1: %5.3g Tau2: %5.3g Wn: %5.3g Ref: %7.0f\n",
          Tau1, Tau2, Wn, RefFreq);

while (abs(out.Frequency - RefFreq) > 1 && abs(out.Frequency - RefFreq) < 1000) {
   for (n = 0; n<38192; n++) {
    ref.clk();
    out.clk();
    Q1 += (long int) (ref.cosine(ref.idx) * out.cosine(out.idx));
    I1 += (long int) (ref.sine(ref.idx)   * out.sine(out.idx));
   }
//   printf("I1%11ld Q1%11ld ", I1, Q1);
   Error = atan(Q1/I1)/(2*M_PI);
   Phi = LastPhi + Tau2/Tau1 * (Error - LastError) + Error * (PDItime/Tau1);
   LastPhi = Phi;
   LastError = Error;
   out.SetFrequency(out.Frequency - Phi * 1.0);
   for (n=0; n<38192; n++) {
    ref.clk();
    out.clk();
    Q2 += (long int) (ref.cosine(ref.idx) * out.cosine(out.idx));
    I2 += (long int) (ref.sine(ref.idx)   * out.sine(out.idx));
   }
//   printf("I2%11ld Q2%11ld ", I2, Q2);
   Error = atan(Q2/I2)/(2*M_PI);
   Phi = LastPhi + Tau2/Tau1 * (Error - LastError) + Error * (PDItime/Tau1);
   LastPhi = Phi;
   LastError = Error;
   out.SetFrequency(out.Frequency - Phi * 1.0);

   dot   = I1 * I2 + Q1 * Q2;
   cross = I1 * Q2 - I2 * Q1;
   FreqError = atan2(cross, dot) / (2 * M_PI * PDItime);
//   out.SetFrequency(OutFreq + FreqError/8);
//   printf("%3ld %9.3f %9.0f\n", idx+1, FreqError, NewFreq);
   printf("%3d FErr:%9.3lf %c%c: %6.1lf %c:%5.1lf %c: %7.0f %7.0f\n",
          idx+1, FreqError, 235, 159, fabs(RefFreq - out.Frequency), 237, Phi, 
          159, out.Frequency, NewFreq); 
   fprintf(outfile, "%9.3f\n", out.Frequency - RefFreq);
   LastFreq = NewFreq;
   NewFreq += FreqError * 1.0;
//   out.SetFrequency(NewFreq);
   I1=I2=Q1=Q2=1;
  }
  fclose(outfile);
}