// Filename: NCO.cpp                                                 2019-02-01
// https://zipcpu.com/dsp/2017/12/09/nco.html

#include <cmath>
#include "NCO.h"

// Adjust the sample rate for your implementation as necessary
// const float ONE_ROTATION = (float) 2.0 * (1u << 31);
//const float ONE_ROTATION = 2.0 * (1u << (sizeof(unsigned) * 8 - 1));

NCO::NCO(const int lgtblsize, const float m_sample_clk) {
	SAMPLE_RATE = m_sample_clk;
    // We'll use a table 2^(lgtblize) in length.  This is non-negotiable, as the
    // rest of this algorithm depends upon this property.
    m_lglen = lgtblsize;
    m_len = (1 << lgtblsize);

    // m_mask is 1 for any bit used in the index, zero otherwise
    m_mask = m_len - 1;
    m_sintable = new float[m_len];
    m_costable = new float[m_len];
    for (k = 0; k < m_len; k++) {
        m_sintable[k] = (float) sin(2.0 * M_PI * k / (double) m_len);
        m_costable[k] = (float) cos(2.0 * M_PI * k / (double) m_len);
    }
    // m_phase is the variable holding our PHI[n] function from above.
    // We'll initialize our initial phase and frequency to zero
    m_phase = 0;
    m_dphase = 0;
}

NCO::~NCO(void) {
    delete[] m_sintable; // On any object deletion, delete the table as well
    delete[] m_costable;
}

void NCO::frequency(float f) {
    // Convert the frequency to a fractional difference in phase
    m_dphase = (unsigned long)(f * ONE_ROTATION / SAMPLE_RATE);
}

unsigned NCO::clk(void) {
    unsigned index;

    // Increment the phase by an amount dictated by our frequency
    // m_phase was our PHI[n] value above
    m_phase += m_dphase; // PHI[n] = PHI[n-1] + (2^32 * f/fs)

    // Grab the top m_lglen bits of this phase word
    index = m_phase >> ((sizeof(unsigned) * 8) - m_lglen);

    // Insist that this index be found within 0... (m_len-1)
    index &= m_mask;

    // Finally return the table lookup value
	return index;
}

float NCO::cosine(int idx) {
	return m_costable[idx];
}

float NCO::sine(int idx) {
	return m_sintable[idx];
}