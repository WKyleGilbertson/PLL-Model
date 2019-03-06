// Filename: NCO.h                                                    2019-02-08
//
#pragma once
#include <climits>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class NCO {
public:
  const float ONE_ROTATION = (float) 2.0 * (1u << 31);
  float SAMPLE_RATE, Frequency;
  unsigned long m_lglen, m_len, m_mask, m_phase, m_dphase, k, idx;
  char * m_sintable, * m_costable, m_sample_clk;
  NCO(const int lgtblsize, const float m_sample_clk);
  ~NCO(void);
  void SetFrequency(float f);
  unsigned clk(void);
  char sine(int idx);
  char cosine(int idx);
};
