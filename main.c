#include "image.h"

int main() {
	clock_t t1 = clock(), t2;
	double d;
	int i;

	/*FILE *in = fopen("images/lena_color.bmp", "rb");
	FILE *out = fopen("images/lena_blurRGB.bmp", "wb");
	if (!in || !out) error(1);

	//copy(in, out);
	//negative(in, out);
	//bright(in, out);
	//black_white(in, out);
	//blur(in, out);
	//rgb2sepia(in, out);
	//rgb2gray(in, out);
	//resize(in, out);
	//rotateRight(in, out);
	//rotate180(in, out);
	//rotateLeft(in, out);
	blurRGB(in, out);*/

	Image *lena = loadImage("images/lena512.bmp");
	Image *color = loadImage("images/lena_color.bmp");
	Image *img1 = loadImage("test/photo1.bmp");
	Image *img2 = loadImage("test/photo2.bmp");
	Image *img3 = loadImage("test/photo3.bmp");
	Image *tour = loadImage("test/tour.bmp");

	//exportImage(copy(tour), "test/tour_copy.bmp");
	//exportImage(negative(tour), "test/tour_negative.bmp");
	//exportImage(copy(img3), "test/photo3_copy.bmp");

	//exportImage(copy(lena), "test/copy1.bmp");
	//exportImage(copy(color), "test/copy2.bmp");
	
	//exportImage(negative(lena), "test/negative_lena.bmp");
	
	//exportImage(add(img2, img3), "test/add23.bmp");

	t2 = clock();
	d = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
	printf("Running time: %lgms\n", d);
}