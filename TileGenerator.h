/*
 * =====================================================================
 *        Version:  1.0
 *        Created:  23.08.2012 12:35:59
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 * =====================================================================
 */

#ifndef TILEGENERATOR_H_JJNUCARH
#define TILEGENERATOR_H_JJNUCARH

#include <stdint.h>
#include <string>
#include <map>

struct Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

class TileGenerator
{
private:
	typedef std::map<std::string, Color> ColorMap;

public:
	TileGenerator();
	~TileGenerator();
	void setBgColor(const std::string &bgColor);
	void setScaleColor(const std::string &scaleColor);
	void setOriginColor(const std::string &originColor);
	void setPlayerColor(const std::string &playerColor);
	void setDrawOrigin(bool drawOrigin);
	void setDrawPlayers(bool drawPlayers);
	void setDrawScale(bool drawScale);
	void setDrawUnderground(bool drawUnderground);
	void generate(const std::string &input, const std::string &output);
	void parseColorsFile(const std::string &fileName);

private:
	std::string m_bgColor;
	std::string m_scaleColor;
	std::string m_originColor;
	std::string m_playerColor;
	bool m_drawOrigin;
	bool m_drawPlayers;
	bool m_drawScale;
	bool m_drawUnderground;
	ColorMap m_colors;
}; /* -----  end of class TileGenerator  ----- */

#endif /* end of include guard: TILEGENERATOR_H_JJNUCARH */

