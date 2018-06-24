#include "image.h"

int main() {
	clock_t t1 = clock(), t2;
	double d;
	int i;

	FILE *in = fopen("images/lena512.bmp", "rb");
	FILE *out = fopen("images/lena_rotate.bmp", "wb");
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
	rotate180(in, out);

	t2 = clock();
	d = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
	printf("Running time: %lgms\n", d);
}