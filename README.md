# Image-Processing

Here are some simple algorithms for image processing in C without use of any external libraries.

These functions are applied to bitmap (.bmp) files, because the bitmap format is probably the easiest to read,
but the algorithms should be the same for different file formats. Detailed information about bmp file format
can be found at wiki page: https://en.wikipedia.org/wiki/BMP_file_format.

There are new functions that use Image structure and old functions that worked with files directly without
loading images to structures. Image structure is made with fields needed for these simple functions, and not
with all fields required to completely describe a bitmap file.
Most of the old functions (all except those with 'rgb' in their names) work for grayscale images only but
can easily be transformed for work with color images.
The new ones work with both grayscale and color images, precisely with 8-bit grayscale and 24-bit rgb color images.
For the sake of simplicity I didn't consider all possible header or pixel formats because the idea was to practice
these algorithms, so I did it with 8-bit and 24-bit depth images.