#include <bits/stdc++.h>
#include "BmpFile.h"
using namespace std;

int main(int argc, char const *argv[])
{
	BmpFile img;
	string filename("scene");
	img.load(filename+".bmp");
	img.printFileInfo(); 
	img.convertToGray();
	img.save(filename+"_gray.bmp");

	img.load(filename+".bmp");
	img.mean_filter(3);
	img.save(filename+"_mean.bmp");

	img.load(filename+".bmp");
	img.laplacian_enhance(3, 0.1);
	img.save(filename+"_laplacian.bmp");

	return 0;
}