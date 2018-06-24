#ifndef _HEADER_
#define _HEADER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

/* functions */
void error(int);
void copy(FILE *, FILE *);
void negative(FILE *, FILE *);
void bright(FILE *, FILE *);
void black_white(FILE *, FILE *);
void blur(FILE *, FILE *);
void rgb2sepia(FILE *, FILE *);
void rgb2gray(FILE *, FILE *);
void resize(FILE *, FILE *);
void rotateRight(FILE *, FILE *);
void rotate180(FILE *, FILE *);
void rotateLeft(FILE *, FILE *);
void blurRGB(FILE *, FILE *);

#endif