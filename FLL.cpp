/* Filename: testNCO.cpp                                          2019-03-05 */
// Verify NCO model works
//#ifndef M_PI
//#define M_PI 3.14159265358979323846
//#endif

#include <cstdio>
#include <cmath>
#include "NCO.h"

using namespace std;

int main() {
  long int idx=0, n, ref_i, out_i, index = 0;
  float FSample = 38192000, RefFreq = 9548000, OutFreq = 9548500, PDItime = 0.001;
  double dot = 0.0, cross = 0.0, FreqError = 5000.0, NewFreq = 0.0, LastFreq = 0.0;
  long int I1, I2, Q1, Q2;
  I1 = I2 = Q1 = Q2 = 0;
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
  printf("%9.3f\n", M_PI);
//while (abs(LastFreq - NewFreq) > 100) {
  for (idx=0; idx<50; idx++) {

   for (n = 0; n<38192; n++) {
    ref.clk();
    out.clk();
    Q1 += (long int) (ref.m_costable[ref.idx] * out.m_costable[out.idx]);
    I1 += (long int) (ref.m_sintable[ref.idx] * out.m_costable[out.idx]);
   }
   for (n=0; n<38192; n++) {
    ref.clk();
    out.clk();
    Q2 += (long int) (ref.m_costable[ref.idx] * out.m_costable[out.idx]);
    I2 += (long int) (ref.m_sintable[ref.idx] * out.m_costable[out.idx]);
   }
   dot   = I1 * I2 + Q1 * Q2;
   cross = I1 * Q2 - I2 * Q1;
   FreqError = atan2(cross, dot) / (2 * M_PI * PDItime);

   LastFreq = NewFreq;
   NewFreq += FreqError /8;
   printf("%4ld CalcErr:%9.3f ActErr:%9.3f Fnow:%9.0f Fnxt:%7.0f\n",
   idx+1, FreqError, RefFreq - out.Frequency, out.Frequency, NewFreq);
//   out.SetFrequency(out.Frequency + FreqError * 1.0);
//   out.SetFrequency(NewFreq);
   I1=I2=Q1=Q2=0;
  }
}