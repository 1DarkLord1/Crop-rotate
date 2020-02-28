#include "bmp.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

const int CROP_ROTATE_ARG_COUNT = 8;

int errorHandler(const int errorCode) {
	if(errorCode == 0) {
		return 0;
	}

	if(errorCode == OPEN_FILE_ERROR_CODE) {
		printf("Open file error!!! ");
	}
	else if(errorCode == MEMORY_ALLOCATION_ERROR_CODE) {
		printf("Memory allocation error!!! ");
	}
	else if(errorCode == BOUNDS_ERROR_CODE) {
		printf("Incorrect begin coordinates or width/height of crop image!!! ");
	}
	return 1;
}

int handlerCropRotate(const char* inBmp, const char* outBmp, const int x, const int y, const int w, const int h) {
	BITMAPFILEHEADER bitmapFileHeader; 
	BITMAPINFOHEADER bitmapInfoHeader;
	RGBTRIPLE* imageData = NULL;
	RGBTRIPLE* cropImage = NULL;
	if(errorHandler(loadBmp(inBmp, &bitmapFileHeader, &bitmapInfoHeader, &imageData))) {
		printf("Loading bmp failed!!!\n");
		return 1;
	}
	if(errorHandler(crop(imageData, &cropImage, bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight, x, y, w, h))) {
		printf("Cropping image failed!!!\n");
		free(imageData);
		return 1;
	}
	if(errorHandler(rotate(cropImage, w, h))) {
		printf("Rotating image failed!!!\n");
		free(imageData);
		free(cropImage);
		return 1;
	}
	bitmapInfoHeader.biWidth = h;
	bitmapInfoHeader.biHeight = w;
	int realByteWidth = 3 * bitmapInfoHeader.biWidth + (ALIGN_SIZE - (3 * bitmapInfoHeader.biWidth) % ALIGN_SIZE) % ALIGN_SIZE;
    bitmapFileHeader.bfSize -= bitmapInfoHeader.biSizeImage; 
	bitmapInfoHeader.biSizeImage = realByteWidth * bitmapInfoHeader.biHeight;
	bitmapFileHeader.bfSize += bitmapInfoHeader.biSizeImage;
	if(errorHandler(saveBmp(outBmp, &bitmapFileHeader, &bitmapInfoHeader, cropImage))) {
		printf("Saving bmp failed!!!\n");
		free(imageData);
		free(cropImage);
		return 1;
	}
	free(imageData);
	free(cropImage);
	return 0;
} 

int main(int argc, char** argv) {
	const char* command = argv[1];
	if(!strcmp(command, "crop-rotate")) {
		if(argc < CROP_ROTATE_ARG_COUNT) {
			printf("Few arguments!!!");
			exit(1);
		}
		const char* inBmp = argv[2];
		const char* outBmp = argv[3];
		int x = atoi(argv[4]), y = atoi(argv[5]);
		int w = atoi(argv[6]), h = atoi(argv[7]);
		if(handlerCropRotate(inBmp, outBmp, x, y, w, h)) {
			exit(1);
		}
	}
	return 0;
}








