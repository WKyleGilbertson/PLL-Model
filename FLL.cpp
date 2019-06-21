/* Filename: FLL.cpp                                          2019-06-21 */

//#ifndef M_PI
//#define M_PI 3.14159265358979323846
//#endif

#include <cstdio>
#include <cmath>
#include "NCO.h"

using namespace std;

int main() {
  long int idx=0, n, ref_i, out_i, index = 0;
  float FSample = 38192000, RefFreq = 9548000, OutFreq = 9548235, PDItime = 0.001;
  double FreqError = 1000.0, NewFreq = 0.0, cross, dot, Ips1, Qps1, Ips2, Qps2;
  long int I1, I2, Q1, Q2;
  char SampleData;
//  long long int cross, dot;
  I1 = I2 = Q1 = Q2 = 1;
  NCO out (5, FSample); // Can use more bits with internal sin/cos map
  NCO ref (3, FSample); // Incoming signal commonly 2 or 3 bit
  NCO a(5, FSample);
  out.SetFrequency(OutFreq);
  ref.SetFrequency(RefFreq);

while (abs(FreqError) > 10) {
//  for (idx=0; idx<25; idx++) {

   for (n = 0; n<FSample*PDItime; n++) {
    ref.clk();
    out.clk();
    SampleData = round(ref.sine(ref.idx) * (1<<3));
    I1 += SampleData * round(out.sine(out.idx)*(1<<5));
    Q1 += SampleData * round(out.cosine(out.idx)*(1<<5));
//    if (n == 0) printf("%ld %ld", I1, Q1);
   }
   Ips1 = I1 / (FSample*PDItime);  // Normalize accumulated integration
   Qps1 = Q1 / (FSample*PDItime);  // Convenient time to convert to double
   
   for (n=0; n<FSample*PDItime; n++) {
    ref.clk();
    out.clk();
    SampleData = round(ref.sine(ref.idx) * (1<<3));
    I1 += SampleData * round(out.sine(out.idx)*(1<<5));
    Q1 += SampleData * round(out.cosine(out.idx)*(1<<5));
   }
   Ips2 = I2 / (FSample*PDItime);
   Qps2 = Q2 / (FSample*PDItime);
   
   dot   = Ips1 * Ips2 + Qps1 * Qps2;
   cross = Ips1 * Qps2 - Ips2 * Qps1;
   FreqError = atan2(cross, dot) / (2 * M_PI * PDItime);
   NewFreq = out.Frequency + FreqError / 1.0;
   printf("%4ld CalcErr:%9.3f ActErr:%9.3f Fnow:%9.0f Fnxt:%7.0f\n",
   idx+1, FreqError, RefFreq - out.Frequency, out.Frequency, NewFreq);
//   printf("%ld %ld %ld %ld %lf %lf\n", I1, Q1, I2, Q2, cross, dot);
   out.SetFrequency(NewFreq);
   I1=I2=Q1=Q2=1;
  }
}