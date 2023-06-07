#include <stdio.h>
//#include <FreeImage.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <limits.h>
#define MAXURL_LEN 1000
/*#include <limits.h>
#undef PNG_Z_DEFAULT_COMPRESSION
#include <png.h>
#undef PNG_Z_DEFAULT_COMPRESSION
#include <jpeglib.h>*/


//*****************************************
void str_cat(char dest[], char source[]){
  int i,j;i=j=0;
  while(dest[i]!=0){
  i++;
  }
  while(source[j]!=0){
    dest[i]=source[j];
    i++;j++;
  }
  dest[i]=0;
}
int int_inp(){
  int n,num;
  while(((n=getchar())>'9')||(n<'0'));
  num = n-'0';
  while((n=getchar())&&(n<='9')&&(n>='0')){
      num = num*10+(n-'0');
      ;
    }
  return num;
}
int str_cmp(const char *a,const char *b){
  while(*a!=0 && *b!=0){
    if(*(a++) != *(b++))
      return 0;
    }
  if(*a==0 && *b==0)
    return 1;
  return 0;
}

int validation_check(const char *filetype,const char *valid_types[],int types){
  int i;
  for(i=0;i<types;i++){
    if(str_cmp(filetype,valid_types[i]))
      return 1;
  }
  return 0;
}
//***************************************************
//filters:
void applychapakiFilter(unsigned char *image, int width, int height,int bpp) {
  int bpps=bpp/8;
    for(int i=0;i<height;++i){
        for(int j=0;j<width/2;++j){
            int x=width-j-1;
            for(int r=0;r<bpps;++r){
                unsigned char temp=image[(j+i*width)*bpps+r];
                image[(j+i*width)*bpps+r]=image[(x+i*width)*bpps+r];
                image[(x+i*width)*bpps+r]=temp;
            }
        }
    }
}

void applykalepaFilter(unsigned char *image, int width, int height,int bpp){
    for(int i=0;i<height/2;++i){
        for(int j=0;j<width;++j){
            int x=height-i-1;
            for(int r=0;r<3;++r){
                unsigned char temp=image[(j+i*width)*3+r];
                image[(j+i*width)*3+r]=image[(j+x*width)*3+r];
                image[(j+x*width)*3+r]=temp;
            }
        }
    }
}
void convertToGray(unsigned char *image, int width, int height,int bpp) {
    // Iterate through each pixel
    float bpps = bpp/8;
    if(bpps == 0)bpps = 0.5;
    for (int i = 0; i < width * height; i++) {
        // Calculate grayscale value using the RGB components
        unsigned char r = image[(int)(bpps * i)];
        unsigned char g = image[(int)(bpps * i + 1)];
        unsigned char b = image[(int)(bpps * i + 2)];
        unsigned char gray = (unsigned char)((0.2989 * r) + (0.587 * g) + (0.114 * b));

        // Set the grayscale value for each RGB component of the pixel
        image[(int)(bpps * i)] = gray;
        image[(int)(bpps * i + 1)] = gray;
        image[(int)(bpps * i + 2)] = gray;
    }
}

double min1(double a,double b){
    if(a>b){
        return b;
    }
    else{
        return a;
    }
}

// Function to apply the sepia effect to an image
void converttoSepia(unsigned char *image, int width, int height,int bpp) {
    for (int i = 0; i < width * height; i++) {
        unsigned char red = image[3 * i];
        unsigned char green = image[3 * i + 1];
        unsigned char blue = image[3 * i + 2];

        unsigned char sRed = (unsigned char)min1((0.393 * red) + (0.769 * green) + (0.189 * blue),255);
        unsigned char sGreen = (unsigned char)min1((0.349 * red) + (0.686 * green) + (0.168 * blue),255);
        unsigned char sBlue = (unsigned char)min1((0.272 * red) + (0.534 * green) + (0.131 * blue),255);

        image[3 * i] = sRed;
        image[3 * i + 1] = sGreen;
        image[3 * i + 2] = sBlue;
    }
}

//*****************************************************************
//handling url
int find_last_slash(char *sample_url){
  int i=0;int index;
  while(sample_url[i]!=0){
    if(sample_url[i]=='/')
      index = i;
    i++;
  }
  return index;
}
int str_len(char *str){
  int i=0;while(str[i++]!=0);
  return i;
}
void str_cpy(char *str,char *new_str){
  while((*(new_str++) = *(str++))!=0);
}
char *downloaded_file_name(char *sample_url){
  int last_slash;last_slash = find_last_slash(sample_url);
  int len ;len = str_len(sample_url+last_slash);
  char *file_name = (char *)malloc(len*sizeof(char));
  str_cpy(sample_url+last_slash+1,file_name);
  return file_name;
}
//********************************************************************
//functions needed for bmp
int little_endian_hex_to_int(char* hex) {
    int result = 0;
    for (int i = 0; i < 4; i++) {
        result |= ((int)hex[i] & 0xFF) << (8*i);
    }
    return result;
}
int little_endian_hex_to_int_bpp(char* hex) {
    int result = 0;
    for (int i = 0; i < 2; i++) {
        result |= ((int)hex[i] & 0xFF) << (8*i);
    }
    return result;
}
//********************************************************************
int main(){
  char filename[FILENAME_MAX];
  printf("Please enter 1 to open image from system files\nenter 2 to open image using URL\nenter 3 to email your image\n");
  int operation=int_inp();
  if(operation==1){
    printf("Enter file name: ");
    char enterfilename[FILENAME_MAX];
    scanf("%s", enterfilename);
    str_cpy("/Users/raha/Desktop/",filename);
    str_cat(filename,enterfilename);
  }
  else if(operation==2){
    char url[MAXURL_LEN];
    printf("Enter Url : ");
    scanf("%s",url);
    char *downloaded_name;
    downloaded_name = downloaded_file_name(url);
    FILE *fp;
    //char outfilename[FILENAME_MAX] = "outfile.bin";
    char filename[FILENAME_MAX];
    str_cpy("/Users/raha/Desktop/",filename);
    str_cat(filename,downloaded_name);

    // Initializing easy handle
    CURL *curl_handle = curl_easy_init();
    if (curl_handle) {
        // Setting up URL to download from
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        // Open output file in binary write mode
        fp = fopen(filename,"wb");
        if (fp == NULL) {
            return 1;
        }
        // Setting write function to write data to output file
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, fwrite);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp);
        //setting up a res to store the downloaded file
        CURLcode res;
        res = curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        //performing the transfer (easy transfer wich only works once)
        res = curl_easy_perform(curl_handle);

        // Clean up
        fclose(fp);
        curl_easy_cleanup(curl_handle);

        if (res != CURLE_OK) {
            printf("Error downloading file: %s\n", curl_easy_strerror(res));
            return 1;
        }
    }
  }
  else{
    //nothing to do for now :(
  }
  //this part of code is to check if the given type is actually a valid image type
  const char* filetype;
  printf("%s",filename);
  int i=0;
  while(filename[i]!='.')i++;
  filetype = filename+i;
  const char *valid_types[]={".bmp" , ".jpeg", ".png"};
  if(!validation_check(filetype,valid_types,5)){
    printf("Error : Invalid file type");
    return 1;
  }

  //*****************************************************
  //opening file in binary mode
  printf("%s",filename);
  FILE *fp = fopen(filename, "rb");
  // check if the file was opened successfully
  if(fp == NULL){
    printf("Error opening file.\n");
    return 1;
  }
  //*************************************************
  printf("Please enter the number of the filter you want to apply on your picture :\n");
  printf("1 for gray filter\n2 for sepia filter\n3 for chapaki filter\n4 for kalepa filter\n");
  int filter=int_inp();
  //**************************************************
  if(str_cmp(filetype,".bmp")){
    char *bmp_header = ( char *)malloc(54);
    fread(bmp_header, 1, 54, fp);
    int width  = little_endian_hex_to_int(bmp_header+18);
    int height = little_endian_hex_to_int(bmp_header+22);
    int offset = little_endian_hex_to_int(bmp_header+10);
    int bpp = little_endian_hex_to_int_bpp(bmp_header+28);
    printf("%d",bpp);
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
    //performing the desired filter :
    if(filter ==1)
      convertToGray(bmp_image_info,width,height,bpp);
    else if(filter ==2)
      converttoSepia(bmp_image_info,width,height,bpp);
    else if(filter ==3)
      applychapakiFilter(bmp_image_info,width,height,bpp);
    else if(filter ==4)
      applykalepaFilter(bmp_image_info,width,height,bpp);
    //**************
    int i=0;
    while(filename[i++]!='.');
    filename[i-1]=0;
    printf("%s",filename);
    str_cat(filename,"_modified.bmp");
    fp = fopen(filename, "wb");
    if (fp == NULL) {
      printf("could not create output file\n");
      return 1;
    }
    fwrite(bmp_header, 1,offset , fp);
    fwrite(bmp_image_info, 1, file_size-offset, fp);
  }
  /*else if(str_cmp(filetype,".jpeg")){

  }*/
  // closing the file when done
   fclose(fp);
  return 0;
}
/*command prompt command for compiling jpeg :
gcc -o image_processing.out image_processing.c -I/System/Volumes/Data/usr/local/Cellar/jpeg-turbo/2.1.5.1/lib/libjpeg.dylib/jpeglib/include -L/usr/local/Cellar/jpeg-turbo/2.1.5.1 -ljpeg
*/
/*final command:
gcc -o image_processing.out image_processing.c -I/usr/local/opt/freeimage -L/usr/local/opt/libpng -I/System/Volumes/Data/usr/local/Cellar/jpeg-turbo/2.1.5.1/lib/libjpeg.dylib/jpeglib/include -L/usr/local/Cellar/jpeg-turbo/2.1.5.1 -lfreeimage -lpng -ljpeg
*/
