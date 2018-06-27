#include "image.h"

Image* add(Image *img1, Image *img2) {
	if (img1->width != img2->width || img1->height != img2->height || img1->size != img2->size) error(3);
	Image *img = malloc(sizeof(Image));
	if (!img) error(2);
	for (int i = 0; i < HEADER_LENGTH; i++)	// zasto ne radi strcpy???
		img->header[i] = img1->header[i];
	img->footer = malloc(img1->footerLength * sizeof(unsigned char));
	if (!img->footer) error(2);
	img->footerLength = img1->footerLength;
	for (int i = 0; i < img->footerLength; i++)
		img->footer[i] = img1->footer[i];
	img->width = img1->width;
	img->height = img1->height;
	img->bitDepth = img1->bitDepth;
	img->size = img1->size;
	if (img->bitDepth <= 8)
		for (int i = 0; i < COLOR_TABLE_LENGTH; i++)
			img->colorTable[i] = img1->colorTable[i];
	img->data = malloc(img->size * sizeof(unsigned char));
	if (!img->data) error(2);
	for (int i = 0; i < img->size; i++)
		img->data[i] = MIN(img1->data[i] + img2->data[i], 255);
	return img;
}