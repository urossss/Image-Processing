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