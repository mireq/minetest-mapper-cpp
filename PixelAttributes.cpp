/*
 * =====================================================================
 *        Version:  1.0
 *        Created:  25.08.2012 10:55:27
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 * =====================================================================
 */

#include "PixelAttributes.h"


PixelAttributes::PixelAttributes()
{
	m_blockPixelAttributes.resize(18); // 16px + 2px gradient calculation
}

void PixelAttributes::setWidth(int width)
{
	for (size_t i = 0; i < 18; ++i) {
		m_blockPixelAttributes[i].resize(width + 2); // Width + 1 px gradient calculation on both sides
	}
}

void PixelAttributes::scroll()
{
	m_blockPixelAttributes[17] = m_blockPixelAttributes[1];
}

