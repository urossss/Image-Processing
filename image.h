#ifndef _HEADER_
#define _HEADER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define HEADER_LENGTH 54
#define COLOR_TABLE_LENGTH 1024

#define MIN_COLOR 0
#define MAX_COLOR 255

#define BRIGHTNESS_FACTOR 25

#define FILTER_WIDTH 5
#define FILTER_HEIGHT 5

#define MAX(a, b) (a) > (b) ? (a) : (b)
#define MIN(a, b) (a) < (b) ? (a) : (b)

typedef struct {
	unsigned char header[HEADER_LENGTH];
	unsigned char colorTable[COLOR_TABLE_LENGTH];
	int width, height, bitDepth, size, offset, gap1Length, gap2Length;
	unsigned char *data, *gap1, *gap2;
} Image;

/* helper functions */
void error(int);
Image *loadImage(char[]);
void exportImage(Image *, char[]);

/* image functions - old version */
void copy_old(FILE *, FILE *);
void negative_old(FILE *, FILE *);
void bright_old(FILE *, FILE *);
void black_white_old(FILE *, FILE *);
void blur_old(FILE *, FILE *);
void rgb2sepia_old(FILE *, FILE *);
void rgb2gray_old(FILE *, FILE *);
void resize_old(FILE *, FILE *);
void rotateRight_old(FILE *, FILE *);
void rotate180_old(FILE *, FILE *);
void rotateLeft_old(FILE *, FILE *);
void blurRGB_old(FILE *, FILE *);

/* image functions */
Image* copy(Image *, int);
Image* negative(Image *);
Image* bright(Image *);
Image* black_white(Image *);
Image* blur(Image *);
Image* sepia(Image *);
Image* gray(Image *);
Image* rotate180(Image *);
Image* rotateLeft(Image *);
Image* rotateRight(Image *);
Image* resize(Image *, int, int);

/* image arithmetic */
Image* add(Image *, Image *);
Image* sub(Image *, Image *);
Image* diff(Image *, Image *);
Image* mul(Image *, Image *);
Image* average(Image *, Image *);
Image* cross_fade(Image *, Image *, double);
Image* minimum(Image *, Image *);
Image* maximum(Image *, Image *);
Image* amplitude(Image *, Image *);
Image* and(Image *, Image *);
Image* or(Image *, Image *);
Image* xor(Image *, Image *);

#endif