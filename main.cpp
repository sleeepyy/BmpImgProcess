#include <bits/stdc++.h>
#include "BmpFile.h"
using namespace std;

int main(int argc, char const *argv[])
{
	BmpFile img;

	img.load("scene.bmp");
	img.printFileInfo();
	img.log_enhance();
	img.save("scene_enhance.bmp");
	img.RGB_histogram_fitting();
	img.save("scene_RGB_fitting.bmp");
	
	return 0;
}