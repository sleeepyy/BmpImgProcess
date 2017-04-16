#include <bits/stdc++.h>
#include "BmpFile.h"
using namespace std;

BmpFile::BmpFile():new_color_data(NULL),color_data(NULL){
	file = NULL;
	bin = NULL;
	new_bin = NULL;
	R=G=B=Y=U=V=NULL;
}
void BmpFile::load(string file_path){
	this->~BmpFile();
	ifstream infile(file_path, ios::in|ios::binary);
	if(!infile){
		std::cout << "Open file failed." << std::endl;
		exit(OPEN_FILE_ERROR);
	}
	// cout << "start to process img" <<endl;
	infile.seekg(2, ios::beg);
	infile.read((char*)&file_size, sizeof(int));
	infile.seekg(34, ios::beg);
	infile.read((char*)&bi_size_image, sizeof(int));
	infile.seekg(10, ios::beg);
	infile.read((char*)&raw_color_data_offset, sizeof(int));
	infile.seekg(18,ios::beg);
	infile.read((char*)&width,sizeof(int));
	infile.read((char*)&height,sizeof(int));
	row_size = 4 * ceil( 3.0 * width / 4.0 ); // 4 bytes 

	color_data = new unsigned char[row_size * height];
	new_color_data = new unsigned char[row_size * height];
	R = new int[width * height];
	G = new int[width * height];
	B = new int[width * height];
	Y = new int[width * height];
	U = new int[width * height];
	V = new int[width * height];
	infile.seekg(raw_color_data_offset, ios::beg);
	infile.read((char*)color_data, row_size*height);
	memcpy(new_color_data, color_data, row_size*height);

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
	string file_p(file_path);
	this->load(file_p);
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

void BmpFile::save(string file_path){
	char header[54] = {
	  0x42,        // identity : B
	  0x4d,        // identity : M
	  0, 0, 0, 0,  // file size
	  0, 0,        // reserved1
	  0, 0,        // reserved2
	  54, 0, 0, 0, // RGB data offset
	  40, 0, 0, 0, // struct BITMAPINFOHEADER size
	  0, 0, 0, 0,  // bmp width
	  0, 0, 0, 0,  // bmp height
	  1, 0,        // planes
	  24, 0,       // bit per pixel
	  0, 0, 0, 0,  // compression
	  0, 0, 0, 0,  // data size
	  0, 0, 0, 0,  // h resolution
	  0, 0, 0, 0,  // v resolution 
	  0, 0, 0, 0,  // used colors
	  0, 0, 0, 0   // important colors
	};

	ofstream outfile(file_path, ios::out|ios::binary);
	 
	if(!outfile){
		cout << " Write image error!!" << endl;
		exit(SAVE_FILE_ERROR);
	}

	// file size = width * height * 3 + rgb_raw_data_offset  ;
	header[2] = (unsigned char)(file_size & 0x000000ff);
	header[3] = (file_size >> 8)  & 0x000000ff;
	header[4] = (file_size >> 16) & 0x000000ff;
	header[5] = (file_size >> 24) & 0x000000ff;
	
	// width
	header[18] = width & 0x000000ff;
	header[19] = (width >> 8)  & 0x000000ff;
	header[20] = (width >> 16) & 0x000000ff;
	header[21] = (width >> 24) & 0x000000ff;

	// height
	header[22] = height &0x000000ff;
	header[23] = (height >> 8)  & 0x000000ff;
	header[24] = (height >> 16) & 0x000000ff;
	header[25] = (height >> 24) & 0x000000ff;  

	header[34] = bi_size_image &0x000000ff;
	header[35] = (bi_size_image >> 8)  & 0x000000ff;
	header[36] = (bi_size_image >> 16) & 0x000000ff;
	header[37] = (bi_size_image >> 24) & 0x000000ff; 

	// write header
	outfile.write(header,54);

	// write image
	if(new_color_data != NULL){
		outfile.write((char*)new_color_data,sizeof(char)* row_size * height);
	}
	else{
		outfile.write((char*)color_data,sizeof(char)* row_size * height);
	}
	outfile.close();
}


void BmpFile::save(const char* file_path){
	string file_p(file_path);
	this->save(file_p);
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
	value = value > 255 ? 255 : value;
	value = value < 0 ? 0 : value;
	return value;
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
	std::cout << std::setw(25) <<"file_size: " << std::dec << file_size << " KB" << std::endl;
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


void BmpFile::translate(int x, int y){
	int new_width = width + abs(x);
	int new_height = height + abs(y);
	int new_rowsize = ((new_width*3-1)/4+1)*4;

	int* temp_R = new int[width*height];
	int* temp_B = new int[width*height];
	int* temp_G = new int[width*height];
	memcpy(temp_R, R, sizeof(int)*width*height);
	memcpy(temp_G, G, sizeof(int)*width*height);
	memcpy(temp_B, B, sizeof(int)*width*height);

	delete[] new_color_data;
	delete[] R, G, B;
	delete[] Y, U, V;
	delete[] new_bin;
	new_color_data = new unsigned char[new_rowsize*new_height];
	memset(new_color_data, 0, sizeof(unsigned char)*new_rowsize*new_height);

	R = new int[new_width*new_height];
	G = new int[new_width*new_height];
	B = new int[new_width*new_height];

	Y = new int[new_width*new_height];
	U = new int[new_width*new_height];
	V = new int[new_width*new_height];
	new_bin = new bool[new_width*new_height];

	int tx, ty;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			int position = i*width + j;
			tx = x>0?x:0;
			ty = y<0?-1*y:0;
			R[(i+ty)*new_width+j+tx] = temp_R[position];
			G[(i+ty)*new_width+j+tx] = temp_G[position];
			B[(i+ty)*new_width+j+tx] = temp_B[position];
		}
	}
	delete[] temp_R, temp_G, temp_B;
	width = new_width;
	height = new_height;
	row_size = new_rowsize;
	file_size = row_size*height + 54;
	this->color2data();
	this->RGB2YUV();
}
// void BmpFile::rotate(double);
// void BmpFile::scale(int);
// void BmpFile::shear(int);
void BmpFile::x_mirror(){
	int half = height/2;
	int temp = 0;
	for (int i = 0; i < half; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			int position1 = i*width + j;
			int position2 = (height-i-1)*width + j;
			temp = R[position1];
			R[position1] = R[position2];
			R[position2] = temp;
			temp = G[position1];
			G[position1] = G[position2];
			G[position2] = temp;
			temp = B[position1];
			B[position1] = B[position2];
			B[position2] = temp;
		}
	}
	this->color2data();
}
void BmpFile::y_mirror(){
	int half = width/2;
	int temp = 0;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < half; ++j)
		{
			int position1 = i*width + j;
			int position2 = i*width + width-j-1;
			temp = R[position1];
			R[position1] = R[position2];
			R[position2] = temp;
			temp = G[position1];
			G[position1] = G[position2];
			G[position2] = temp;
			temp = B[position1];
			B[position1] = B[position2];
			B[position2] = temp;
		}
	}
	this->color2data();
}

