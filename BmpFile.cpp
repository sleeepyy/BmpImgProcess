#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iomanip>
#include "BmpFile.h"
using namespace std;

BmpFile::BmpFile():new_color_data(NULL),color_data(NULL){
	file = NULL;
	bin = NULL;
	new_bin = NULL;
	R=G=B=Y=U=V=NULL;
}
// void BmpFile::file_load(){
// 	infile.seekg(2, ios::beg);
// 	infile.read((char*)&file_size, sizeof(int));
// 	std::cout << file_size <<std::endl;
// 	infile.seekg(10, ios::beg);
// 	infile.read((char*)&raw_color_data_offset, sizeof(int));
//     infile.seekg(18,ios::beg);
//     infile.read((char*)&width,sizeof(int));
//     infile.read((char*)&height,sizeof(int));
// 	row_size = 4 * ceil( 3.0 * width / 4.0 ); // 4 bytes 

//     color_data = new unsigned char[row_size * height * 3];
// 	R = new int[width * height];
// 	G = new int[width * height];
// 	B = new int[width * height];
// 	Y = new int[width * height];
// 	U = new int[width * height];
// 	V = new int[width * height];
//     infile.seekg(raw_color_data_offset, ios::beg);
// 	infile.read((char*)color_data, row_size*height*3);
	
// 	for(int y = 0; y < height; y++) {
// 		for(int x = 0; x < width; x++) {
// 			int position = width * y + x;
// 			R[position] = color_data[(row_size*y + 3*x) + 2];
// 			G[position] = color_data[(row_size*y + 3*x) + 1];
// 			B[position] = color_data[(row_size*y + 3*x) + 0];
// 		}
// 	}

// 	this->RGB2YUV();
// }

void BmpFile::load(string file_path){
	this->~BmpFile();
	ifstream infile(file_path, ios::in|ios::binary);
	if(!infile){
		std::cout << "Open file failed." << std::endl;
		exit(OPEN_FILE_ERROR);
	}
	cout << "start to process img" <<endl;
	infile.seekg(2, ios::beg);
	infile.read((char*)&file_size, sizeof(int));
	std::cout << file_size <<std::endl;
	infile.seekg(10, ios::beg);
	infile.read((char*)&raw_color_data_offset, sizeof(int));
    infile.seekg(18,ios::beg);
    infile.read((char*)&width,sizeof(int));
    infile.read((char*)&height,sizeof(int));
	row_size = 4 * ceil( 3.0 * width / 4.0 ); // 4 bytes 

    color_data = new unsigned char[row_size * height * 3];
	R = new int[width * height];
	G = new int[width * height];
	B = new int[width * height];
	Y = new int[width * height];
	U = new int[width * height];
	V = new int[width * height];
    infile.seekg(raw_color_data_offset, ios::beg);
	infile.read((char*)color_data, row_size*height*3);
	
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int position = width * y + x;
			R[position] = color_data[(row_size*y + 3*x) + 2];
			G[position] = color_data[(row_size*y + 3*x) + 1];
			B[position] = color_data[(row_size*y + 3*x) + 0];
		}
	}

	this->RGB2YUV();
	infile.close();
}

void BmpFile::load(const char* file_path){
	this->~BmpFile();
	ifstream infile(file_path, ios::in|ios::binary);
	if(!infile){
		std::cout << "Open file failed." << std::endl;
		exit(OPEN_FILE_ERROR);
	}
	//this->file_load();
	// char tmp[10];
	// fread(tmp, sizeof(char), 2, file); // 0x0h ~ 0x2h
	// fread(&file_size, sizeof(int), 1, file); // 0x2h ~ 0x6h
	// fread(tmp, sizeof(char), 4, file);  // 0x6h ~ 0xAh
	// fread(&raw_color_data_offset, sizeof(int), 1, file); // 0xAh ~ 0xEh
	// fread(&header_size, sizeof(int), 1, file); // 0xEh ~ 0x12h
	// fread(&width, sizeof(int), 1, file);
	// fread(&height, sizeof(int), 1, file);

	// fseek(file, 0, SEEK_SET);
	// file_header = new unsigned char[raw_color_data_offset];
	// fread(file_header, sizeof(char), raw_color_data_offset, file);

	// row_size = 4 * ceil( 3.0 * width / 4.0 ); // 4 bytes 
	// // printf("%d\n", row_size);
	// color_data = new unsigned char[row_size * height * 3];
	// new_color_data = new unsigned char[row_size * height * 3];
	
	// R = new int[width * height];
	// G = new int[width * height];
	// B = new int[width * height];
	// Y = new int[width * height];
	// U = new int[width * height];
	// V = new int[width * height];
	// fread(color_data, sizeof(char), row_size*height*3, file);
	// memcpy(new_color_data, color_data, row_size*height*3);


	// for(int y = 0; y < height; y++) {
	// 	for(int x = 0; x < width; x++) {
	// 		int position = width * y + x;
	// 		R[position] = color_data[(row_size*y + 3*x) + 2];
	// 		G[position] = color_data[(row_size*y + 3*x) + 1];
	// 		B[position] = color_data[(row_size*y + 3*x) + 0];
	// 	}
	// }
	// this->RGB2YUV();
}

BmpFile::~BmpFile(){
	if(file){
		fclose(file);
		file = NULL;		
	}
	if(R)
	delete R,G,B,Y,U,V,color_data,new_color_data;
	bin=new_bin=NULL;
	color_data=new_color_data = NULL;
	R=G=B=Y=U=V=NULL;
}

void BmpFile::save(const char* file_path){
	FILE* save_file = fopen(file_path, "wb");
	if(save_file == NULL)exit(SAVE_FILE_ERROR);
	fwrite(file_header, sizeof(char), raw_color_data_offset, save_file);
	fwrite(new_color_data, sizeof(char), row_size*height, save_file);
	fclose(save_file);
}

void BmpFile::RGB2YUV(){
	min_Y = 0xffff;    // initialize value;
	max_Y = 0;
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int position = width*y + x;
			Y[position] = 0.299*R[position] + 0.587*G[position] + 0.114*B[position];
			U[position] = -0.169*R[position] - 0.331*G[position] + 0.5*B[position] + 128;
			V[position] = 0.5*R[position] -0.419*G[position] - 0.081*B[position] +128;
			if(Y[position] > max_Y)max_Y = Y[position];
			if(Y[position] < min_Y)min_Y = Y[position];
		}
	}
	// std::cout << max_Y << " " << min_Y << std::endl;
}

void BmpFile:: rearrangeY(){
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int position = width*y + x;
			Y[position] = (int)Y[position] * 255 /(max_Y - min_Y);
		}
	}	
}
unsigned char BmpFile::clip_value(double value){
	//printf("%d\n", value);
	//if(value < 0)return 0;
	return value > 255 ? 255 : value;
}

void BmpFile::YUV2RGB(){
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int position = width*y + x;
			//printf("--\n");
			R[position] = clip_value(1.0*Y[position] - 0.00093*U[position] + 1.401687*(V[position]-128));
			G[position] = clip_value(1.0*Y[position] - 0.3437*(U[position]-128) - 0.71417*(V[position]-128));
			B[position] = clip_value(1.0*Y[position] + 1.77216*(U[position]-128) + 0.00099*(V[position]-128));
		}
	}	
}


void BmpFile::convertToGray(){
	this->rearrangeY();
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int position = width*y + x;
			new_color_data[(row_size*y + 3*x) + 2] = Y[position];
			new_color_data[(row_size*y + 3*x) + 1] = Y[position];
			new_color_data[(row_size*y + 3*x) + 0] = Y[position];
			//std::cout << Y[position] << std::endl;
		}
	}
}

void BmpFile::convertToColorful(){
	this->YUV2RGB();
	this->color2data();
}

void BmpFile::color2data(){
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int position = width*y + x;
			new_color_data[(row_size*y + 3*x) + 2] = R[position];
			new_color_data[(row_size*y + 3*x) + 1] = G[position];
			new_color_data[(row_size*y + 3*x) + 0] = B[position];
		}
	}
}

void BmpFile::printFileInfo(){
	std::cout << std::setw(25) <<"file_size: " << std::hex << file_size << " KB" << std::endl;
	std::cout << std::setw(25) <<"width: " << width << " px" << std::endl;
	std::cout << std::setw(25) <<"height: " << height << " px" << std::endl;
	std::cout << std::setw(25) <<"raw_color_data_offset: " << "0x" << std::hex << raw_color_data_offset << "h" << std::dec << std::endl;

}
int BmpFile::calculateThre(int x1, int y1, int x2, int y2){
	int threshold;
	unsigned char min_value = 255;
	unsigned char max_value = 0;
	unsigned long long max_between = 0;
	unsigned long long variance = 0;
	unsigned long long mean = 0;
	int count_fore = 0;
	int count_back = 0;
	double fore_mean = 0;
	double back_mean = 0;
	int num = (x2-x1)*(y2-y1);
	int diff = 0;
	for(int y = y1; y < y2; y++) {
		for(int x = x1; x < x2; x++) {
			int position = width*y + x;
			if(Y[position]>max_value)max_value = Y[position];
			if(Y[position]<min_value)min_value = Y[position];
		}
	}
	// std::cout << max_value << ":" << min_value << std::endl;
	// printf("%d %d \n", min_value, max_value);
	for (int value = min_value; value < max_value; ++value)
	{
		fore_mean = back_mean = 0;
		count_fore = count_back = 0;
		variance = 0;
		for(int y = y1; y < y2; y++) {
			for(int x = x1; x < x2; x++) {
				int position = width*y + x;
				if(Y[position]>value){
					count_fore++;
					fore_mean += Y[position];
				}
				else{
					count_back++;
					back_mean += Y[position];
				}
			}
		}
		fore_mean /= count_fore;
		back_mean /= count_back;
		// std::cout << fore_mean << " " << back_mean << " " << count_fore << " " << count_back << std::endl;
		variance = (fore_mean-back_mean)*(fore_mean-back_mean)*count_fore*count_back/num/num;
		// std::cout << value << "..." << std::hex << variance << std::endl;
		if(variance > max_between){
			max_between = variance;
			threshold = value;
		}
	}
	return threshold;
}

void BmpFile::bin2data(bool* bin_data){
	unsigned char data_array[2] = {255, 0};
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int position = width*y + x;
			new_color_data[(row_size*y + 3*x) + 2] = data_array[(int)bin_data[position]];
			new_color_data[(row_size*y + 3*x) + 1] = data_array[(int)bin_data[position]];
			new_color_data[(row_size*y + 3*x) + 0] = data_array[(int)bin_data[position]];
		}
	}
}
void BmpFile::overall_binarization(){
	this->convertToGray();
	bin = new bool[width*height];
	new_bin = new bool[width*height];
	int threshold = calculateThre(0, 0, width, height);
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int position = width*y + x;
			if(Y[position] > threshold) bin[position] = 0; // 0 is white, background;
			else bin[position] = 1;   // 1 is black, foreground;
		}
	}
	this->bin2data(bin);
}


void BmpFile::local_binarization(){
	this->convertToGray();
	bin = new bool[width*height];
	new_bin = new bool[width*height];
	int threshold = 0;
	int scale = 5;
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int position = width*y + x;
			int x1 = std::max(0, x-scale);
			int y1 = std::max(0, y-scale);
			int x2 = std::min(width, x+scale);
			int y2 = std::min(height, y+scale);
			threshold = calculateThre(x1, y1, x2, y2);
			if(Y[position] > threshold) bin[position] = 0; // 0 is white, background;
			else bin[position] = 1;   // 1 is black, foreground;
		}
	}
	this->bin2data(bin);
}

void BmpFile::erosion(bool* structure, int st_size){
	if(bin == NULL)this->overall_binarization();
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int position = width*y + x;
			bool flag = true;
			int half = st_size / 2;
			for (int i = -1*half; i <= half; ++i)
			{
				for (int j = -1*half; j <= half; ++j)
				{
					int t_pos = position + i*width + j;
					int pos = (i+half)*st_size + (j+half);
					if(t_pos>=0 && x+j<width && structure[pos] && !bin[t_pos]){
						flag = false;
					}
				}
			}
			new_bin[position] = flag;
		}
	}
	this->bin2data(new_bin);
}

void BmpFile::dilation(bool* structure, int st_size){
	if(bin == NULL)this->overall_binarization();
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int position = width*y + x;
			bool flag = false;
			int half = st_size / 2;
			for (int i = -1*half; i <= half; ++i)
			{ 
				for (int j = -1*half; j <= half; ++j)
				{
					int t_pos = position + i*width + j;
					int pos = (i+half)*st_size + (j+half);
					if(t_pos>=0 && x+j<width && structure[pos] && bin[t_pos]){
						flag = true;
					}
				}
			}
			new_bin[position] = flag;
		}
	}
	this->bin2data(new_bin);
}

void BmpFile::opening(bool* structure, int st_size){
	this->erosion(structure, st_size);
	this->dilation(structure, st_size);
}

void BmpFile::closing(bool* structure, int st_size){
	this->dilation(structure, st_size);
	this->erosion(structure, st_size);
}

void BmpFile::log_enhance(){
	this->RGB2YUV();
	double log_max_Y_1 = std::log10(max_Y*1.0/255+1);
	// printf("%d %llf\n", max_Y, log_max_Y_1);
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int position = width*y + x;
			// printf("%d\n", Y[position]);
			Y[position] = std::log10(Y[position]*1.0/255+1)/log_max_Y_1*255;
			// printf("%d\n", Y[position]);
		}
	}
	this->convertToColorful();
}
void BmpFile::fit_one_color(int* X){
	int sum[256] = {0,};
	int pix = width*height;
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int position = width*y + x;
			sum[X[position]] ++;
		}
	}
	for (int level = 1; level < 256; ++level)
	{
		sum[level] += sum[level-1];
	}
	//memset(X, 0, sizeof(unsigned char*)*width*height);
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int position = width*y + x;
			X[position] = ceil(255.0*sum[X[position]]/pix);
			// printf("%d\n", X[position]);
		}
	}
}


void BmpFile::RGB_histogram_fitting(){
	this->fit_one_color(R);
	this->fit_one_color(G);
	this->fit_one_color(B);
	// this->RGB2YUV();
	// this->fit_one_color(Y);
	// this->YUV2RGB();
	this->color2data();
}

void BmpFile::Y_histogram_fitting(){
	//if(Y==NULL)this->RGB2YUV();
	this->fit_one_color(Y);
	this->YUV2RGB();
	this->color2data();
}

