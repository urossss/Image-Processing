#include "image.h"

void error(int e) {
	switch (e) {
	case 1:
		fprintf(stderr, "Error opening files!\n");
		break;
	case 2:
		fprintf(stderr, "Error in allocation!\n");
		break;
	case 3:
		fprintf(stderr, "Images must have the same dimensions to perform this operation!\n");
		break;
	}
	exit(e);
}

Image* loadImage(char name[]) {
	FILE *in = fopen(name, "rb");
	if (!in) error(1);

	Image *i = malloc(sizeof(Image));
	if (!i) error(2);
	fread(i->header, sizeof(unsigned char), HEADER_LENGTH, in);
	i->width = *(int*)&i->header[18];
	i->height = *(int*)&i->header[22];
	i->bitDepth = *(int*)&i->header[28];
	i->size = i->width * i->height;

	if (i->bitDepth <= 8)
		fread(i->colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, in);
	else i->size *= i->bitDepth / 8;	// color
	
	i->data = malloc(i->size * sizeof(unsigned char));
	if (!i->data) error(2);

	fread(i->data, sizeof(unsigned char), i->size, in);

	fread(i->footer, sizeof(unsigned char), FOOTER_LENGTH, in);

	fclose(in);
	return i;
}

void exportImage(Image *i, char name[]) {
	FILE *out = fopen(name, "wb");
	if (!out) error(1);
	fwrite(i->header, sizeof(unsigned char), HEADER_LENGTH, out);
	if (i->bitDepth <= 8)
		fwrite(i->colorTable, sizeof(unsigned char), COLOR_TABLE_LENGTH, out);
	fwrite(i->data, sizeof(unsigned char), i->size, out);
	fwrite(i->footer, sizeof(unsigned char), FOOTER_LENGTH, out);
	fclose(out);
}