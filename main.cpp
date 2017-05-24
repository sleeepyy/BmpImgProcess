#include <bits/stdc++.h>
#include "BmpFile.h"
using namespace std;

int main(int argc, char const *argv[])
{
	BmpFile img;
	string filename("queban");
	img.load(filename+".bmp");
	img.printFileInfo();
	img.bilateral_filter(12, 25, 21);
	img.save(filename+"_bilateral.bmp");
	return 0;
}