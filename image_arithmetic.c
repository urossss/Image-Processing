#include "image.h"

/* img1 + img2 */
Image* add(Image *img1, Image *img2) {
	if (img1->width != img2->width || img1->height != img2->height || img1->size != img2->size) error(3);
	Image *img = copy(img1, 0);
	for (int i = 0; i < img->size; i++)
		img->data[i] = MIN(img1->data[i] + img2->data[i], MAX_COLOR);
	return img;
}

/* img1 - img2 */
Image* sub(Image *img1, Image *img2) {
	if (img1->width != img2->width || img1->height != img2->height || img1->size != img2->size) error(3);
	Image *img = copy(img1, 0);
	for (int i = 0; i < img->size; i++)
		img->data[i] = MAX(img1->data[i] - img2->data[i], MIN_COLOR);
	return img;
}

/* |img1 - img2| */
Image* diff(Image *img1, Image *img2) {
	if (img1->width != img2->width || img1->height != img2->height || img1->size != img2->size) error(3);
	Image *img = copy(img1, 0);
	for (int i = 0; i < img->size; i++)
		img->data[i] = abs(img1->data[i] - img2->data[i]);
	return img;
}

/* img1 * img2 */
Image* mul(Image *img1, Image *img2) {
	if (img1->width != img2->width || img1->height != img2->height || img1->size != img2->size) error(3);
	Image *img = copy(img1, 0);
	for (int i = 0; i < img->size; i++)
		img->data[i] = (unsigned char)(img1->data[i] * img2->data[i] / 255.0);
	return img;
}

/* (img1 + img2) / 2 */
Image* average(Image *img1, Image *img2) {
	if (img1->width != img2->width || img1->height != img2->height || img1->size != img2->size) error(3);
	Image *img = copy(img1, 0);
	for (int i = 0; i < img->size; i++)
		img->data[i] = (img1->data[i] + img2->data[i]) / 2;
	return img;
}

/* img1 * weight + img2 * (1 - weight), 0 <= weight <= 1 */
Image* cross_fade(Image *img1, Image *img2, double weight) {
	if (img1->width != img2->width || img1->height != img2->height || img1->size != img2->size) error(3);
	Image *img = copy(img1, 0);
	for (int i = 0; i < img->size; i++)
		img->data[i] = (unsigned char)(img1->data[i] * weight + img2->data[i] * (1 - weight));
	return img;
}

/* min(img1, img2) */
Image* minimum(Image *img1, Image *img2) {
	if (img1->width != img2->width || img1->height != img2->height || img1->size != img2->size) error(3);
	Image *img = copy(img1, 0);
	for (int i = 0; i < img->size; i++)
		img->data[i] = MIN(img1->data[i], img2->data[i]);
	return img;
}

/* max(img1, img2) */
Image* maximum(Image *img1, Image *img2) {
	if (img1->width != img2->width || img1->height != img2->height || img1->size != img2->size) error(3);
	Image *img = copy(img1, 0);
	for (int i = 0; i < img->size; i++)
		img->data[i] = MAX(img1->data[i], img2->data[i]);
	return img;
}

/* sqrt(img1^2 + img2^2) */
Image* amplitude(Image *img1, Image *img2) {
	if (img1->width != img2->width || img1->height != img2->height || img1->size != img2->size) error(3);
	Image *img = copy(img1, 0);
	for (int i = 0; i < img->size; i++)
		img->data[i] = (unsigned char)(sqrt(img1->data[i] * img1->data[i] + img2->data[i] * img2->data[i]) / sqrt(2.0));
	return img;
}

/* img1 & img2 */
Image* and(Image *img1, Image *img2) {
	if (img1->width != img2->width || img1->height != img2->height || img1->size != img2->size) error(3);
	Image *img = copy(img1, 0);
	for (int i = 0; i < img->size; i++)
		img->data[i] = img1->data[i] & img2->data[i];
	return img;
}

/* img1 | img2 */
Image* or(Image *img1, Image *img2) {
	if (img1->width != img2->width || img1->height != img2->height || img1->size != img2->size) error(3);
	Image *img = copy(img1, 0);
	for (int i = 0; i < img->size; i++)
		img->data[i] = img1->data[i] | img2->data[i];
	return img;
}

/* img1 ^ img2 */
Image* xor(Image *img1, Image *img2) {
	if (img1->width != img2->width || img1->height != img2->height || img1->size != img2->size) error(3);
	Image *img = copy(img1, 0);
	for (int i = 0; i < img->size; i++)
		img->data[i] = img1->data[i] ^ img2->data[i];
	return img;
}