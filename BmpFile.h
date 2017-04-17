enum ERRORS{
	OPEN_FILE_ERROR, SAVE_FILE_ERROR, ARGUMENT_ERROR
};

class BmpFile{
public:
	BmpFile();
	~BmpFile();
	void load(std::string);
	void save(std::string);
	void load(const char*);
	void save(const char*);
	void rearrangeY();
	void RGB2YUV();
	void convertToGray();
	void convertToColorful();
	void YUV2RGB();
	void printFileInfo();
	void bin2data(bool*);
	void color2data();
	void local_binarization();
	void overall_binarization();
	void erosion(bool* structure, int st_size);
	void dilation(bool* structure, int st_size);
	void opening(bool* structure, int st_size);
	void closing(bool* structure, int st_size);
	void log_enhance();
	void RGB_histogram_fitting();
	void Y_histogram_fitting();
	void fit_one_color(int *);
	void translate(int, int);
	void rotate(double);
	void scale(double, double);
	void shear(double, char);
	void x_mirror();
	void y_mirror();

private:
	FILE* file;
	int width, height;
	int header_size;
	int file_size;
	int raw_color_data_offset;
	int row_size;
	int bi_size_image;
	unsigned char* color_data;
	unsigned char* new_color_data;
	int* R;
	int* G;
	int* B;
	int* Y;
	int* U;
	int* V;
	bool* bin;
	bool* new_bin;
	int max_Y;
	int min_Y;
	unsigned char* file_header;
	unsigned char clip_value(double);
	int calculateThre(int, int, int, int);
	//void file_load();
};

