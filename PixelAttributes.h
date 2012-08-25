/*
 * =====================================================================
 *        Version:  1.0
 *        Created:  25.08.2012 10:55:29
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 * =====================================================================
 */

#ifndef PIXELATTRIBUTES_H_ADZ35GYF
#define PIXELATTRIBUTES_H_ADZ35GYF

#include <vector>

struct PixelAttribute {
	PixelAttribute(): height(0) {};
	int height;
};

class PixelAttributes
{
public:
	PixelAttributes();
	void setWidth(int width);
	void scroll();
	inline PixelAttribute &attribute(int z, int x) { return m_blockPixelAttributes[z + 1][x + 1]; };

private:
	std::vector <std::vector <PixelAttribute> > m_blockPixelAttributes;
};

#endif /* end of include guard: PIXELATTRIBUTES_H_ADZ35GYF */

