/* Filename: testNCO.cpp                                          2019-03-05 */
// Verify NCO model works

#include <cstdio>
#include "NCO.h"

using namespace std;

int main() {
  long int idx, index = 0;
  float FSample = 38192000, RefFreq = 9548000, OutFreq = 9548500, PDItime = 0.001;
  NCO out (5, FSample);
  NCO Ref (5, FSample);
  NCO a(5, FSample);
  a.SetFrequency(9.5485e6);
  printf("length: %d mask: %3d dPhase: %3d\n", a.m_len, a.m_mask, a.m_dphase);
  for (idx=0; idx<37; idx++) {
    printf("%10lu %4d %4.1f %4.1f\n",
    a.m_phase, index+1, a.m_sintable[index], a.m_costable[index]);
    index = a.clk();
  }
}