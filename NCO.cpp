// Filename: NCO.cpp                                                 2019-02-01
// https://zipcpu.com/dsp/2017/12/09/nco.html

#include <cmath>
#include "NCO.h"

NCO::NCO(const int lgtblsize, const float m_sample_clk) {
	SAMPLE_RATE = m_sample_clk;
    // We'll use a table 2^(lgtblize) in length.  This is non-negotiable, as the
    // rest of this algorithm depends upon this property.
    m_lglen = lgtblsize;
    m_len = (1 << lgtblsize);

    // m_mask is 1 for any bit used in the index, zero otherwise
    m_mask = m_len - 1;
    /*(m_sintable = new char[m_len];
    m_costable = new char[m_len];
    for (k = 0; k < m_len; k++) {
        m_sintable[k] = (char) (m_len) * sin(2.0 * M_PI * k / (double) m_len);
        m_costable[k] = (char) (m_len) * cos(2.0 * M_PI * k / (double) m_len);
        */
    m_sintable = new float[m_len];
    m_costable = new float[m_len];
    for (k = 0; k < m_len; k++) {
        m_sintable[k] = sin(2.0 * M_PI * k / (double) m_len);
        m_costable[k] = cos(2.0 * M_PI * k / (double) m_len);
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

void NCO::SetFrequency(float f) {
    // Convert the frequency to a fractional difference in phase
//    Frequency = f * 112.5 - pow(2, 32)/4; // NCO bias... Is this needed?
    m_dphase = (unsigned long)(f * ONE_ROTATION / SAMPLE_RATE);
    Frequency = f;
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
    idx = index;
    // Finally return the table lookup value
	return idx;
}

float NCO::cosine(int idx) {
	return m_costable[idx];
}

float NCO::sine(int idx) {
	return m_sintable[idx];
}
