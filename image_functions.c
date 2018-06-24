#include "image.h"

void copy(FILE *src, FILE *dst) {
	unsigned char header[HEADER_LENGTH];	// to store the image header
	unsigned char colorTable[COLOR_TABLE_LENGTH];	// to store the colorTable, if it exists.

	fread(header, sizeof(unsigned char), HEADER_LENGTH, src);	// header

	int width = *(int*)&header[18];	// read the width from the image header
	int height = *(int*)&header[22];	// read the height from the image header
	int bitDepth = *(int*)&header[28];	// read the bitDepth from the image header

	if (bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, src);	// color table

	unsigned char *buff = malloc(width * height * sizeof(unsigned char));
	if (!buff) error(2);

	fread(buff, sizeof(unsigned char), height * width, src);	// image data

	fwrite(header, sizeof(unsigned char), HEADER_LENGTH, dst); // write the image header to output file
	if (bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, dst);
	fwrite(buff, sizeof(unsigned char), height * width, dst);

	fclose(src);
	fclose(dst);
	free(buff);
}

void negative(FILE *src, FILE *dst) {
	unsigned char header[HEADER_LENGTH];
	unsigned char colorTable[COLOR_TABLE_LENGTH];

	fread(header, sizeof(unsigned char), HEADER_LENGTH, src);

	int width = *(int*)&header[18];
	int height = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];
	int imageSize = width * height;

	if (bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, src);

	unsigned char *imageData = malloc(imageSize * sizeof(unsigned char));
	unsigned char *newImageData = malloc(imageSize * sizeof(unsigned char));
	if (!imageData || !newImageData) error(2);

	fread(imageData, sizeof(unsigned char), imageSize, src);

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			newImageData[i * width + j] = MAX_COLOR - imageData[i * width + j];

	fwrite(header, sizeof(unsigned char), HEADER_LENGTH, dst);
	if (bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, dst);
	fwrite(newImageData, sizeof(unsigned char), imageSize, dst);

	fclose(src);
	fclose(dst);
	free(imageData);
	free(newImageData);
}

void bright(FILE *src, FILE *dst) {
	unsigned char header[HEADER_LENGTH];
	unsigned char colorTable[COLOR_TABLE_LENGTH];

	fread(header, sizeof(unsigned char), HEADER_LENGTH, src);

	int width = *(int*)&header[18];
	int height = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];
	int imageSize = width * height;

	if (bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, src);

	unsigned char *imageData = malloc(imageSize * sizeof(unsigned char));
	if (!imageData) error(2);

	fread(imageData, sizeof(unsigned char), imageSize, src);

	for (int i = 0; i < imageSize; i++)
		imageData[i] = MIN(MAX_COLOR, imageData[i] + BRIGHTNESS_FACTOR);

	fwrite(header, sizeof(unsigned char), HEADER_LENGTH, dst);
	if (bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, dst);
	fwrite(imageData, sizeof(unsigned char), imageSize, dst);

	fclose(src);
	fclose(dst);
	free(imageData);
}

void black_white(FILE *src, FILE *dst) {
	unsigned char header[HEADER_LENGTH];
	unsigned char colorTable[COLOR_TABLE_LENGTH];

	fread(header, sizeof(unsigned char), HEADER_LENGTH, src);

	int width = *(int*)&header[18];
	int height = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];
	int imageSize = width * height;

	if (bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, src);

	unsigned char *imageData = malloc(imageSize * sizeof(unsigned char));
	if (!imageData) error(2);

	fread(imageData, sizeof(unsigned char), imageSize, src);

	for (int i = 0; i < imageSize; i++)
		imageData[i] = imageData[i] > (MAX_COLOR - MIN_COLOR) / 2 ? MAX_COLOR : MIN_COLOR;

	fwrite(header, sizeof(unsigned char), HEADER_LENGTH, dst);
	if (bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, dst);
	fwrite(imageData, sizeof(unsigned char), imageSize, dst);

	fclose(src);
	fclose(dst);
	free(imageData);
}

void blur(FILE *src, FILE *dst) {
	unsigned char header[HEADER_LENGTH];
	unsigned char colorTable[COLOR_TABLE_LENGTH];

	fread(header, sizeof(unsigned char), HEADER_LENGTH, src);

	int width = *(int*)&header[18];
	int height = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];
	int imageSize = width * height;

	if (bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, src);

	unsigned char *imageData = malloc(imageSize * sizeof(unsigned char));
	unsigned char *newImageData = malloc(imageSize * sizeof(unsigned char));
	if (!imageData || !newImageData) error(2);

	fread(imageData, sizeof(unsigned char), imageSize, src);

	const double filter[FILTER_HEIGHT][FILTER_WIDTH] = {
		{ 0, 0, 1, 0, 0 },
		{ 0, 1, 1, 1, 0 },
		{ 1, 1, 1, 1, 1 },
		{ 0, 1, 1, 1, 0 },
		{ 0, 0, 1, 0, 0 }
	};
	const double factor = 1. / 13.;

	for (int times = 0; times < 1; times++) {	// if we want more blur we can make bigger filter or use this one multiple times
		for (int x = 0; x < height; x++)
			for (int y = 0; y < width; y++) {
				double s = 0;
				for (int filterX = 0; filterX < FILTER_HEIGHT; filterX++)
					for (int filterY = 0; filterY < FILTER_WIDTH; filterY++) {
						int imageX = (x - FILTER_HEIGHT / 2 + filterX + height) % height;
						int imageY = (y - FILTER_WIDTH / 2 + filterY + width) % width;
						s += imageData[imageX * width + imageY] * filter[filterX][filterY];
					}

				newImageData[x * width + y] = MIN(MAX((int)(factor * s), 0), 255);
			}
		strcpy(imageData, newImageData);
	}


	fwrite(header, sizeof(unsigned char), HEADER_LENGTH, dst);
	if (bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, dst);
	fwrite(newImageData, sizeof(unsigned char), imageSize, dst);

	fclose(src);
	fclose(dst);
	free(imageData);
}

void rgb2sepia(FILE *src, FILE *dst) {
	unsigned char header[HEADER_LENGTH];
	unsigned char colorTable[COLOR_TABLE_LENGTH];

	fread(header, sizeof(unsigned char), HEADER_LENGTH, src);

	int width = *(int*)&header[18];
	int height = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];
	int imageSize = width * height;

	if (bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, src);

	fwrite(header, sizeof(unsigned char), HEADER_LENGTH, dst);
	if (bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, dst);

	unsigned char buffer[3], r, g, b;
	for (int i = 0; i < imageSize; i++) {
		buffer[2] = getc(src);	// blue
		buffer[1] = getc(src);	// green
		buffer[0] = getc(src);	// red

		// conversion formula of rgb to sepia
		r = MIN((buffer[0] * 0.393) + (buffer[1] * 0.769) + (buffer[2] * 0.189), MAX_COLOR);
		g = MIN((buffer[0] * 0.349) + (buffer[1] * 0.686) + (buffer[2] * 0.168), MAX_COLOR);
		b = MIN((buffer[0] * 0.272) + (buffer[1] * 0.534) + (buffer[2] * 0.131), MAX_COLOR);

		putc(b, dst);
		putc(g, dst);
		putc(r, dst);
	}

	fclose(src);
	fclose(dst);
}

void rgb2gray(FILE *src, FILE *dst) {
	unsigned char header[HEADER_LENGTH];
	unsigned char colorTable[COLOR_TABLE_LENGTH];

	fread(header, sizeof(unsigned char), HEADER_LENGTH, src);

	int width = *(int*)&header[18];
	int height = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];
	int imageSize = width * height;

	if (bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, src);

	fwrite(header, sizeof(unsigned char), HEADER_LENGTH, dst);
	if (bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, dst);

	unsigned char buffer[3], y;
	for (int i = 0; i < imageSize; i++) {
		buffer[2] = getc(src);	// blue
		buffer[1] = getc(src);	// green
		buffer[0] = getc(src);	// red

		// conversion formula of rgb to gray
		y = MIN((buffer[0] * 0.3) + (buffer[1] * 0.59) + (buffer[2] * 0.11), MAX_COLOR);

		putc(y, dst);
		putc(y, dst);
		putc(y, dst);
	}

	fclose(src);
	fclose(dst);
}

void resize(FILE *src, FILE *dst) {
	unsigned char header[HEADER_LENGTH];	// to store the image header
	unsigned char colorTable[COLOR_TABLE_LENGTH];	// to store the colorTable, if it exists.

	fread(header, sizeof(unsigned char), HEADER_LENGTH, src);	// header

	int width = *(int*)&header[18];	// read the width from the image header
	int height = *(int*)&header[22];	// read the height from the image header
	int bitDepth = *(int*)&header[28];	// read the bitDepth from the image header

	if (bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, src);	// color table

	unsigned char *buff = malloc(width * height * sizeof(unsigned char));
	if (!buff) error(2);

	fread(buff, sizeof(unsigned char), height * width, src);	// image data

	int newWidth = 1024, newHeight = 256;
	// scanf("%d%d", &newWidth, &newHeight);
	if (newWidth * newHeight <= width * height) {
		*(int*)&header[18] = newWidth;
		*(int*)&header[22] = newHeight;
	}

	fwrite(header, sizeof(unsigned char), HEADER_LENGTH, dst); // write the image header to output file
	if (bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, dst);
	fwrite(buff, sizeof(unsigned char), height * width, dst);

	fclose(src);
	fclose(dst);
	free(buff);
}

void rotateRight(FILE *src, FILE *dst) {
	unsigned char header[HEADER_LENGTH];
	unsigned char colorTable[COLOR_TABLE_LENGTH];

	fread(header, sizeof(unsigned char), HEADER_LENGTH, src);

	int width = *(int*)&header[18];
	int height = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];
	int imageSize = width * height;

	if (bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, src);

	unsigned char *imageData = malloc(imageSize * sizeof(unsigned char));
	unsigned char *newImageData = malloc(imageSize * sizeof(unsigned char));
	if (!imageData || !newImageData) error(2);

	fread(imageData, sizeof(unsigned char), imageSize, src);

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			newImageData[(width - j - 1) * height + i] = imageData[width * i + j];

	*(int*)&header[18] = height;
	*(int*)&header[22] = width;

	fwrite(header, sizeof(unsigned char), HEADER_LENGTH, dst);
	if (bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, dst);
	fwrite(newImageData, sizeof(unsigned char), imageSize, dst);

	fclose(src);
	fclose(dst);
	free(imageData);
	free(newImageData);
}

void rotate180(FILE *src, FILE *dst) {
	unsigned char header[HEADER_LENGTH];
	unsigned char colorTable[COLOR_TABLE_LENGTH];

	fread(header, sizeof(unsigned char), HEADER_LENGTH, src);

	int width = *(int*)&header[18];
	int height = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];
	int imageSize = width * height;

	if (bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, src);

	unsigned char *imageData = malloc(imageSize * sizeof(unsigned char));
	if (!imageData) error(2);

	fread(imageData, sizeof(unsigned char), imageSize, src);

	unsigned char tmp;
	for (int i = 0, j = imageSize - 1; i < j; i++, j--) {
		tmp = imageData[i];
		imageData[i] = imageData[j];
		imageData[j] = tmp;
	}

	fwrite(header, sizeof(unsigned char), HEADER_LENGTH, dst);
	if (bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, dst);
	fwrite(imageData, sizeof(unsigned char), imageSize, dst);

	fclose(src);
	fclose(dst);
	free(imageData);
}

void rotateLeft(FILE *src, FILE *dst) {
	unsigned char header[HEADER_LENGTH];
	unsigned char colorTable[COLOR_TABLE_LENGTH];

	fread(header, sizeof(unsigned char), HEADER_LENGTH, src);

	int width = *(int*)&header[18];
	int height = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];
	int imageSize = width * height;

	if (bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, src);

	unsigned char *imageData = malloc(imageSize * sizeof(unsigned char));
	unsigned char *newImageData = malloc(imageSize * sizeof(unsigned char));
	if (!imageData || !newImageData) error(2);

	fread(imageData, sizeof(unsigned char), imageSize, src);

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			newImageData[j * height + height - i - 1] = imageData[width * i + j];

	*(int*)&header[18] = height;
	*(int*)&header[22] = width;

	fwrite(header, sizeof(unsigned char), HEADER_LENGTH, dst);
	if (bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, dst);
	fwrite(newImageData, sizeof(unsigned char), imageSize, dst);

	fclose(src);
	fclose(dst);
	free(imageData);
	free(newImageData);
}

void blurRGB(FILE *src, FILE *dst) {
	unsigned char header[HEADER_LENGTH];
	unsigned char colorTable[COLOR_TABLE_LENGTH];

	fread(header, sizeof(unsigned char), HEADER_LENGTH, src);
	fwrite(header, sizeof(unsigned char), HEADER_LENGTH, dst);

	int width = *(int*)&header[18];
	int height = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];
	int imageSize = width * height;
	int i, x, y, filterX, filterY;

	if (bitDepth <= 8) {
		fread(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, src);
		fwrite(colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, dst);
	}

	unsigned char *imageData = malloc(3 * imageSize * sizeof(unsigned char));
	unsigned char *newImageData = malloc(3 * imageSize * sizeof(unsigned char));
	if (!imageData || !newImageData) error(2);

	for (i = 0; i < imageSize; i++) {
		imageData[3 * i + 2] = getc(src);	// blue
		imageData[3 * i + 1] = getc(src);	// green
		imageData[3 * i + 0] = getc(src);	// red
	}

	const double filter[FILTER_HEIGHT][FILTER_WIDTH] = {
		{ 0, 0, 1, 0, 0 },
		{ 0, 1, 1, 1, 0 },
		{ 1, 1, 1, 1, 1 },
		{ 0, 1, 1, 1, 0 },
		{ 0, 0, 1, 0, 0 }
	};
	const double factor = 1. / 13.;

	double sr, sg, sb;
	for (int times = 0; times < 1; times++) {	// if we want more blur we can make bigger filter or use this one multiple times
		for (x = 0; x < height; x++)
			for (y = 0; y < width; y++) {
				sr = sg = sb = 0;
				for (filterX = 0; filterX < FILTER_HEIGHT; filterX++)
					for (int filterY = 0; filterY < FILTER_WIDTH; filterY++) {
						int imageX = (x - FILTER_HEIGHT / 2 + filterX + height) % height;
						int imageY = (y - FILTER_WIDTH / 2 + filterY + width) % width;
						sr += imageData[3 * (imageX * width + imageY) + 0] * filter[filterX][filterY];
						sg += imageData[3 * (imageX * width + imageY) + 1] * filter[filterX][filterY];
						sb += imageData[3 * (imageX * width + imageY) + 2] * filter[filterX][filterY];
					}
				newImageData[3 * (x * width + y) + 0] = MIN(MAX((int)(factor * sr), 0), 255);
				newImageData[3 * (x * width + y) + 1] = MIN(MAX((int)(factor * sg), 0), 255);
				newImageData[3 * (x * width + y) + 2] = MIN(MAX((int)(factor * sb), 0), 255);
			}
		strcpy(imageData, newImageData);
	}

	for (i = 0; i < imageSize; i++) {
		putc(newImageData[3 * i + 2], dst);
		putc(newImageData[3 * i + 1], dst);
		putc(newImageData[3 * i + 0], dst);
	}

	fclose(src);
	fclose(dst);
	free(imageData);
	free(newImageData);
}
