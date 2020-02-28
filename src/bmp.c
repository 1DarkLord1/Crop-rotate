#include "bmp.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

int loadBmp(const char* filePath, PBITMAPFILEHEADER pBitmapFileHeader, PBITMAPINFOHEADER pBitmapInfoHeader, RGBTRIPLE** pImageData) {
	FILE* inputFile = fopen(filePath, "rb");
	if(inputFile == NULL) {
		return OPEN_FILE_ERROR_CODE;
	}
	fseek(inputFile, 0, SEEK_SET);
	fread(pBitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, inputFile);
	fread(pBitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, inputFile);
	*pImageData = (RGBTRIPLE*)malloc(pBitmapInfoHeader->biWidth * pBitmapInfoHeader->biHeight * sizeof(RGBTRIPLE));
	RGBTRIPLE* imageData = *pImageData;
	if(imageData == NULL) {
		fclose(inputFile);
		return MEMORY_ALLOCATION_ERROR_CODE;
	}
	uchar* buffer = (uchar*)malloc(pBitmapInfoHeader->biSizeImage * sizeof(uchar));
	if(buffer == NULL) {
		fclose(inputFile);
		return MEMORY_ALLOCATION_ERROR_CODE;
	}
	fseek(inputFile, pBitmapFileHeader->bfOffBits, SEEK_SET);
	fread(buffer, sizeof(uchar), pBitmapInfoHeader->biSizeImage, inputFile);
	fclose(inputFile);
	int realByteWidth = 3 * pBitmapInfoHeader->biWidth + (ALIGN_SIZE - (3 * pBitmapInfoHeader->biWidth) % ALIGN_SIZE) % ALIGN_SIZE;
	for(int i = 0; i < pBitmapInfoHeader->biHeight; i++) {
		for(int j = 0; j < pBitmapInfoHeader->biWidth; j++) {
			RGBTRIPLE pixel;
			pixel.rgbBlue = buffer[i * realByteWidth + 3 * j];
			pixel.rgbGreen = buffer[i * realByteWidth + 3 * j + 1];
			pixel.rgbRed = buffer[i * realByteWidth + 3 * j + 2];
			imageData[(pBitmapInfoHeader->biHeight - i - 1) * pBitmapInfoHeader->biWidth + j] = pixel;
		}
	}
	free(buffer);
	return 0;
}

int crop(RGBTRIPLE* imageData, RGBTRIPLE** pCropImage, const int imageWidth, const int imageHeight, const int x, const int y, const int cropWidth, const int cropHeight) {
	if(x < 0 || y < 0 || x + cropWidth > imageWidth || y + cropHeight > imageHeight) {
		return BOUNDS_ERROR_CODE;
	}
	*pCropImage = (RGBTRIPLE*)malloc(cropWidth * cropHeight * sizeof(RGBTRIPLE));
	RGBTRIPLE* cropImage = *pCropImage;
	if(cropImage == NULL) {
		return MEMORY_ALLOCATION_ERROR_CODE;
	}
	for(int i = 0; i < cropHeight; i++) {
		for(int j = 0; j < cropWidth; j++) {
			cropImage[i * cropWidth + j] = imageData[(y + i) * imageWidth + x + j]; 
		}
	}
	return 0;  	
}

int rotate(RGBTRIPLE* imageData, const int imageWidth, const int imageHeight) {
	RGBTRIPLE* buffer = (RGBTRIPLE*)malloc(imageWidth * imageHeight * sizeof(RGBTRIPLE));
	if(buffer == NULL) {
		return MEMORY_ALLOCATION_ERROR_CODE;
	}
	for(int i = 0; i < imageHeight; i++) {
		for(int j = 0; j < imageWidth; j++) {
			buffer[j * imageHeight + (imageHeight - i - 1)] = imageData[i * imageWidth + j];
		}
	}
	memcpy(imageData, buffer, imageWidth * imageHeight * sizeof(RGBTRIPLE));
	free(buffer);
	return 0; 
}

int saveBmp(const char* filePath, PBITMAPFILEHEADER pBitmapFileHeader, PBITMAPINFOHEADER pBitmapInfoHeader, RGBTRIPLE* imageData) {
	FILE* outputFile = fopen(filePath, "wb");
	if(outputFile == NULL) {
		return OPEN_FILE_ERROR_CODE;
	}
	int realByteWidth = 3 * pBitmapInfoHeader->biWidth + (ALIGN_SIZE - (3 * pBitmapInfoHeader->biWidth) % ALIGN_SIZE) % ALIGN_SIZE;
	uchar* outputImage = (uchar*)malloc(pBitmapInfoHeader->biSizeImage * sizeof(uchar));
	if(outputImage == NULL) {
		fclose(outputFile);
		return MEMORY_ALLOCATION_ERROR_CODE;
	}
	for(int i = 0; i < pBitmapInfoHeader->biHeight; i++) {
		int iRev = pBitmapInfoHeader->biHeight - i - 1;
		for(int j = 0; j < pBitmapInfoHeader->biWidth; j++) {
			outputImage[iRev * realByteWidth + 3 * j] = imageData[i * pBitmapInfoHeader->biWidth + j].rgbBlue;
			outputImage[iRev * realByteWidth + 3 * j + 1] = imageData[i * pBitmapInfoHeader->biWidth + j].rgbGreen;
			outputImage[iRev * realByteWidth + 3 * j + 2] = imageData[i * pBitmapInfoHeader->biWidth + j].rgbRed;
		}
		for(int j = 3 * pBitmapInfoHeader->biWidth; j < realByteWidth; j++) {
			outputImage[iRev * realByteWidth + j] = 0;
		}
	}
	fseek(outputFile, 0, SEEK_SET);
	fwrite(pBitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(pBitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fseek(outputFile, pBitmapFileHeader->bfOffBits, SEEK_SET);
	fwrite(outputImage, sizeof(uchar) * pBitmapInfoHeader->biSizeImage, 1, outputFile);
	free(outputImage);
	fclose(outputFile);
	return 0;
}

















