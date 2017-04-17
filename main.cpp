#include <bits/stdc++.h>
#include "BmpFile.h"
using namespace std;

int main(int argc, char const *argv[])
{
	BmpFile img;
	string filename("scene");
	img.load(filename+".bmp");
	img.printFileInfo(); 
	
	img.load(filename+".bmp");
	img.translate(100, 100);
	img.save(filename+"_tran1.bmp");

	img.load(filename+".bmp");
	img.translate(-100, -100);
	img.save(filename+"_tran2.bmp");

	img.load(filename+".bmp");
	img.x_mirror();
	img.save(filename+"_x_mirror.bmp");

	img.load(filename+".bmp");
	img.y_mirror();
	img.save(filename+"_y_mirror.bmp");

	img.load(filename+".bmp");
	img.shear(0.5, 'x');
	img.save(filename+"_x_shear.bmp");

	img.load(filename+".bmp");
	img.shear(1, 'y');
	img.save(filename+"_y_shear.bmp");

	img.load(filename+".bmp");
	img.scale(2, 2);
	img.save(filename+"_scale.bmp");

	img.load(filename+".bmp");
	const double PI = 3.1415926;
	img.rotate(PI/3);
	img.save(filename+"_rotate.bmp");

	return 0;
}