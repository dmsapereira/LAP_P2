/*
max width = 80 colunas
tab = 4 spaces
01234567890123456789012345678901234567890123456789012345678901234567890123456789

LAP - AMD 2019
secrets.c

------
Student 1: ????? mandatory to fill
Student 2: ????? mandatory to fill

Comment:

?????????????????????????
?????????????????????????
?????????????????????????
?????????????????????????
?????????????????????????
?????????????????????????

*/

#include "Secrets.h"
#define BUFFER_SIZE 1024
// most significant bit
#define msb(byte)               ( ((signed char)(byte)) < 0 ? 1 : 0 )
// shift left and insert a new least significant bit
#define shiftl(byte, newbit)    ( (byte) * 2 + (newbit) )
typedef unsigned char Byte;

/* FUNCTIONS String */


/* FUNCTIONS Byte */


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
}

int cesar_encrypt_char(int c, int key){
	if(c >= 'A'&& c <= 'Z'){
		if(c + key > 'Z')
			c ='A'+ (key - ('Z' - c)) - 1;
		else
			c += key;
	}
	return c;
}

void cesar_encrypt(String input_filename, int key, String encrypted_filename)
{
	FILE *i, *o;
	int b;
	i=fopen(input_filename,"rb");
	o=fopen(encrypted_filename,"wb");
	if(i != NULL && o != NULL){
		while ((b=fgetc(i)) != EOF)
			fputc(cesar_encrypt_char(b,key),o);
		fclose(i);
		fclose(o);
	}	
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

void cesar_decrypt(String encrypted_filename, int key,
										String decrypted_filename)
{
	FILE *i, *o;
	int b;
	i=fopen(encrypted_filename,"rb");
	o=fopen(decrypted_filename,"wb");
	if(i != NULL && o != NULL){
		while ((b=fgetc(i)) != EOF){
			fputc(cesar_decrypt_char(b,key),o);
		}
		fclose(i);
		fclose(o);
	}	
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
		fclose(i);
		fclose(o);
		fclose(pi);
	}
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
		fclose(i);
		fclose(o);
		fclose(pi);
	}
}

Byte pack_tool8(Byte bl[8], Byte result[7], int iteration){
	Byte b,c;
	if(iteration == 6){
		b=bl[6] & 0x1;
		c=shiftl(bl[7],0);
		for(int i = 0; i< 7; i++){
			b=shiftl(b,msb(c));
			c=shiftl(c,0);
		}
	}else{
		b=bl[iteration];
		b=shiftl(b,msb(pack_tool8(bl,result,iteration+1)));
		result[iteration] = b;
	}
	return b;
}

Byte pack_tool_end(int size, Byte bl[size], Byte result[size], int iteration){
	Byte b;
	if(iteration == size){
		printf("Iteration on size %d\n", size);
		b=bl[iteration];
		for (int i = 0; i < iteration; i++)
			b=shiftl(b,0);
	}else{
		printf("iteration %d\n",iteration);
		b=bl[iteration];
		b=shiftl(b,msb(pack_tool_end(size,bl,result,iteration+1)));
		result[iteration]=b;
	}
	return b;
}

void pack_encrypt(String input_filename, String encrypted_filename)
{
	FILE *i, *o;
	int b,count;
	char buffer[BUFFER_SIZE];
	char result[7];
	i=fopen(input_filename,"rb");
	o=fopen(encrypted_filename,"wb");
	if(i != NULL && o != NULL){
		while((count=fread(buffer,sizeof(char),8,i)) == 8){
			pack_tool8(buffer,result,0);
			fwrite(result,sizeof(char),7,o);
		}
	pack_tool_end(count,buffer,result,0);
	fwrite(result,sizeof(char),count,o);
	fclose(i);
	fclose(o);
	}
}

void pack_decrypt(String encrypted_filename, String decrypted_filename)
{
}


/* STEGANOGRAPHY */

void dots_hide(String input_filename,
				String message_filename, String disguised_filename)
{
}

void dots_reveal(String disguised_filename, String decoded_filename)
{	
}

Int2 crude_hide(Image img, Int2 n,
					String message_filename, Image result)
{
	FILE *f;
	Int2 i;
	f=fopen(message_filename,"rb");
	if(f == NULL)
		return n;
	int b=fgetc(f);
	for(i.y = 0 ; i.y < n.y; i.y++)
		for(i.x = 0 ; i.x < n.x; i.x++){
			if(b == EOF){
				result[i.x][i.y].green=img[i.x][i.y].green;
				result[i.x][i.y].red=img[i.x][i.y].red;
				result[i.x][i.y].blue=img[i.x][i.y].blue;
			}else{
				result[i.x][i.y].green=b;
				result[i.x][i.y].red=img[i.x][i.y].red;
				result[i.x][i.y].blue=img[i.x][i.y].blue;
				b=fgetc(f);
				if(b == EOF){
					if(i.x+1 == n.x){
						i.y++;
						result[0][i.y].green=0x0;
						result[0][i.y].blue=img[0][i.y].blue;
						result[0][i.y].red=img[0][i.y].red;
					}else{
						i.x++;
						result[i.x][i.y].green=0x0;
						result[i.x][i.y].blue=img[0][i.y].blue;
						result[i.x][i.y].red=img[0][i.y].red;
				}
			}
		}
	}
	if(b != EOF)
		error("Didn't fit\n","Didn't fit\n");
	return n;
}

void crude_reveal(Image img, Int2 n, String decoded_filename)
{
	FILE *f;
	Int2 i;
	char b;
	if ((f=fopen(decoded_filename,"wb")) != NULL){
		b=img[0][0].green;
		for(i.y = 0 ; i.y < n.y && b!=0x0; i.y++)
			for(i.x = 0 ; i.x < n.x && b!=0x0; i.x++){
				fputc(b,f);
				b=img[i.x][i.y].green;
			}
	}
}


Int2 image_hide(Image img, Int2 n,
					String message_filename, Image result)
{
	return n;
}

void image_reveal(Image img, Int2 n, String decoded_filename)
{
}


