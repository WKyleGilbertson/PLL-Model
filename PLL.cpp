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
  float BasisFreq = 9548000, TargetFreq = 9548500, SampleFreq = 38192000;
  float carrError = 0.0, oldcarrError = 0.0, NewFreq = 0.0;
  unsigned long carrNCO = 0, oldcarrNCO = 0, NewPhase = 0;
  long int I = 0, Q = 0;
  NCO  Target(5, SampleFreq);
  NCO  Start(5, SampleFreq);
  Target.frequency(TargetFreq);
  Start.frequency(BasisFreq);
  calculate_loop_coefficients(tau, 25, 0.7, 0.25);// LBW = 25, zeta = .7, k = 0.25
  printf("tau1: %5.3e tau2: %5.3e\n", tau[0], tau[1]);
  NewFreq = BasisFreq;
//  NewPhase = Start.m_dphase;

  for (idx = 0; idx < 30; idx++) {
   for (idx1=0; idx1<38192; idx1++) { // Need to get 1 ms worth of measurements
   tbl1 = Target.clk();
   tbl2 = Start.clk();
   Q += 10* Start.sine(tbl2) * 10* Target.sine(tbl1);
   I += 10* Start.cosine(tbl2) * 10* Target.cosine(tbl1);
   }  // End of 1 ms integration
//  carrError = (float) atan2(Q , I)/(2*M_PI); // Discriminator [Phase Detector]
  carrError = (float) atan(Q/I)/(2*M_PI); // Discriminator [Phase Detector]
  carrNCO = oldcarrNCO;                     // Loop Filter
  carrNCO += (tau[1]/tau[0]) * (carrError - oldcarrError);
  carrNCO += carrError * (PDIcarr/tau[0]);
  oldcarrNCO = carrNCO;
  oldcarrError = carrError;
//  NewPhase += carrNCO;
//  Target.m_dphase = NewPhase;
  NewFreq += carrNCO;
  Target.frequency(NewFreq);
   printf("%3d Q:%8ld I:%8ld CErr:%6.3f carrNCO:%12lu dF: %5.0f F: %7.0f %10ld\n",
          idx+1, Q, I, carrError, carrNCO, fabs(TargetFreq - NewFreq),
          Target.m_dphase *Target.SAMPLE_RATE/Target.ONE_ROTATION,
          (int) Target.m_dphase - Start.m_dphase);
  I = 0;
  Q = 0;
  }
 return 0;
}
