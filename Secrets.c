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

//least significant bit
#define lsb(byte)				( ((signed char)(byte)) & 0x1)

// shift left and insert a new least significant bit
#define shiftl(byte, newbit)    ( (byte) * 2 + (newbit) )

//shift right and insert a new most significant bit
#define shiftr(byte, newbit)	( (byte) / 2 + 128 * (newbit))

typedef unsigned char Byte;

/* FUNCTIONS String */
int fpeek(FILE *f)
{
    int c;
    if( (c = fgetc(f)) != EOF )
        ungetc(c, f); 
	return c;
}

/* FUNCTIONS Byte */
void encrypt_tool(Byte *bl, Byte *result, int *carries, int iteration){
	Byte b = bl[iteration], c;
	if(iteration == 6){
		c = shiftl(bl[7],0);
		for(int i = 0; i <= iteration; i++){
			b=shiftl(b,msb(c));
			c=shiftl(c,0);
			carries[i] = msb(b);
		}
	}else{
		encrypt_tool(bl, result, carries, iteration + 1);
		for(int i = 0; i <= iteration; i++){
			b=shiftl(b, carries[i]);
			carries[i] = msb(b);
		}
	}
	result[iteration] = b;
}

void encrypt_tool_end(int size, Byte *bl, Byte *result, 
						int *carries, int iteration){
	Byte b = bl[iteration];
	if(iteration == size)
		for (int i = 0; i <= iteration; i++){
			b = shiftl(b,0);
			carries[i] = msb(b);
		}
	else{
		encrypt_tool_end(size, bl, result, carries, iteration +1);
		for(int i = 0; i <= iteration; i++){
			b = shiftl(b, carries[i]);
			carries[i] = msb(b);
		}
	}
	result[iteration] = b;
}

void decrypt_tool(int size, Byte *bl, Byte *result, int *carries, int iteration){
	Byte b = bl[iteration];
	int c;
	if(iteration == 0){
		carries[iteration] = lsb(b);
		result[iteration] = shiftr(b,0);
	}else{
		decrypt_tool(size, bl, result, carries, iteration - 1);
		for(int i = 0; i < iteration; i++){
			c = lsb(b);
			b = shiftr(b, carries[i]);
			carries[i] = c;
		}
		carries[iteration] = lsb(b);
		b=shiftr(b,0);
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


void pack_encrypt(String input_filename, String encrypted_filename)
{
	FILE *i, *o;
	int count;
	char buffer[8], result[7];
	int carries[7];
	i=fopen(input_filename,"rb");
	o=fopen(encrypted_filename,"wb");
	if(i != NULL && o != NULL){
		while((count=fread(buffer,sizeof(char),8,i)) == 8){
			encrypt_tool(buffer,result, carries, 0);
			fwrite(result,sizeof(char),7,o);
		}
	encrypt_tool_end(count,buffer,result, carries, 0);
	fwrite(result,sizeof(char),count,o);
	fclose(i);
	fclose(o);
	}
}



void pack_decrypt(String encrypted_filename, String decrypted_filename)
{
	FILE *i, *o;
	int count;
	char buffer[7], result[8];
	int carries[7];
	i=fopen(encrypted_filename,"rb");
	o=fopen(decrypted_filename,"wb");
	if( i != NULL && o != NULL){
		while((count = fread(buffer,sizeof(char),7,i)) == 7){
			decrypt_tool(7, buffer, result, carries, 7);
			fwrite(result, sizeof(char), 8, o);
		}
		decrypt_tool(7, buffer, result, carries, count);
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
			if(b == '.' && fpeek(i) == ' ' && s != EOF){
				fputc(b,o);
				fgetc(i);
				//if the current secret char's 8th bit is 1
				printf("%d\n", msb(s));
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
		fclose(i);
		fclose(m);
		fclose(o);
		if ( s != EOF)
			error("Didn't fit\n", "Didn't fit\n");
	}
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
			if(b == EOF)
				b=img[i.x][i.y].green;
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
	fclose(f);
	if(b != EOF)
		error("Didn't fit\n","Didn't fit\n");
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
				if(b != 0x0)
					fputc(b,f);
				else
					done=true;
			}
		fclose(f);
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


