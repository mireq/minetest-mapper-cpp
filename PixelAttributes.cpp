/*
 * =====================================================================
 *        Version:  1.0
 *        Created:  25.08.2012 10:55:27
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 * =====================================================================
 */

#include <cstdlib>
#include <cstring>
#include "PixelAttributes.h"

using namespace std;

PixelAttributes::PixelAttributes():
	m_width(0)
{
	for (size_t i = 0; i < BlockCount; ++i) {
		m_pixelAttributes[i] = 0;
	}
}

PixelAttributes::~PixelAttributes()
{
	freeAttributes();
}

void PixelAttributes::setWidth(int width)
{
	freeAttributes();
	m_width = width;
	for (size_t i = 0; i < BlockCount; ++i) {
		m_pixelAttributes[i] = new PixelAttribute[m_width + 1];
	}
}

void PixelAttributes::scroll()
{
	memcpy(m_pixelAttributes[FirstLine], m_pixelAttributes[LastLine], (m_width + 1) * sizeof(PixelAttribute));
	for (size_t i = 1; i < BlockCount - 1; ++i) {
		memcpy(m_pixelAttributes[i], m_pixelAttributes[EmptyLine], (m_width + 1) * sizeof(PixelAttribute));
	}
}

void PixelAttributes::freeAttributes()
{
	for (size_t i = 0; i < BlockCount; ++i) {
		if (m_pixelAttributes[i] != 0) {
			delete[] m_pixelAttributes[i];
			m_pixelAttributes[i] = 0;
		}
	}
}

