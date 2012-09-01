/*
 * =====================================================================
 *        Version:  1.0
 *        Created:  25.08.2012 10:55:27
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 * =====================================================================
 */

#include "PixelAttributes.h"
#include <algorithm>

using namespace std;

PixelAttributes::PixelAttributes()
{
	m_blockPixelAttributes.resize(17); // 16px + 1px gradient calculation
}

void PixelAttributes::setWidth(int width)
{
	for (size_t i = 0; i < 17; ++i) {
		m_blockPixelAttributes[i].resize(width + 2); // Width + 1 px gradient calculation on both sides
	}
}

void PixelAttributes::scroll()
{
	m_blockPixelAttributes[0] = m_blockPixelAttributes[16];
	for (size_t i = 1; i < 17; ++i) {
		fill(m_blockPixelAttributes[i].begin(), m_blockPixelAttributes[i].end(), PixelAttribute());
	}
}

