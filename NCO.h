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
  float SAMPLE_RATE;
  unsigned long m_lglen, m_len, m_mask, m_phase, m_dphase, k;
  float * m_sintable, * m_costable, m_sample_clk;
  NCO(const int lgtblsize, const float m_sample_clk);
  ~NCO(void);
  void frequency(float f);
  unsigned clk(void);
  float sine(int idx);
  float cosine(int idx);
};
