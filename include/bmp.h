#ifndef BMP_H
	#define BMP_H
	static const int ALIGN_SIZE = 4;
	static const int OPEN_FILE_ERROR_CODE = 1;
	static const int MEMORY_ALLOCATION_ERROR_CODE = 2;
	static const int BOUNDS_ERROR_CODE = 3;

	typedef unsigned int uint;
	typedef unsigned char uchar;
	typedef unsigned short ushort;
#pragma pack(push)
#pragma pack(1)
	typedef struct tagBITMAPFILEHEADER { 
    	ushort bfType; 
    	uint bfSize; 
    	ushort bfReserved1; 
    	ushort bfReserved2; 
    	uint bfOffBits; 
	} BITMAPFILEHEADER, *PBITMAPFILEHEADER;
	
	typedef struct tagBITMAPINFOHEADER {
		uint biSize; 
		int biWidth;
		int biHeight; 
		ushort biPlanes; 
		ushort biBitCount; 
		uint biCompression; 
		uint biSizeImage; 
		int biXPelsPerMeter; 
		int biYPelsPerMeter; 
		uint biClrUsed; 
		uint biClrImportant; 
	} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
#pragma pack(pop)
	typedef struct tagRGBTRIPLE {
		uchar rgbBlue;
		uchar rgbGreen;
		uchar rgbRed;
	} RGBTRIPLE;
	
	int loadBmp(const char* filePath, PBITMAPFILEHEADER pBitmapFileHeader, PBITMAPINFOHEADER pBitmapInfoHeader, RGBTRIPLE** pImageData);
	int crop(RGBTRIPLE* imageData, RGBTRIPLE** pCropImage, const int imageWidth, const int imageHeight, const int x, const int y, const int cropWidth, const int cropHeight);
	int rotate(RGBTRIPLE* imageData, const int imageWidth, const int imageHeight);
	int saveBmp(const char* filePath, PBITMAPFILEHEADER pBitmapFileHeader, PBITMAPINFOHEADER pBitmapInfoHeader, RGBTRIPLE* imageData);
#endif

