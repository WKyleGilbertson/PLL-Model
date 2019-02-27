#include <cstdio>
#include <climits>
#include <cmath>
#include "NCO.h"

using namespace std;

void calculate_loop_coefficients(float tau[2], float LBW, float zeta, float k){
  float Wn;
  Wn = (LBW * 8 * zeta) / (4 * zeta * zeta + 1);
  tau[0] = k / (Wn * Wn);
  tau[1] = 2.0 * zeta / Wn;
};

int main() {
  unsigned idx, tbl1, tbl2, idx1;
  float PDIcarr = 0.001, tau[2];
  float RefFreq = 9548000, OutFreq = 9548500, SampleFreq = 38192000, cross, dot;
  float carrError = 0.0, oldcarrError = 0.0, NewFreq = 0.0, FreqError = 0.0;
  long int carrNCO = 0, oldcarrNCO = 0, NewPhase = 0;
  long int I = 0, Q = 0, I1 = 0, Q1 = 0;
  NCO  Out(5, SampleFreq);
  NCO  Ref(5, SampleFreq);
  Out.frequency(OutFreq);
  Ref.frequency(RefFreq);
  calculate_loop_coefficients(tau, 25, 0.7, 0.25);// LBW = 25, zeta = .7, k = 0.25
  printf("tau1: %5.3e tau2: %5.3e\n", tau[0], tau[1]);
  NewFreq = OutFreq;
//  NewPhase = Ref.m_dphase;

  for (idx = 0; idx < 30; idx++) {
   for (idx1=0; idx1<38192; idx1++) { // Need to get 1 ms worth of measurements
   tbl1 = Out.clk();
   tbl2 = Ref.clk();
   Q += 100* Ref.cosine(tbl2) * 100* Out.cosine(tbl1);
   I += 100* Ref.sine(tbl2) * 100* Out.sine(tbl1);
   }  // End of 1 ms integration
  cross = I1 * Q - I * Q1;
  dot   = I1 * I + Q1 * Q;
  FreqError = atan2(cross, dot) / (360 * 0.001);
  carrError = (float) atan2(Q,I)/(2*M_PI); // Discriminator [Phase Detector]
//  carrError = (float) atan(Q/I)/(2*M_PI); // Discriminator [Phase Detector]
//carrError = FreqError;
  carrNCO = oldcarrNCO;                     // Loop Filter
  carrNCO += (tau[1]/tau[0]) * (carrError - oldcarrError);
  carrNCO += carrError * (PDIcarr/tau[0]);
  oldcarrNCO = carrNCO;
  oldcarrError = carrError;
//  NewPhase += carrNCO;
//  Out.m_dphase = NewPhase;
  NewFreq += carrNCO;
  Out.frequency(NewFreq);
   printf("%3d Q:%12ld I:%12ld CErr:%6.3f carrNCO:%5ld dF: %5.0f F: %7.0f %10ld\n",
          idx+1, Q, I, FreqError, carrNCO, fabs(RefFreq - NewFreq),
          Out.m_dphase *Out.SAMPLE_RATE/Out.ONE_ROTATION,
          (int) Out.m_dphase - Ref.m_dphase);
  I1 = I;
  Q1 = Q;
  I = 0;
  Q = 0;
  }
 return 0;
}
