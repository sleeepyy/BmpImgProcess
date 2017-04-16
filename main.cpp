#include <bits/stdc++.h>
#include "BmpFile.h"
using namespace std;

int main(int argc, char const *argv[])
{
	BmpFile img;
	string filename("scene");
	img.load(filename+".bmp");
	img.printFileInfo();
	img.Y_histogram_fitting();
	img.save(filename+"_Y_fitting.bmp");
	img.load(filename+".bmp");
	img.translate(100, 100);
	img.save(filename+"_tran1.bmp");
	img.load(filename+".bmp");
	img.translate(-100, -100);
	img.save(filename+"_tran2.bmp");
	return 0;
}