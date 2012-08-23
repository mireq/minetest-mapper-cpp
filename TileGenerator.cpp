/*
 * =====================================================================
 *        Version:  1.0
 *        Created:  23.08.2012 12:35:53
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 * =====================================================================
 */

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "TileGenerator.h"

using namespace std;

TileGenerator::TileGenerator():
	m_drawOrigin(false),
	m_drawPlayers(false),
	m_drawScale(false),
	m_drawUnderground(false),
	m_db(0),
	m_image(0),
	m_xMin(0),
	m_xMax(0),
	m_zMin(0),
	m_zMax(0)
{
	m_bgColor = {255, 255, 255};
	m_scaleColor = {0, 0, 0};
	m_originColor = {255, 0, 0};
	m_playerColor = {255, 0, 0};
}

TileGenerator::~TileGenerator()
{
	if (m_db != 0) {
		sqlite3_close(m_db);
		m_db = 0;
	}
}

void TileGenerator::setBgColor(const std::string &bgColor)
{
	m_bgColor = parseColor(bgColor);
}

void TileGenerator::setScaleColor(const std::string &scaleColor)
{
	m_scaleColor = parseColor(scaleColor);
}

void TileGenerator::setOriginColor(const std::string &originColor)
{
	m_originColor = parseColor(originColor);
}

void TileGenerator::setPlayerColor(const std::string &playerColor)
{
	m_playerColor = parseColor(playerColor);
}

Color TileGenerator::parseColor(const std::string &color)
{
	Color parsed;
	if (color.length() != 7) {
		throw ColorError();
	}
	if (color[0] != '#') {
		throw ColorError();
	}
	long col = strtol(color.c_str() + 1, NULL, 16);
	parsed.b = col % 256;
	col = col / 256;
	parsed.g = col % 256;
	col = col / 256;
	parsed.r = col % 256;
	return parsed;
}

void TileGenerator::setDrawOrigin(bool drawOrigin)
{
	m_drawOrigin = drawOrigin;
}

void TileGenerator::setDrawPlayers(bool drawPlayers)
{
	m_drawPlayers = drawPlayers;
}

void TileGenerator::setDrawScale(bool drawScale)
{
	m_drawScale = drawScale;
}

void TileGenerator::setDrawUnderground(bool drawUnderground)
{
	m_drawUnderground = drawUnderground;
}

void TileGenerator::parseColorsFile(const std::string &fileName)
{
	ifstream in;
	in.open(fileName.c_str(), ifstream::in);
	if (!in.is_open()) {
		std::cerr << "File colors.txt does not exist" << std::endl;
		exit(-2);
	}

	while (in.good()) {
		string name;
		Color color;
		in >> name;
		if (name[0] == '#') {
			in.ignore(65536, '\n');
			in >> name;
		}
		while (name == "\n" && in.good()) {
			in >> name;
		}
		int r, g, b;
		in >> r;
		in >> g;
		in >> b;
		if (in.good()) {
			m_colors[name] = color;
			color.r = r;
			color.g = g;
			color.b = b;
		}
	}
}

void TileGenerator::generate(const std::string &input, const std::string &output)
{
	openDb(input);
	loadBlocks();
	createImage();
	renderMap();
	writeImage(output);
}

void TileGenerator::openDb(const std::string &input)
{
	string db_name = input + "map.sqlite";
	if (sqlite3_open(db_name.c_str(), &m_db) != SQLITE_OK) {
		throw DbError();
	}
}

void TileGenerator::loadBlocks()
{
	sqlite3_stmt *statement;
	string sql = "SELECT pos FROM blocks";
	if (sqlite3_prepare_v2(m_db, sql.c_str(), sql.length(), &statement, 0) == SQLITE_OK) {
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);
			if(result == SQLITE_ROW) {
				sqlite3_int64 blocknum = sqlite3_column_int64(statement, 0);
				BlockPos pos = decodeBlockPos(blocknum);
				if (pos.x > SectorXMax || pos.x < SectorXMin || pos.z > SectorZMax || pos.z < SectorZMin) {
					continue;
				}
				if (pos.x < m_xMin) {
					m_xMin = pos.x;
				}
				if (pos.x > m_xMax) {
					m_xMax = pos.x;
				}
				if (pos.z < m_zMin) {
					m_zMin = pos.z;
				}
				if (pos.z > m_zMax) {
					m_zMax = pos.z;
				}
				m_positions.push_back(std::pair<int, int>(pos.x, pos.z));
			}
			else {
				break;
			}
		}
		m_positions.sort();
		m_positions.unique();
	}
	else {
		throw DbError();
	}
}

inline BlockPos TileGenerator::decodeBlockPos(sqlite3_int64 blockId)
{
	BlockPos pos;
	pos.x = unsignedToSigned(blockId % 4096, 2048);
	blockId = (blockId - pos.x) / 4096;
	pos.y = unsignedToSigned(blockId % 4096, 2048);
	blockId = (blockId - pos.y) / 4096;
	pos.z = unsignedToSigned(blockId % 4096, 2048);
	return pos;
}

inline sqlite3_int64 TileGenerator::encodeBlockPos(int x, int y, int z)
{
	return sqlite3_int64(z) * 16777216l + sqlite3_int64(y) * 4096l + sqlite3_int64(x);
}

inline int TileGenerator::unsignedToSigned(long i, long max_positive)
{
	if (i < max_positive) {
		return i;
	}
	else {
		return i - 2l * max_positive;
	}
}

void TileGenerator::createImage()
{
	m_imgWidth = (m_xMax - m_xMin) * 16;
	m_imgHeight = (m_zMax - m_zMin) * 16;
	m_image = gdImageCreate(m_imgWidth, m_imgHeight);
	// Background
	gdImageColorAllocate(m_image, m_bgColor.r, m_bgColor.g, m_bgColor.b);
}

void TileGenerator::renderMap()
{
	sqlite3_stmt *statement;
	string sql = "SELECT pos FROM blocks WHERE pos >= ? AND pos <= ? AND (pos - ?) % 4096 = 0";
	if (sqlite3_prepare_v2(m_db, sql.c_str(), sql.length(), &statement, 0) != SQLITE_OK) {
		throw DbError();
	}

	for (auto position = m_positions.begin(); position != m_positions.end(); ++position) {
		int xPos = position->first;
		int zPos = position->second;

		sqlite3_int64 psMin = encodeBlockPos(xPos, -2048, zPos);
		sqlite3_int64 psMax = encodeBlockPos(xPos, 2047, zPos);
		sqlite3_bind_int64(statement, 1, psMin);
		sqlite3_bind_int64(statement, 2, psMax);
		sqlite3_bind_int64(statement, 3, psMin);
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);
			if(result == SQLITE_ROW) {
			}
			else {
				break;
			}
		}
	}
}

void TileGenerator::writeImage(const std::string &output)
{
	FILE *out;
	out = fopen(output.c_str(), "w");
	gdImagePng(m_image, out);
	fclose(out);
	gdImageDestroy(m_image);
}

