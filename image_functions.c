#include "image.h"

/*
	Pravi kopiju slike img, pri cemu kopira i img->data ukoliko je data != 0, u suprotnom sve ostalo. Ovaj parametar
	data je ubacen jer ce ostale funkcije pozivati ovu funkciju sa namerom da se prekopira sve osim samog sadrzaja
	slike, pa ce se pozivati sa data = 0. Ukoliko je cilj da se kopira sama slika, pozivni parametar data nece biti 0.
*/
Image *copy(Image *img, int data) {
	Image *i = malloc(sizeof(Image));
	if (!i) error(2);

	for (int j = 0; j < HEADER_LENGTH; j++)
		i->header[j] = img->header[j];

	i->gap1Length = img->gap1Length;
	if (i->gap1Length) {
		i->gap1 = malloc(img->gap1Length * sizeof(unsigned char));
		if (!i->gap1) error(2);
		for (int j = 0; j < i->gap1Length; j++)
			i->gap1[j] = img->gap1[j];
	}
	else
		i->gap1 = NULL;

	i->gap2Length = img->gap2Length;
	if (i->gap2Length) {
		i->gap2 = malloc(img->gap2Length * sizeof(unsigned char));
		if (!i->gap2) error(2);
		for (int j = 0; j < i->gap2Length; j++)
			i->gap2[j] = img->gap2[j];
	}
	else
		i->gap2 = NULL;

	i->width = img->width;
	i->height = img->height;
	i->bitDepth = img->bitDepth;
	i->size = img->size;
	i->offset = img->offset;

	if (i->bitDepth <= 8)
		for (int j = 0; j < COLOR_TABLE_LENGTH; j++)
			i->colorTable[j] = img->colorTable[j];

	i->data = malloc(i->size * sizeof(unsigned char));
	if (!i->data) error(2);
	if (data)
		for (int j = 0; j < i->size; j++)
			i->data[j] = img->data[j];

	return i;
}

Image *negative(Image *img) {
	Image *neg = copy(img, 0);
	for (int i = 0; i < neg->size; i++)
		neg->data[i] = MAX_COLOR - img->data[i];
	return neg;
}

Image *bright(Image *img) {
	Image *bright = copy(img, 0);
	for (int i = 0; i < bright->size; i++)
		bright->data[i] = MIN(img->data[i] + BRIGHTNESS_FACTOR, MAX_COLOR);
	return bright;
}

Image *black_white(Image *img) {
	Image *bw = copy(img, 0);
	if (bw->bitDepth <= 8)
		for (int i = 0; i < bw->size; i++)
			bw->data[i] = img->data[i] > (MAX_COLOR - MIN_COLOR) / 2 ? MAX_COLOR : MIN_COLOR;
	else	// mozda je rezultat bolji ako se prvo pretvori u grayscale pa onda u crno-belo???
		for (int i = 0; i < bw->size; i += 3)
			bw->data[i] = bw->data[i + 1] = bw->data[i + 2] =
			(img->data[i] + img->data[i + 1] + img->data[i + 2] + 3.5 * BRIGHTNESS_FACTOR) > (MAX_COLOR - MIN_COLOR) * 3 / 2 ? MAX_COLOR : MIN_COLOR;
	return bw;
}

Image *blur(Image *img) {
	const double filter[FILTER_HEIGHT][FILTER_WIDTH] = {
		{ 0, 0, 1, 0, 0 },
		{ 0, 1, 1, 1, 0 },
		{ 1, 1, 1, 1, 1 },
		{ 0, 1, 1, 1, 0 },
		{ 0, 0, 1, 0, 0 }
	};
	const double factor = 1.0 / 13.0;
	const double bias = 0.0;

	Image *blur = copy(img, 0);

	if (blur->bitDepth <= 8)
		for (int times = 0; times < 1; times++) {	// if we want more blur we can make bigger filter or use this one multiple times
			for (int x = 0; x < blur->height; x++)
				for (int y = 0; y < blur->width; y++) {
					double s = 0;
					for (int filterX = 0; filterX < FILTER_HEIGHT; filterX++)
						for (int filterY = 0; filterY < FILTER_WIDTH; filterY++) {
							int imageX = (x - FILTER_HEIGHT / 2 + filterX + blur->height) % blur->height;
							int imageY = (y - FILTER_WIDTH / 2 + filterY + blur->width) % blur->width;
							s += img->data[imageX * blur->width + imageY] * filter[filterX][filterY];
						}

					blur->data[x * blur->width + y] = MIN(MAX((int)(factor * s + bias), 0), 255);
				}
			strcpy(img->data, blur->data);
		}
	else
		for (int times = 0; times < 1; times++) {	// if we want more blur we can make bigger filter or use this one multiple times
			for (int x = 0; x < blur->height; x++)
				for (int y = 0; y < blur->width; y++) {
					double sr = 0, sg = 0, sb = 0;
					for (int filterX = 0; filterX < FILTER_HEIGHT; filterX++)
						for (int filterY = 0; filterY < FILTER_WIDTH; filterY++) {
							int imageX = (x - FILTER_HEIGHT / 2 + filterX + blur->height) % blur->height;
							int imageY = (y - FILTER_WIDTH / 2 + filterY + blur->width) % blur->width;
							sr += img->data[3 * (imageX * blur->width + imageY) + 0] * filter[filterX][filterY];
							sg += img->data[3 * (imageX * blur->width + imageY) + 1] * filter[filterX][filterY];
							sb += img->data[3 * (imageX * blur->width + imageY) + 2] * filter[filterX][filterY];
						}
					blur->data[3 * (x * blur->width + y) + 0] = MIN(MAX((int)(factor * sr), 0), 255);
					blur->data[3 * (x * blur->width + y) + 1] = MIN(MAX((int)(factor * sg), 0), 255);
					blur->data[3 * (x * blur->width + y) + 2] = MIN(MAX((int)(factor * sb), 0), 255);
				}
			strcpy(img->data, blur->data);
		}
	return blur;
}

Image *sepia(Image *img) {
	Image *sepia = copy(img, 0);
	for (int i = 0; i < sepia->size; i += 3) {
		// rgb to sepia
		sepia->data[i + 2] = MIN((img->data[i + 2] * 0.393) + (img->data[i + 1] * 0.769) + (img->data[i] * 0.189), MAX_COLOR);	// red
		sepia->data[i + 1] = MIN((img->data[i + 2] * 0.349) + (img->data[i + 1] * 0.686) + (img->data[i] * 0.168), MAX_COLOR);	// green
		sepia->data[i] = MIN((img->data[i + 2] * 0.272) + (img->data[i + 1] * 0.534) + (img->data[i] * 0.131), MAX_COLOR);		// blue
	}
	return sepia;
}

Image *gray(Image *img) {
	Image *gray = copy(img, 0);
	if (gray->bitDepth <= 8)
		for (int i = 0; i < gray->size; i++)
			gray->data[i] = img->data[i];
	else
		for (int i = 0; i < gray->size; i += 3) {
			// rgb to gray
			gray->data[i + 2] = gray->data[i + 1] = gray->data[i] =
				MIN((img->data[i + 2] * 0.3) + (img->data[i + 1] * 0.59) + (img->data[i] * 0.11), MAX_COLOR);
		}
	return gray;
}

Image *rotate180(Image *img) {
	Image *rotate = copy(img, 0);
	if (rotate->bitDepth <= 8)
		for (int i = 0; i < rotate->size; i++)
			rotate->data[i] = img->data[rotate->size - 1 - i];
	else
		for (int i = 0; i < rotate->size; i += 3) {
			rotate->data[i + 0] = img->data[rotate->size - 1 - (i + 2)];	// blue
			rotate->data[i + 1] = img->data[rotate->size - 1 - (i + 1)];	// green
			rotate->data[i + 2] = img->data[rotate->size - 1 - (i + 0)];	// red
		}
	return rotate;
}

Image *rotateLeft(Image *img) {
	Image *rotate = copy(img, 0);
	if (rotate->bitDepth <= 8)
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++)
				rotate->data[j * img->height + img->height - i - 1] = img->data[img->width * i + j];
	else
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++) {
				rotate->data[3 * (j * img->height + img->height - i - 1) + 0] = img->data[3 * (img->width * i + j) + 0];
				rotate->data[3 * (j * img->height + img->height - i - 1) + 1] = img->data[3 * (img->width * i + j) + 1];
				rotate->data[3 * (j * img->height + img->height - i - 1) + 2] = img->data[3 * (img->width * i + j) + 2];
			}
	*(int*)&rotate->header[18] = img->height;
	*(int*)&rotate->header[22] = img->width;
	rotate->width = img->height;
	rotate->height = img->width;
	return rotate;
}

Image *rotateRight(Image *img) {
	Image *rotate = copy(img, 0);
	if (rotate->bitDepth <= 8)
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++)
				rotate->data[(img->width - j - 1) * img->height + i] = img->data[img->width * i + j];
	else
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++) {
				rotate->data[3 * ((img->width - j - 1) * img->height + i) + 0] = img->data[3 * (img->width * i + j) + 0];
				rotate->data[3 * ((img->width - j - 1) * img->height + i) + 1] = img->data[3 * (img->width * i + j) + 1];
				rotate->data[3 * ((img->width - j - 1) * img->height + i) + 2] = img->data[3 * (img->width * i + j) + 2];
			}
	*(int*)&rotate->header[18] = img->height;
	*(int*)&rotate->header[22] = img->width;
	rotate->width = img->height;
	rotate->height = img->width;
	return rotate;
}