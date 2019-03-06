/* Filename: testNCO.cpp                                          2019-03-05 */
// Verify NCO model works


#include <cstdio>
#include <cmath>
#include "NCO.h"

using namespace std;

int main() {
  FILE * outfile = fopen("FLLPLL.dat", "w");
  long int idx=0, n, ref_i, out_i, index = 0;
  float FSample = 38192000, RefFreq = 9548000, OutFreq = 9548100, PDItime = 0.001;
  double dot = 0.0, cross = 0.0, FreqError = 5000.0, NewFreq = 0.0, LastFreq = 0.0;
  double Error = 0.0, LastError = 0.0, Phi = 0.0, LastPhi = 0.0;
  double  Wn = 47.14, Tau1 = 1/47.14, Tau2 = 2 * 0.707 / 47.14;
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
/*  printf("length: %d mask: %3d dPhase: %3d\n", a.m_len, a.m_mask, a.m_dphase);
  for (idx=0; idx<37; idx++) {
    printf("%10lu %3d %9.3f %9.3f\n",
    a.m_phase, index+1, a.m_sintable[index], a.m_costable[index]);
    index = a.clk();
  }*/

while (abs(out.Frequency - RefFreq) > 1) {
//  for (idx=0; idx<1000; idx++) {

   for (n = 0; n<38192; n++) {
    ref_i = ref.clk();
    out_i = out.clk();
    Q1 += (long int) (ref.m_costable[ref_i] * out.m_costable[out_i]);
    I1 += (long int) (ref.m_sintable[ref_i] * out.m_costable[out_i]);
   }
   printf("I1%11ld Q1%11ld ", I1, Q1);
//   if (I1 == 0) I1 = 1;
   Error = atan(Q1/I1)/(2*M_PI);
   Phi = LastPhi + Tau2/Tau1 * (Error - LastError) + Error * (PDItime/Tau1);
   LastPhi = Phi;
   LastError = Error;
   out.SetFrequency(out.Frequency - Phi * 1.0);
//    printf("Error: %9.3f Phi: %9.3f\n", Error, Phi);
   for (n=0; n<38192; n++) {
    ref_i = ref.clk();
    out_i = out.clk();
    Q2 += (long int) (ref.m_costable[ref_i] * out.m_costable[out_i]);
    I2 += (long int) (ref.m_sintable[ref_i] * out.m_costable[out_i]);
   }
   printf("I2%11ld Q2%11ld ", I2, Q2);
//   if (I2 == 0) I2 = 1;
   Error = atan(Q2/I2)/(2*M_PI);
   Phi = LastPhi + Tau2/Tau1 * (Error - LastError) + Error * (PDItime/Tau1);
   LastPhi = Phi;
   LastError = Error;
   out.SetFrequency(out.Frequency - Phi * 1.0);
//    printf("Error: %9.3f Phi: %9.3f\n", Error, Phi);
    
   dot   = I1 * I2 + Q1 * Q2;
   cross = I1 * Q2 - I2 * Q1;
   FreqError = atan2(cross, dot) / (2 * M_PI * PDItime);
//   out.SetFrequency(OutFreq + FreqError/8);
//   printf("%3ld %9.3f %9.0f\n", idx+1, FreqError, NewFreq);
   printf("idx%5ld FErr:%9.3f F:%9.0f\n", idx++, out.Frequency - RefFreq, out.Frequency);
   fprintf(outfile, "%9.3f\n", out.Frequency - RefFreq);
   LastFreq = NewFreq;
   NewFreq += FreqError * 1.0;
//   out.SetFrequency(NewFreq);
   I1=I2=Q1=Q2=1;
  }
  fclose(outfile);
}