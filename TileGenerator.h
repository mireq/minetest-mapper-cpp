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

#include <gd.h>
#include <map>
#include <sqlite3.h>
#include <stdint.h>
#include <string>

struct Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

struct BlockPos {
	int x;
	int y;
	int z;
};

class DbError {
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
	void parseColorsFile(const std::string &fileName);
	void generate(const std::string &input, const std::string &output);

private:
	void openDb(const std::string &input);
	void loadBlocks();
	BlockPos decodeBlockPos(sqlite3_int64 blockId);
	int unsignedToSigned(long i, long max_positive);
	void createImage();
	void writeImage(const std::string &output);

private:
	std::string m_bgColor;
	std::string m_scaleColor;
	std::string m_originColor;
	std::string m_playerColor;
	bool m_drawOrigin;
	bool m_drawPlayers;
	bool m_drawScale;
	bool m_drawUnderground;

	sqlite3 *m_db;
	gdImagePtr m_image;
	int m_xMin;
	int m_xMax;
	int m_zMin;
	int m_zMax;
	int m_imgWidth;
	int m_imgHeight;
	ColorMap m_colors;

	static const int SectorXMin = -1500/16;
	static const int SectorXMax = 1500/16;
	static const int SectorZMin = -1500/16;
	static const int SectorZMax = 1500/16;
}; /* -----  end of class TileGenerator  ----- */

#endif /* end of include guard: TILEGENERATOR_H_JJNUCARH */

