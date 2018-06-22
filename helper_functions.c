#include "image.h"

void error(int e) {
	switch (e) {
	case 1:
		fprintf(stderr, "Error opening files!\n");
		break;
	case 2:
		fprintf(stderr, "Error in allocation!\n");
		break;
	}
	exit(e);
}