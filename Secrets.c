/*
max width = 80 colunas
tab = 4 spaces
01234567890123456789012345678901234567890123456789012345678901234567890123456789

LAP - AMD 2019
secrets.c

------
Student 1: 52890 - David Pereira
Student 2: 53675 - Pedro Bailao


*/

#include "Secrets.h"
#define BUFFER_SIZE 1024
// most significant bit
#define msb(byte)               ( ((signed char)(byte)) < 0 ? 1 : 0 )

//least significant bit
#define lsb(byte)				( ((signed char)(byte)) & 0x1)

// shift left and insert a new least significant bit
#define shiftl(byte, newbit)    ( (byte) * 2 + (newbit) )

//shift right and insert a new most significant bit
#define shiftr(byte, newbit)	( (byte) / 2 + 128 * (newbit))

//Color scheme for image_hide. Changing color order is easier!
#define RED 0
#define GREEN 1
#define BLUE 2

typedef unsigned char Byte;

/* FUNCTIONS String */

int cesar_encrypt_char(int c, int key){
	if(c >= 'A'&& c <= 'Z'){
		if(c + key > 'Z')
			c ='A'+ (key - ('Z' - c)) - 1;
		else
			c += key;
	}
	return c;
}

int cesar_decrypt_char(int c, int key){
	if(c >= 'A'&& c <= 'Z'){
		if(c - key < 'A')
			c ='Z'+ ((c - 'A') - key + 1);
		else
			c -= key;
	}
	return c;
}

int fpeek(FILE *f)
{
    int c;
    if( (c = fgetc(f)) != EOF )
        ungetc(c, f); 
	return c;
}

/* FUNCTIONS Byte */
void encrypt_tool(Byte *bl, Byte *result){
	Byte b,c;
	int carries[7];
	for(int iteration = 6; iteration >= 0; iteration--){
		b=bl[iteration];
		if(iteration == 6){
			c = shiftl(bl[7], 0);
			for(int i = 0; i<= iteration; i++){
				b=shiftl(b,msb(c));
				c=shiftl(c,0);
				carries[i] = msb(b);
			}
		}else
			for(int i = 0; i<= iteration; i++){
				b=shiftl(b, carries[i]);
				carries[i] = msb(b);
			}
		result[iteration] = b;
	}
}

void encrypt_tool_end(int size, Byte *bl, Byte *result){
	Byte b;
	int carries[size - 1];
	for(int iteration = size; iteration >= 0; iteration--){
		b=bl[iteration];
		if(iteration == size)
			for(int i = 0; i<= iteration; i++){
				b=shiftl(b,0);
				carries[i] = msb(b);
			}
		else
			for(int i = 0; i<= iteration; i++){
				b=shiftl(b, carries[i]);
				carries[i] = msb(b);
			}
		result[iteration] = b;
	}
}

void decrypt_tool(int size, Byte *bl, Byte *result){
	Byte b;
	int c;
	int carries[size - 1];
	for(int iteration = 0; iteration <= size; iteration++){
		b=bl[iteration];
		if(iteration != 0){
			for(int i = 0; i < iteration; i++){
				c = lsb(b);
				b = shiftr(b, carries[i]);
				carries[i] = c;
			}
		}
		carries[iteration] = lsb(b);
		b=shiftr(b, 0);
		result[iteration] = b;
	}
}

/* FUNCTIONS Int2 */


/* FUNCTIONS Pixel */


/* FUNCTIONS Image */


/* ENCRYPTION */

void copy_file(String input_filename, String output_filename)
{
	FILE *i, *o;
	char buffer[BUFFER_SIZE];
	int count;
	i=fopen(input_filename,"rb");
	o=fopen(output_filename,"wb");
	if(i != NULL && o != NULL){
		while( (count = fread(buffer, 1, BUFFER_SIZE, i)) > 0 ) {
        	fwrite(buffer, 1, count, o);
    	}
	}
	fclose(i);
	fclose(o);
}

void cesar_encrypt(String input_filename, int key, String encrypted_filename)
{
	FILE *i, *o;
	int b;
	i=fopen(input_filename,"rb");
	o=fopen(encrypted_filename,"wb");
	if(i != NULL && o != NULL)
		while ((b=fgetc(i)) != EOF)
			fputc(cesar_encrypt_char(b,key),o);
	fclose(i);
	fclose(o);
}

void cesar_decrypt(String encrypted_filename, int key,
										String decrypted_filename)
{
	FILE *i, *o;
	int b;
	i=fopen(encrypted_filename,"rb");
	o=fopen(decrypted_filename,"wb");
	if(i != NULL && o != NULL){
		while ((b=fgetc(i)) != EOF)
			fputc(cesar_decrypt_char(b,key),o);
	}
	fclose(i);
	fclose(o);	
}

void pi_encrypt(String input_filename, String pi_filename,
										String encrypted_filename)
{
	FILE *i, *o, *pi;
	int b,pic;
	i=fopen(input_filename,"rb");
	o=fopen(encrypted_filename,"wb");
	pi=fopen(pi_filename,"rb");
	if(i != NULL && o != NULL && pi != NULL){
		fseek(pi,0x2,SEEK_SET);
		while ((b=fgetc(i)) != EOF){
			if(b >= 'A' && b <= 'Z'){
				pic=fgetc(pi);
				if(pic == EOF){
					fseek(pi,0x2,SEEK_SET);
					pic=fgetc(pi);
				}
				b=cesar_encrypt_char(b,pic - '0');
			}
			fputc(b,o);
		}
	}
	fclose(i);
	fclose(o);
	fclose(pi);
}

void pi_decrypt(String encrypted_filename, String pi_filename,
											String decrypted_filename)
{
	FILE *i, *o, *pi;
	int b,pic;
	i=fopen(encrypted_filename,"rb");
	o=fopen(decrypted_filename,"wb");
	pi=fopen(pi_filename,"rb");
	if(i != NULL && o != NULL && pi != NULL){
		fseek(pi,0x2,SEEK_SET);
		while ((b=fgetc(i)) != EOF){
			if(b >= 'A' && b <= 'Z'){
				pic=fgetc(pi);
				if(pic == EOF){
					fseek(pi,0x2,SEEK_SET);
					pic=fgetc(pi);
				}
				b=cesar_decrypt_char(b,pic - '0');
			}
			fputc(b,o);
		}
	}
	fclose(i);
	fclose(o);
	fclose(pi);
}


void pack_encrypt(String input_filename, String encrypted_filename)
{
	FILE *i, *o;
	int count;
	Byte buffer[8], result[7];
	i=fopen(input_filename,"rb");
	o=fopen(encrypted_filename,"wb");
	if(i != NULL && o != NULL){
		while((count=fread(buffer,sizeof(char),8,i)) == 8){
			encrypt_tool(buffer,result);
			fwrite(result,sizeof(char),7,o);
		}
	encrypt_tool_end(count,buffer,result);
	fwrite(result,sizeof(char),count,o);
	}
	fclose(i);
	fclose(o);
}

void pack_decrypt(String encrypted_filename, String decrypted_filename)
{
	FILE *i, *o;
	int count;
	Byte buffer[7], result[8];
	i=fopen(encrypted_filename,"rb");
	o=fopen(decrypted_filename,"wb");
	if( i != NULL && o != NULL){
		while((count = fread(buffer,sizeof(char),7,i)) == 7){
			decrypt_tool(7, buffer, result);
			fwrite(result, sizeof(char), 8, o);
		}
		decrypt_tool(count, buffer, result);
		fwrite(result, sizeof(char), count, o);
	}
	fclose(i);
	fclose(o);
}


/* STEGANOGRAPHY */

void dots_hide(String input_filename,
				String message_filename, String disguised_filename)
{
	FILE *i, *m, *o;
	int b, s, count;
	bool lastByte = false;
	i=fopen(input_filename, "rb");
	m=fopen(message_filename, "rb");
	o=fopen(disguised_filename, "wb");
	if( i != NULL && m != NULL && o != NULL){
		s=fgetc(m);
		count=0;
		while((b=fgetc(i)) != EOF){
			//if it finds a full stop and the secret message is not over
			if(s != EOF && b == '.' && fpeek(i) == ' '){
				fputc(b,o);
				fgetc(i);
				//if the current secret char's 8th bit is 1
				if(msb(s) != 0)
					fputc(' ',o);
				s=shiftl(s,0);
				count++;
				//if the current secret char's bits have all been written
				if(count == 8){
					s=fgetc(m);
					count=0;
					//If EOF is found, we need to write the 0 byte
					if(s == EOF && !lastByte){
						lastByte = true;
						s=0x0;
					}

				}
				fputc(' ',o);
				//Eliminates all the next space chars
				while(fpeek(i) == ' ')
					fgetc(i);
			//If it's not a full stop
			}else
				fputc(b,o);
		}
		if ( s != EOF)
			error("Didn't fit\n", message_filename);
	}
	fclose(i);
	fclose(m);
	fclose(o);
}

void dots_reveal(String disguised_filename, String decoded_filename)
{	
	FILE *i, *o;
	int b, c = 0x0, count = 0;
	i=fopen(disguised_filename, "rb");
	o=fopen(decoded_filename, "wb");
	if( i != NULL && o != NULL){
		while((b = fgetc(i)) != EOF){
			if(b == '.' && fpeek(i) == ' '){
				fgetc(i);
				if(fpeek(i) == ' ')
					c=shiftl(c, 1);
				else
					c=shiftl(c, 0);
				count++;
				if(count == 8){
					if(c == 0x0)
						break;
					fputc(c, o);
					c = 0x0;
					count=0;
				}
			}
		}
	}
	fclose(i);
	fclose(o);
}

Int2 crude_hide(Image img, Int2 n,
					String message_filename, Image result)
{
	FILE *f;
	Int2 i;
	bool done=false;
	int b;
	f=fopen(message_filename,"rb");
	if(f == NULL)
		return n;
	for(i.y = 0 ; i.y < n.y; i.y++)
		for(i.x = 0 ; i.x < n.x; i.x++){
			b = fgetc(f);
			if(!done && b == EOF){
				result[i.x][i.y].green=0x0;
				result[i.x][i.y].blue=img[0][i.y].blue;
				result[i.x][i.y].red=img[0][i.y].red;
				done=true;
			}else{
				result[i.x][i.y].green = img[i.x][i.y].green;
				result[i.x][i.y].red=img[i.x][i.y].red;
				result[i.x][i.y].blue=img[i.x][i.y].blue;
			}
	}
	fclose(f);
	if(b != EOF)
		error("Didn't fit\n", message_filename);
	return n;
}



void crude_reveal(Image img, Int2 n, String decoded_filename)
{
	FILE *f;
	Int2 i;
	char b;
	bool done=false;
	if ((f=fopen(decoded_filename,"wb")) != NULL){
		for(i.y = 0 ; i.y < n.y && !done; i.y++)
			for(i.x = 0 ; i.x < n.x && !done; i.x++){
				b=img[i.x][i.y].green;
				//if the green pixels value is not 0, decode it
				if(b != 0x0)
					fputc(b,f);
				//if its 0, means the message is over
				else
					done=true;
			}
		fclose(f);
	}
}


Int2 image_hide(Image img, Int2 n,
					String message_filename, Image result)
{
	FILE *f;
	Int2 i;
	int  b, count = 0;
	bool finalByte=false;
	f=fopen(message_filename, "rb");
	if( f != NULL){
		b=fgetc(f);
		for(i.y = 0; i.y < n.y; i.y++)
			for(i.x = 0; i.x < n.x; i.x++){
				//If f still has chars to read
				if( b != EOF){
					//loop for changing the pixel's color' bit
					for(int color = 0; color < 3; color++){
						switch (color){
							case RED:
								result[i.x][i.y].red = 
								(img[i.x][i.y].red & 0xFE) + msb(b);
								break;
							case GREEN:
								result[i.x][i.y].green = 
								(img[i.x][i.y].green & 0xFE) + msb(b);
								break;
							case BLUE:
								result[i.x][i.y].blue = 
								(img[i.x][i.y].blue & 0xFE) + msb(b);
								break;
							default: break;
						}
						b=shiftl(b, 0);
						count++;
						//if a full byte has already been encrypted
						if( count % 8 == 0){
							b = fgetc(f);
							if(b == EOF){
								//if b wasn't the final byte, it is now
								if(!finalByte){
									b = 0x0;
									finalByte = true;
								}else{ /* if b was the final byte, then
									completes the color assignment cycle
									by keeping the remainder of the pixel's
									colors*/
									for(int colour = color + 1; colour < 3; 
										colour++){
										switch (colour){
											case RED:
												result[i.x][i.y].red = 
												img[i.x][i.y].red;
												break;
											case GREEN:
												result[i.x][i.y].green = 
												img[i.x][i.y].green;
												break;
											case BLUE:
												result[i.x][i.y].blue = 
												img[i.x][i.y].blue;
												break;
											default: break;
										}
									}
									break;
								}
							}
						}
					}	
				//if we're done encrypting, just keep the pixel's colors
				}else{
					/*this is for the case where we receive an empty file
					in that case, we must just need to hide a 0.
					Only finalbyte is check because if we reach this part
					we know that b is EOF */
					if(!finalByte){
						b = 0x0;
						finalByte = true;
						for(int color = 0; color < 3; color++){
							switch (color){
								case RED:
									result[i.x][i.y].red = 
									(img[i.x][i.y].red & 0xFE) + msb(b);
									break;
								case GREEN:
									result[i.x][i.y].green = 
									(img[i.x][i.y].green & 0xFE) + msb(b);
									break;
								case BLUE:
									result[i.x][i.y].blue = 
									(img[i.x][i.y].blue & 0xFE) + msb(b);
									break;
								default: break;
							}
							//No need to shift b because its just 8 0-bits
							count++;
						}
					}else{
						result[i.x][i.y].red = img[i.x][i.y].red;
						result[i.x][i.y].green = img[i.x][i.y].green;
						result[i.x][i.y].blue = img[i.x][i.y].blue;
					}
				}
			}
		if( b != EOF)
			error("Didn't fit\n", message_filename);
	}
	fclose(f);
	return n;
}

void image_reveal(Image img, Int2 n, String decoded_filename)
{
	FILE *f;
	Int2 i;
	char b = 0x0;
	int count = 0;
	bool done = false;
	f=fopen(decoded_filename, "wb");
	if(f != NULL){
		for(i.y = 0; i.y < n.y && !done; i.y++)
			for(i.x = 0; i.x < n.x && !done; i.x++)
				//loop for verifying each pixel 3 times: 1 for each color
				for(int color = 0; color < 3; color++){
					switch (color){/*for each color it shift b to the left
									inserting the pixel's color's least
									significant bit on the right*/
						case RED:
								b=shiftl(b, lsb(img[i.x][i.y].red));
								break;
							case GREEN:
								b=shiftl(b, lsb(img[i.x][i.y].green));
								break;
							case BLUE:
								b=shiftl(b, lsb(img[i.x][i.y].blue));
								break;
							default: break;
					}
					count++;
					//if we've got a full byte decoded
					if( count % 8 == 0){
						if( b == 0x0)
							done=true;
						else{
							fputc(b, f);
							b=0x0;
						}
					}
				}
	}
	fclose(f);
}


