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
	m_drawUnderground(false)

{
}

TileGenerator::~TileGenerator()
{
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

void TileGenerator::generate(const std::string &/*input*/, const std::string &/*output*/)
{
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

