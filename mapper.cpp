/*
 * =====================================================================
 *        Version:  1.0
 *        Created:  22.08.2012 15:15:54
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 * =====================================================================
 */

#include <getopt.h>
#include <cstdlib>
#include <iostream>

using namespace std;

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
		{"draworigin", no_argument, 0, 'w'},
		{"drawplayers", no_argument, 0, 'l'},
		{"drawscale", no_argument, 0, 'c'},
		{"drawunderground", no_argument, 0, 'u'}
	};
	int option_index = 0;
	int c = getopt_long(argc, argv, "hi:o:", long_options, &option_index);
	if (c == -1) {
		usage();
		exit(-1);
	}
	std::cout << c << std::endl;
	while (1) {
		c = getopt_long(argc, argv, "hi:o:", long_options, &option_index);
		if (c == -1) {
			break;
		}
		std::cout << c << std::endl;
	}
}
