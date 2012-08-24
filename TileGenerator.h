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
#include <list>
#include <map>
#include <sqlite3.h>
#include <stdint.h>
#include <string>

struct Color {
	Color() {};
	Color(uint8_t r, uint8_t g, uint8_t b): r(r), g(g), b(b) {};
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

class ColorError {
};

class DecompressError {
};

class TileGenerator
{
private:
	typedef std::map<std::string, Color> ColorMap;
	typedef std::pair<BlockPos, std::string> Block;
	typedef std::list<Block> BlockList;

public:
	TileGenerator();
	~TileGenerator();
	void setBgColor(const std::string &bgColor);
	void setScaleColor(const std::string &scaleColor);
	void setOriginColor(const std::string &originColor);
	void setPlayerColor(const std::string &playerColor); Color parseColor(const std::string &color);
	void setDrawOrigin(bool drawOrigin);
	void setDrawPlayers(bool drawPlayers);
	void setDrawScale(bool drawScale);
	void setDrawUnderground(bool drawUnderground);
	void parseColorsFile(const std::string &fileName);
	void generate(const std::string &input, const std::string &output);

private:
	void openDb(const std::string &input);
	void loadBlocks();
	BlockPos decodeBlockPos(sqlite3_int64 blockId) const;
	sqlite3_int64 encodeBlockPos(int x, int y, int z) const;
	int unsignedToSigned(long i, long max_positive) const;
	void createImage();
	void renderMap();
	std::list<int> getZValueList() const;
	std::map<int, BlockList> getBlocksOnZ(int zPos, sqlite3_stmt *statement) const;
	void writeImage(const std::string &output);
	void *zlibDecompress(const void *data, std::size_t *processed) const;

private:
	Color m_bgColor;
	Color m_scaleColor;
	Color m_originColor;
	Color m_playerColor;
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
	std::list<std::pair<int, int> > m_positions;
	ColorMap m_colors;

	static const int SectorXMin = -1500/16;
	static const int SectorXMax = 1500/16;
	static const int SectorZMin = -1500/16;
	static const int SectorZMax = 1500/16;
}; /* -----  end of class TileGenerator  ----- */

#endif /* end of include guard: TILEGENERATOR_H_JJNUCARH */

