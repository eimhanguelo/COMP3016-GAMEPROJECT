#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include"gl/glut.h"
#include "texture.h"
#define BITMAP_ID 0x3D42

unsigned char *LoadBitmapFile(const char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;   
	BITMAPFILEHEADER bitmapFileHeader;
	unsigned char    *bitmapImage;       
	int    imageIdx = 0;       
	unsigned char    tempRGB;    

					
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL) {
		printf("file not open\n");
		return NULL;
	}

	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];

	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	fclose(filePtr);
	return bitmapImage;
}

GLuint LoadTexture(const string filename) {
	BITMAPINFOHEADER bitmapInfoHeader;                               
	unsigned char*   bitmapData;                                      
	const char* fp = filename.c_str();
	bitmapData = LoadBitmapFile(fp, &bitmapInfoHeader);
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	GLenum image_format = GL_RGB;
	GLint internal_format = GL_BGRA_EXT;

	glTexImage2D(GL_TEXTURE_2D, 0, image_format, bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight, 0, internal_format, GL_UNSIGNED_BYTE, bitmapData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return id;
}


