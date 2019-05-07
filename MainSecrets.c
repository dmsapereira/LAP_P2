/*
max width = 80 colunas
tab = 4 spaces
01234567890123456789012345678901234567890123456789012345678901234567890123456789

LAP - AMD 2019
main_secrets.c
Do not change this file.
*/

#include "Secrets.h"


/* FUNCTIONS Int2 */

Int2 int2_zero = {0, 0};
Int2 int2_error = {-1, -1};

Int2 int2(int x, int y)
{
	Int2 t = {x, y};
	return t;
}

bool int2_equal(Int2 a, Int2 b)
{
	return a.x == b.x && a.y == b.y;
}

bool int2_less(Int2 a, Int2 b)
{
	return a.x < b.x && a.y < b.y;
}

double int2_distance(Int2 a, Int2 b)
{
	double x_dist = abs(a.x - b.x);
	double y_dist = abs(a.y - b.y);
	return sqrt(x_dist * x_dist + y_dist * y_dist);;
}

bool int2_is_error(Int2 i)
{
	return int2_equal(i, int2_error);
}


/* FUNCTIONS Pixel */

Pixel white = {255, 255, 255};
Pixel red = {255, 0, 0};
Pixel green = {0, 255, 0};
Pixel grey = {127, 127, 127};
Pixel blue = {0, 0, 255};
Pixel black = {0, 0, 0};

static int color_normalize(int color) /* force color into the 0..MAX_COLOR range */
{
	if( color < 0 )
		return 0;
	else if( color > MAX_COLOR )
		return MAX_COLOR;
	else
		return color;
}

Pixel pixel(int red, int green, int blue) /* pixel constructor */
{
	Pixel p = {
			color_normalize(red),
			color_normalize(green),
			color_normalize(blue)
	};
	return p;
}

bool pixel_equal(Pixel a, Pixel b) /* are the two pixels equal? */
{
	return a.red == b.red && a.green == b.green && a.blue == b.blue;
}

Pixel pixel_turn_gray(Pixel p) /* example on how to process a pixel */
{
	int grey_level = p.red + p.green + p.blue/3;
	return pixel(grey_level, grey_level, grey_level);
}


/* FUNCTIONS Image */

Int2 image_load(String filename, Image result)
{  /* too technical... do not try to understand */
	Byte *mem;
	unsigned w, h;
	int erro = lodepng_decode24_file(&mem, &w, &h, filename);
	if( erro != 0 )
		return int2_error;
	if( w == 0 || h == 0 || w > MAX_X || h > MAX_Y ) {
		free(mem);
		return int2_error;
	}
	Int2 n = int2(w, h);
	Pixel *p = (Pixel *)mem;
	if( erro != 0 )
		return int2_error;
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		result[i.x][i.y] = *p++;
	}
	free(mem);
	return n;
}

bool image_store(String filename, Image img, Int2 n)
{ /* too technical... do not try to understand */
	Int2 i;
	Byte *mem = malloc(MAX_X * MAX_Y * sizeof(Pixel));
	Pixel *p = (Pixel *)mem;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		*p++ = img[i.x][i.y];
	}
	if( lodepng_encode24_file(filename, mem, n.x, n.y) != 0 )	
		return false;
	free(mem);
	return true;
}

Int2 image_copy(Image img, Int2 n, Image result)
{
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		result[i.x][i.y] = img[i.x][i.y];
	}
	return n;
}

Int2 image_turn_gray(Image img, Int2 n, Image result)
{ /* example on how to process images */
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		result[i.x][i.y] = pixel_turn_gray(img[i.x][i.y]);
	}
	return n;
}	 

Int2 image_mirror(Image img, Int2 n, Image result)
{ /* example on how to process images */
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		result[n.x - 1 - i.x][i.y] = img[i.x][i.y];
	}
	return n;
}



/* MISC */

bool error(String err, String arg)
{
	String mesg;
	sprintf(mesg, "ERROR: %s!\n", err);
	printf(mesg, arg);
	exit(1);
}


/* ENCRYPTION */

static void command_copy(String input_filename, String output_filename)
{
	copy_file(input_filename, output_filename);
}

static void command_cesar_encrypt(String input_filename, String key,
											String encrypted_filename)
{
	int k;
	if( sscanf(key, "%d", &k) != 1 || k < 0 || k >= CESAR_MAX )
		error("Invalid key '%s'", key);
	cesar_encrypt(input_filename, k, encrypted_filename);
}

static void command_cesar_decrypt(String encrypted_filename, String key,
											String decrypted_filename)
{
	int k;
	if( sscanf(key, "%d", &k) != 1 || k < 0 || k >= CESAR_MAX )
		error("Invalid key '%s'", key);
	cesar_decrypt(encrypted_filename, k, decrypted_filename);
}

void command_pi_encrypt(String input_filename, String encrypted_filename)
{
	pi_encrypt(input_filename, PY_FILENAME, encrypted_filename);
}

void command_pi_decrypt(String encrypted_filename, String decrypted_filename)
{
	pi_decrypt(encrypted_filename, PY_FILENAME, decrypted_filename);
}

void command_pack_encrypt(String input_filename, String encrypted_filename)
{
	pack_encrypt(input_filename, encrypted_filename);
}

void command_pack_decrypt(String encrypted_filename, String decrypted_filename)
{
	pack_decrypt(encrypted_filename, decrypted_filename);
}


/* STEGANOGRAPHY */

static void command_dots_hide(String input_filename,
				String message_filename, String disguised_filename)
{
	dots_hide(input_filename, message_filename, disguised_filename);
}

static void command_dots_reveal(String disguised_filename,
											String decoded_filename)
{
	dots_reveal(disguised_filename, decoded_filename);
}

static void command_crude_hide(String input_filename,
							String message_filename, String disguised_filename)
{
	static Image ia, ib;
	Int2 na = image_load(input_filename, ia), nb;
	if( int2_is_error(na) )
         error("File not found or invalid file '%s'", input_filename);
	nb = crude_hide(ia, na, message_filename, ib);
	if( !image_store(disguised_filename, ib, nb) )
		error("Could not write file '%s'", disguised_filename);
}

static void command_crude_reveal(String disguised_filename,
										String decoded_filename)
{
	static Image ia;
	Int2 na = image_load(disguised_filename, ia);
	if( int2_is_error(na) )
         error("File not found or invalid file '%s'", disguised_filename);
	crude_reveal(ia, na, decoded_filename);
}

static void command_image_hide(String input_filename,
							String message_filename, String disguised_filename)
{
	static Image ia, ib;
	Int2 na = image_load(input_filename, ia), nb;
	if( int2_is_error(na) )
         error("File not found or invalid file '%s'", input_filename);
	nb = image_hide(ia, na, message_filename, ib);
	if( !image_store(disguised_filename, ib, nb) )
		error("Could not write file '%s'", disguised_filename);
}

static void command_image_reveal(String disguised_filename,
										String decoded_filename)
{
	static Image ia;
	Int2 na = image_load(disguised_filename, ia);
	if( int2_is_error(na) )
         error("File not found or invalid file '%s'", disguised_filename);
	image_reveal(ia, na, decoded_filename);
}

static void run_command(String command, String arg1, String arg2, String arg3)
{
	if( strcmp(command, "copy") == 0 )
		command_copy(arg1, arg2);

	else if( strcmp(command, "cesar+") == 0 )
		command_cesar_encrypt(arg1, arg2, arg3);

	else if( strcmp(command, "cesar-") == 0 )
		command_cesar_decrypt(arg1, arg2, arg3);

	else if( strcmp(command, "pi+") == 0 )
		command_pi_encrypt(arg1, arg2);

	else if( strcmp(command, "pi-") == 0 )
		command_pi_decrypt(arg1, arg2);

	else if( strcmp(command, "pack+") == 0 )
		command_pack_encrypt(arg1, arg2);

	else if( strcmp(command, "pack-") == 0 )
		command_pack_decrypt(arg1, arg2);

	else if( strcmp(command, "dots+") == 0 )
		command_dots_hide(arg1, arg2, arg3);

	else if( strcmp(command, "dots-") == 0 )
		command_dots_reveal(arg1, arg2);

	else if( strcmp(command, "crude+") == 0 )
		command_crude_hide(arg1, arg2, arg3);

	else if( strcmp(command, "crude-") == 0 )
		command_crude_reveal(arg1, arg2);

	else if( strcmp(command, "image+") == 0 )
		command_image_hide(arg1, arg2, arg3);

	else if( strcmp(command, "image-") == 0 )
		command_image_reveal(arg1, arg2);

	else
		error("Unknown command '%s'", command);
}

/*
gcc -o project2 MainSecrets.c Secrets.c lodepng.c -lm


project2 copy fruits.png fruits_copy.png

project2 cesar+ plain_message.txt 3 secret_message.txt
project2 cesar- secret_message.txt 3 decoded_message.txt

project2 pi+ plain_message.txt secret_message.txt
project2 pi- secret_message.txt decoded_message.txt

project2 pack+ plain_message.txt secret_message.txt
project2 pack- secret_message.txt decoded_message.txt

project2 dots+ container_text.txt plain_message.txt text_disguised.txt
project2 dots- text_disguised.txt extracted_message.txt

project2 crude+ fruits.png plain_message.txt fruits_disguised.png
project2 crude- fruits_disguised.png extracted_message.txt

project2 image+ fruits.png plain_message.txt fruits_disguised.png
project2 image- fruits_disguised.png extracted_message.txt
*/



/* MAIN */

int main(int argc, char *argv[])
{
	String command = "", arg1 = "", arg2 = "", arg3 = "";
	if( argc > 1 ) strcpy(command, argv[1]);
	if( argc > 2 ) strcpy(arg1, argv[2]);
	if( argc > 3 ) strcpy(arg2, argv[3]);
	if( argc > 4 ) strcpy(arg3, argv[4]);
	run_command(command, arg1, arg2, arg3);
	return 0;
}
