#ifndef SECRETS_H_
#define SECRETS_H_

/*
max width = 80 colunas
tab = 4 spaces
01234567890123456789012345678901234567890123456789012345678901234567890123456789

LAP (AMD 2019)
secrets.h
Do not change this file.
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "lodepng.h"


/* TYPE String */

#define MAX_STRING		256

typedef char String[MAX_STRING];


/* TYPE Byte */

typedef unsigned char Byte;


/* TYPE Int2 - two-dimensional integers, matrix indices */

typedef struct {
	int x, y;
} Int2;

extern Int2 int2_zero, int2_erro;

extern Int2 int2(int x, int y);
extern bool int2_equal(Int2 a, Int2 b);
extern bool int2_less(Int2 a, Int2 b);
extern double int2_distance(Int2 a, Int2 b);
extern bool int2_is_error(Int2 a);


/* TYPE Pixel */

#define MAX_COLOR			255

typedef struct {
	Byte red, green, blue;
} Pixel;

extern Pixel white, red, green, gray, blue, black;

extern Pixel pixel(int red, int green, int blue);
extern bool pixel_equal(Pixel a, Pixel b);
extern Pixel pixel_turn_gray(Pixel p);


/* TYPE Image */

#define MAX_X			1024
#define MAX_Y			1024

typedef Pixel Image[MAX_X][MAX_Y];

Int2 image_load(String filename, Image result);
bool image_store(String filename, Image img, Int2 n);
Int2 image_copy(Image img, Int2 n, Image result);
Int2 image_turn_gray(Image img, Int2 n, Image result);
Int2 image_mirror(Image img, Int2 n, Image result);


/* MISC */

extern bool error(String err, String arg);


/* ENCRYPTION */

extern void copy_file(String input_filename, String output_filename);

#define CESAR_MAX	26
extern void cesar_encrypt(String input_filename, int key,
										String encrypted_filename);
extern void cesar_decrypt(String encrypted_filename, int key,
										String decrypted_filename);

#define PY_FILENAME "pi.txt"  // https://www.angio.net/pi/digits/100000.txt
extern void pi_encrypt(String input_filename, String pi_filename,
										String encrypted_filename);
extern void pi_decrypt(String encrypted_filename, String pi_filename,
										String decrypted_filename);

extern void pack_encrypt(String input_filename, String encrypted_filename);
extern void pack_decrypt(String encrypted_filename, String decrypted_filename);


/* STEGANOGRAPHY */

extern void dots_hide(String container_filename, String message_filename,
													String disguised_filename);
extern void dots_reveal(String disguised_filename, String decoded_filename);

extern Int2 crude_hide(Image img, Int2 n, String message_filename,
													Image result);
extern void crude_reveal(Image img, Int2 n, String decoded_filename);

extern Int2 image_hide(Image img, Int2 n, String message_filename,
													Image result);
extern void image_reveal(Image img, Int2 n, String decoded_filename);


#endif

