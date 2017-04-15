#include <bits/stdc++.h>
#include "BmpFile.h"
using namespace std;

int main(int argc, char const *argv[])
{
	BmpFile img;
	string filename("scene.bmp");
	img.load(filename);
	img.printFileInfo();
	// img.load("scene.bmp");
	// img.printFileInfo();
	return 0;
}