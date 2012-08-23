/*
 * =====================================================================
 *        Version:  1.0
 *        Created:  22.08.2012 15:15:54
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 * =====================================================================
 */

#include <cstdlib>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <map>
#include <stdint.h>
#include <string>
#include "TileGenerator.h"

using namespace std;

struct Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

typedef map<string, Color> ColorMap;

ColorMap parse_colors()
{
	ColorMap parsed;

	ifstream in;
	in.open("colors.txt", ifstream::in);
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
			parsed[name] = color;
			color.r = r;
			color.g = g;
			color.b = b;
		}
	}

	return parsed;
}

void usage()
{
	 const char *usage_text = "minetestmapper.py [options]\n\
  -i/--input <world_path>\n\
  -o/--output <output_image.png>\n\
  --bgcolor <color>\n\
  --scalecolor <color>\n\
  --playercolor <color>\n\
  --origincolor <color>\n\
  --drawscale\n\
  --drawplayers\n\
  --draworigin\n\
  --drawunderground\n\
Color format: '#000000'\n";
	std::cout << usage_text;
}

int main(int argc, char *argv[])
{
	static struct option long_options[] =
	{
		{"help", no_argument, 0, 'h'},
		{"input", required_argument, 0, 'i'},
		{"output", required_argument, 0, 'o'},
		{"bgcolor", required_argument, 0, 'b'},
		{"scalecolor", required_argument, 0, 's'},
		{"origincolor", required_argument, 0, 'r'},
		{"playercolor", required_argument, 0, 'p'},
		{"draworigin", no_argument, 0, 'R'},
		{"drawplayers", no_argument, 0, 'P'},
		{"drawscale", no_argument, 0, 'S'},
		{"drawunderground", no_argument, 0, 'U'}
	};

	string input;
	string output;

	TileGenerator generator;
	int option_index = 0;
	int c = 0;
	while (1) {
		c = getopt_long(argc, argv, "hi:o:", long_options, &option_index);
		if (c == -1) {
			if (input.empty() || output.empty()) {
				usage();
				exit(-1);
			}
			break;
		}
		switch (c) {
			case 'h':
				usage();
				exit(0);
				break;
			case 'i':
				input = optarg;
				break;
			case 'o':
				output = optarg;
				break;
			case 'b':
				generator.setBgColor(optarg);
				break;
			case 's':
				generator.setScaleColor(optarg);
				break;
			case 'r':
				generator.setOriginColor(optarg);
				break;
			case 'p':
				generator.setPlayerColor(optarg);
				break;
			case 'R':
				generator.setDrawOrigin(true);
				break;
			case 'P':
				generator.setDrawPlayers(true);
				break;
			case 'S':
				generator.setDrawScale(true);
				break;
			case 'U':
				generator.setDrawScale(true);
				break;
			default:
				abort();
		}
	}

	ColorMap colors = parse_colors();
}
