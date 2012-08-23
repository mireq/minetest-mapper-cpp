/*
 * =====================================================================
 *        Version:  1.0
 *        Created:  23.08.2012 12:35:53
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 * =====================================================================
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include "TileGenerator.h"

using namespace std;

TileGenerator::TileGenerator():
	m_bgColor("#ffffff"),
	m_scaleColor("#000000"),
	m_originColor("#ff0000"),
	m_playerColor("#ff0000"),
	m_drawOrigin(false),
	m_drawPlayers(false),
	m_drawScale(false),
	m_drawUnderground(false),
	m_db(0)
{
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
	m_bgColor = bgColor;
}

void TileGenerator::setScaleColor(const std::string &scaleColor)
{
	m_scaleColor = scaleColor;
}

void TileGenerator::setOriginColor(const std::string &originColor)
{
	m_originColor = originColor;
}

void TileGenerator::setPlayerColor(const std::string &playerColor)
{
	m_playerColor = playerColor;
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

void TileGenerator::generate(const std::string &input, const std::string &/*output*/)
{
	openDb(input);
	loadBlocks();
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
		//int cols = sqlite3_column_count(statement);
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);
			if(result == SQLITE_ROW) {
				sqlite3_int64 blocknum = sqlite3_column_int64(statement, 0);
				decodeBlockPos(blocknum);
			}
			else {
				break;
			}
		}
	}
	else {
		throw DbError();
	}
}

inline BlockPos TileGenerator::decodeBlockPos(sqlite3_int64 blockId)
{
	BlockPos pos;
	pos.x = unsignedToSigned(blockId % 4096, 2048);
	blockId = blockId / 4096;
	pos.y = unsignedToSigned(blockId % 4096, 2048);
	blockId = blockId / 4096;
	pos.z = unsignedToSigned(blockId % 4096, 2048);
	return pos;
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

