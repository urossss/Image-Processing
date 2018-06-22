#include "image.h"

void copy(FILE *src, FILE *dst) {
	unsigned char header[54];	// to store the image header
	unsigned char colorTable[1024];	// to store the colorTable, if it exists.

	fread(header, sizeof(unsigned char), 54, src);	// header

	int width = *(int*)&header[18];	// read the width from the image header
	int height = *(int*)&header[22];	// read the height from the image header
	int bitDepth = *(int*)&header[28];	// read the bitDepth from the image header

	if (bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), 1024, src);	// color table

	unsigned char *buff = malloc(width * height * sizeof(unsigned char));
	if (!buff) error(2);

	fread(buff, sizeof(unsigned char), height * width, src);	// image data

	fwrite(header, sizeof(unsigned char), 54, dst); // write the image header to output file
	if (bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), 1024, dst);
	fwrite(buff, sizeof(unsigned char), height * width, dst);

	fclose(src);
	fclose(dst);
	free(buff);
}

void negative(FILE *src, FILE *dst) {
	unsigned char header[54];
	unsigned char colorTable[1024];

	fread(header, sizeof(unsigned char), 54, src);

	int width = *(int*)&header[18];
	int height = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];
	int imageSize = width * height;

	if (bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), 1024, src);

	unsigned char *imageData = malloc(imageSize * sizeof(unsigned char));
	unsigned char *newImageData = malloc(imageSize * sizeof(unsigned char));
	if (!imageData || !newImageData) error(2);

	fread(imageData, sizeof(unsigned char), imageSize, src);

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			newImageData[i * width + j] = MAX_COLOR - imageData[i * width + j];

	fwrite(header, sizeof(unsigned char), 54, dst);
	if (bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), 1024, dst);
	fwrite(newImageData, sizeof(unsigned char), imageSize, dst);

	fclose(src);
	fclose(dst);
	free(imageData);
	free(newImageData);
}

void bright(FILE *src, FILE *dst) {
	unsigned char header[54];
	unsigned char colorTable[1024];

	fread(header, sizeof(unsigned char), 54, src);

	int width = *(int*)&header[18];
	int height = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];
	int imageSize = width * height;

	if (bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), 1024, src);

	unsigned char *imageData = malloc(imageSize * sizeof(unsigned char));
	if (!imageData) error(2);

	fread(imageData, sizeof(unsigned char), imageSize, src);

	for (int i = 0; i < imageSize; i++)
		imageData[i] = MIN(MAX_COLOR, imageData[i] + BRIGHTNESS_FACTOR);

	fwrite(header, sizeof(unsigned char), 54, dst);
	if (bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), 1024, dst);
	fwrite(imageData, sizeof(unsigned char), imageSize, dst);

	fclose(src);
	fclose(dst);
	free(imageData);
}