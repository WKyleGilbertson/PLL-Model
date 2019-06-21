/* Filename: FLLPLL.cpp                                           2019-03-05 */
//
#include <cstdio>
#include <cmath>
#include "NCO.h"

using namespace std;

int main() {
  FILE * outfile = fopen("FLLPLL.dat", "w");
  long int idx=0, n, ref_i, out_i, index = 0;
  float FSample = 38192000, RefFreq = 9548000, OutFreq = 9548235, PDItime = 0.001;
  double dot = 0.0, cross = 0.0, FreqError = 5000.0, NewFreq = 0.0;
  double Ips1, Ips2, Qps1, Qps2;
  double Error = 0.0, LastError = 0.0, Phi = 0.0, LastPhi = 0.0;
  double  Wn = 47.14, Tau1 = 1/(47.14*47.14), Tau2 = 2 * 0.707 / 47.14;
  long int I1, I2, Q1, Q2;
  char SampleData;
  I1 = I2 = Q1 = Q2 = 1;
  NCO out (5, FSample);
  NCO ref (5, FSample);
  NCO a(5, FSample);
  a.SetFrequency(9.5485e6);
  out.SetFrequency(OutFreq);
  ref.SetFrequency(RefFreq);

  printf("Tau1: %5.3g Tau2: %5.3g Wn: %5.3g Ref: %7.0f\n",
          Tau1, Tau2, Wn, RefFreq);

//while (abs(out.Frequency - RefFreq) > 1 && abs(out.Frequency - RefFreq) < 1000) {
 for (idx=0; idx<10; idx++) {
   for (n = 0; n<FSample*PDItime; n++) {
    ref.clk();
    out.clk();
    SampleData = ref.sine(ref.idx) * (1<<5);
    I1 += SampleData * round(out.sine(out.idx) * (1<<5));
    Q1 += SampleData * round(out.cosine(out.idx)*(1<<5));
   }
   Ips1 = I1 / (FSample*PDItime);  // Normalize accumulated integration
   Qps1 = Q1 / (FSample*PDItime);  // Convenient time to convert to double
//   printf("I1%11ld Q1%11ld ", I1, Q1);
   Error = atan(Qps1/Ips1)/(2*M_PI);
//   Error = atan(Q1/I1)/(2*M_PI);
   Phi = LastPhi + Tau2/Tau1 * (Error - LastError) + Error * (PDItime/Tau1);
   LastPhi = Phi;
   LastError = Error;
//   out.SetFrequency(out.Frequency - Phi * 1.0);
   
   for (n=0; n<FSample*PDItime; n++) {
    ref.clk();
    out.clk();
    SampleData = ref.sine(ref.idx) * (1<<5);
    I2 += SampleData * round(out.sine(out.idx) * (1<<5));
    Q2 += SampleData * round(out.cosine(out.idx)*(1<<5));
   }
   Ips2 = I2 / (FSample*PDItime);
   Qps2 = Q2 / (FSample*PDItime);
//   printf("I2%11ld Q2%11ld ", I2, Q2);
   Error = atan(Qps2/Ips2)/(2*M_PI);
//   Error = atan(Q2/I2)/(2*M_PI);
   Phi = LastPhi + Tau2/Tau1 * (Error - LastError) + Error * (PDItime/Tau1);
   LastPhi = Phi;
   LastError = Error;
//   out.SetFrequency(out.Frequency - Phi * 1.0);
   dot   = Ips1 * Ips2 + Qps1 * Qps2;
   cross = Ips1 * Qps2 - Ips2 * Qps1;

   FreqError = atan2(cross, dot) / (2 * M_PI * PDItime);
   NewFreq = out.Frequency + FreqError / 1.0;

   printf("%3d FErr:%9.3lf %c%c: %6.1lf %c:%5.1lf %c: %7.0f %7.0f\n",
          idx+1, FreqError, 235, 159, fabs(RefFreq - out.Frequency), 237, Phi, 
          159, out.Frequency, NewFreq); 
   fprintf(outfile, "%9.3f\n", out.Frequency - RefFreq);
   out.SetFrequency(NewFreq);
   I1=I2=Q1=Q2=1;
  }
  fclose(outfile);
}