#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

//creating a struct to hold image information
//this struct wasnt used in the actual code
struct pic_info{
  int width;
  int height;
  int offset;
  int file_size;
  int image_size;
  unsigned short signature;
  int bits_per_pixel;
};
//this will take a 4 byte hexadecimal number that is little endian and will turn it to integer
int little_endian_hex_to_int(char* hex) {
    int result = 0;
    for (int i = 0; i < 4; i++) {
        result |= ((int)hex[i] & 0xFF) << (8*i);
    }
    return result;
}


int str_cmp(const char *a, const char *b){
  while (*a != 0 && *b != 0){
    if (*(a++) != *(b++))
      return 0;
  }
  if (*a == 0 && *b == 0)
    return 1;
  return 0;
}

int validation_check(const char *filetype, const char *valid_types[], int types){
  int i;
  for (i = 0; i < types; i++){
    if (str_cmp(filetype, valid_types[i]))
      return 1;
  }
  return 0;
}

void convertToGray(unsigned char *image, int width, int height) {
    // Iterate through each pixel
    for (int i = 0; i < width * height; i++) {
        // Calculate grayscale value using the RGB components
        unsigned char r = image[3 * i];
        unsigned char g = image[3 * i + 1];
        unsigned char b = image[3 * i + 2];
        unsigned char gray = (unsigned char)((0.2989 * r) + (0.587 * g) + (0.114 * b));

        // Set the grayscale value for each RGB component of the pixel
        image[3 * i] = gray;
        image[3 * i + 1] = gray;
        image[3 * i + 2] = gray;
    }
}

int main(){
  FILE *fp = fopen("/Users/raha/Desktop/modified_image.bmp", "rb");
  if (fp == NULL) {
    printf("could not open input file\n");
    return 1;
  }
  //using the information of the bit map header file to take information needed to add filters
  //taking offset since the image might not start from the 54th pixel
  //taking bpp inorder to handle files that are not 24bpp
  char *bmp_header = ( char *)malloc(54);
  fread(bmp_header, 1, 54, fp);
  int width  = little_endian_hex_to_int(bmp_header+18);
  int height = little_endian_hex_to_int(bmp_header+22);
  int offset = little_endian_hex_to_int(bmp_header+10);
  int bitsPerPixel = little_endian_hex_to_int(bmp_header+25);
  //printf("%d",bitsPerPixel);
  int file_size = little_endian_hex_to_int(bmp_header+2);
  //printf("\n%d  %d",file_size-offset,little_endian_hex_to_int(bmp_header+34));

  if(offset>54){
    bmp_header = ( char *)realloc(bmp_header,offset);
    fread(bmp_header+54, 1, offset-54, fp);
  }
  unsigned char *bmp_image_info = (unsigned char *)malloc(file_size-offset);
  if(bmp_image_info==NULL){
    printf("Failed to allocate memory!");
    return 1;
  }
  fread(bmp_image_info, 1,file_size-offset, fp);

  fclose(fp);

  convertToGray(bmp_image_info,width,height);

  fp = fopen("/Users/raha/Desktop/modified_image_out.bmp", "wb");
  if (fp == NULL) {
    printf("could not create output file\n");
    return 1;
  }

  fwrite(bmp_header, 1,offset , fp);
  fwrite(bmp_image_info, 1, file_size-offset, fp);

  fclose(fp);

  return 0;
}
