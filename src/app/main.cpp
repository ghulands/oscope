#include <nanogui/screen.h>

#include <oscope/version.h>

#include <stdio.h>

using namespace oscope;

int main(int argc, char *argv[]) {
	Version v = Version::instance();
	fprintf(stdout, "%d.%d", v.major, v.minor);
	return 0;
}
