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
  float FSample = 38192000, RefFreq = 9548000, OutFreq = 9548405, PDItime = 0.001;
  double FreqError = 0.0, NewFreq = 0.0, cross, dot;
  long int I1, I2, Q1, Q2;
//  long long int cross, dot;
  I1 = I2 = Q1 = Q2 = 1;
  NCO out (5, FSample);
  NCO ref (5, FSample);
  NCO a(5, FSample);
  out.SetFrequency(OutFreq);
  ref.SetFrequency(RefFreq);

//while (abs(LastFreq - NewFreq) > 100) {
  for (idx=0; idx<25; idx++) {

   for (n = 0; n<38192; n++) {
    ref.clk();
    out.clk();
    I1 += (ref.sine(ref.idx) * out.sine(out.idx));
    Q1 += (ref.sine(ref.idx) * out.cosine(out.idx));
//    if (n == 0) printf("%ld %ld", I1, Q1);
   }
   for (n=0; n<38192; n++) {
    ref.clk();
    out.clk();
    I2 += (ref.sine(ref.idx) * out.sine(out.idx));
    Q2 += (ref.sine(ref.idx) * out.cosine(out.idx));
   }
   dot   = (double) (I1 * I2 + Q1 * Q2);
   cross = (double) (I1 * Q2 - I2 * Q1);
   FreqError = atan2(cross, dot) / (2 * M_PI * PDItime);
   NewFreq = out.Frequency + FreqError / 1.0;
   printf("%4ld CalcErr:%9.3f ActErr:%9.3f Fnow:%9.0f Fnxt:%7.0f\n",
   idx+1, FreqError, RefFreq - out.Frequency, out.Frequency, NewFreq);
//   printf("%ld %ld %ld %ld %lf %lf\n", I1, Q1, I2, Q2, cross, dot);
   out.SetFrequency(NewFreq);
   I1=I2=Q1=Q2=1;
  }
}