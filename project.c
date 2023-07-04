#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <limits.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb-master/stb_image_write.h"
//the only function i used the string library for is strstr
#include "/usr/local/Cellar/libb64/1.2.1_1/include/b64/cdecode.h"
#define MAXURL_LEN 1000


//*****************************************
//string & Input functions :

int int_inp(){
  int n,num;
  while(((n=getchar())>'9')||(n<'0'));
  num = n-'0';
  while((n=getchar())&&(n<='9')&&(n>='0')){
      num = num*10+(n-'0');
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
int str_len(char *str){
  int i=0;while(str[i++]!=0);
  return i;
}
void str_cpy(char *str,char *new_str){
  while((*(new_str++) = *(str++))!=0);
}
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
//make strstr work with pointers and give back the pointer to the starting index
//or a nULL pointer if it isnt a substr

/*int str_str(char str[], char substr[]){
  int i=0;
  int j,flag;
  int start_index;
  while(str[i]!=0){
    if(str[i]==substr[0]){
      start_index =i;
      j=flag=0;
      while(substr[j]!=0){
        if(str[i]!=substr[j]){
          flag =1;
          break;
        }
        i++;j++;
      }
      if(flag==0&&substr[j]==0)
        return start_index;
    }
    i++;
  }
  return -1;
}*/




//***************************************************
//filters:
void threshold(unsigned char* image, int width, int height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int pixelIndex = (width* i + j) * 3;
            double avrage = (image[pixelIndex] + image[pixelIndex + 1] + image[pixelIndex + 2]) / 3.0;
            if (125 < avrage) {
                for (int k = 0; k < 3; ++k) {
                    image[pixelIndex+k]=255;
                }
            }
            else{
                for (int k = 0; k < 3; ++k) {
                    image[pixelIndex+k]=0;
                }
            }
        }
    }
}

void nesf(unsigned char *image, int width, int height){
    int k=0;
    for (int i=0;i<height;++i){
        for(int j=0;j<width;++j){
            if(j<=width/2){
                k=j;
            }
            else{
                --k;
            }
            for(int r=0;r<3;++r){
                image[(i*width+j)*3+r]=image[(i*width+k)*3+r];
            }
        }
    }
}

/*void hashoor(unsigned char *image, int width, int height){

}*/

double min1(double a,double b){
    if(a>b){
        return b;
    }
    else{
        return a;
    }
}

// Function to apply the sepia effect to an image
void converttoSepia(unsigned char *image, int width, int height) {
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

// Function to apply a mirror filter to an image
void applychapakiFilter(unsigned char *image, int width, int height,int bpp) {
    int bpps=bpp/8;
    for(int i=0;i<height;++i){
        for(int j=0;j<width/2;++j){
            int x=width-j-1;
            for(int r=0;r<bpps;++r){
                unsigned char temp=image[(j+i*width)*bpps+r];
                image[(j+i*width)*bpps+r]=image[(x+i*width)*3+r];
                image[(x+i*width)*3+r]=temp;
            }
        }
    }
}

void saabet(unsigned char *image, int width, int height){
    int k=0;
    for(int i=0;i<height;++i){
        if(i<=height/2){
            k=i;
        }
        for(int j=0;j<width;++j){
            image[(i*width+j)*3]=image[(k*width+j)*3];
            image[(i*width+j)*3+1]=image[(k*width+j)*3+1];
            image[(i*width+j)*3+2]=image[(k*width+j)*3+2];
        }
    }
}

void saabet2(unsigned char *image, int width, int height){
    unsigned char* image2= malloc(height*width*3);
    int k=0;
    for(int i=0;i<height;++i){
        k=0;
        for(int j=0;j<width;++j){
            if(j<=width/2){
                k=j;
            }
            image2[(i*width+j)*3]=image[(i*width+k)*3];
            image2[(i*width+j)*3+1]=image[(i*width+k)*3+1];
            image2[(i*width+j)*3+2]=image[(i*width+k)*3+2];
        }
    }
    for(int t=0;t<height*width*3;++t){
        image[t]=image2[t];
    }
}

void badquality(unsigned char *image, int width, int height){
    int step=255/5;
    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j){
            for(int r=0;r<3;++r){
                image[(i*width+j)*3+r]=(image[(i*width+j)*3+r]/step)*step;
            }
        }
    }
}

void morecolorful(unsigned char *image, int width, int height){
    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j){
            for(int r=0;r<3;++r){
                image[(i*width+j)*3+r]=0.5*image[(i*width+j)*3+r];
            }
        }
    }
}

void purpleimage(unsigned char *image, int width, int height){
    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j){
            image[(i*width+j)*3]=0.5*image[(i*width+j)*3];
            image[(i*width+j)*3+1]=0*image[(i*width+j)*3+1];
            image[(i*width+j)*3+2]=0.5*image[(i*width+j)*3+2];
        }
    }
}

void greenimage(unsigned char *image, int width, int height){
    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j){
            image[(i*width+j)*3]=0*image[(i*width+j)*3];
            image[(i*width+j)*3+1]=1*image[(i*width+j)*3+1];
            image[(i*width+j)*3+2]=0*image[(i*width+j)*3+2];
        }
    }
}

void invertColor(unsigned char *image, int width, int height){
    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j){
            for(int r=0;r<3;++r){
                image[(i*width+j)*3+r]=255-image[(i*width+j)*3+r];
            }
        }
    }
}

void blurFilter(unsigned char *image, int width, int height){
    unsigned char* blurimg=malloc(width*height*3);
    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j){
            unsigned char* center=&image[(i*width+j)*3];
            unsigned char* upleft=&image[((i-1)*width+(j-1))*3];
            unsigned char* up=&image[((i-1)*width+j)*3];
            unsigned char* upright=&image[((i-1)*width+(j+1))*3];
            unsigned char* down=&image[((i+1)*width+j)*3];
            unsigned char* downleft=&image[((i+1)*width+(j-1))*3];
            unsigned char* right=&image[(i*width+j+1)*3];
            unsigned char* downright=&image[((i+1)*width+(j+1))*3];
            unsigned char* left=&image[(i*width+j-1)*3];
            for(int r=0;r<3;++r){
                blurimg[(i*width+j)*3+r]=((center[r]+up[r]+down[r]+right[r]+left[r]+upleft[r]+upright[r]+downleft[r]+downright[r])/9);
            }
        }
    }
    for(int i=0;i<width*height*3;++i){
        image[i]=blurimg[i];
    }
    free(blurimg);
}

void shadowing(unsigned char *image, int width, int height){
    double x=1.00000000;
    //double step=1/(double)height;
    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j){
            for(int r=0;r<3;++r){
                image[(i*width+j)*3+r]=x*image[(i*width+j)*3+r];
            }
        }
        x=x-0.0007;
    }
}

void shadowing2(unsigned char *image, int width, int height){
    double x=1.003;
    for(int i=0;i<height;++i){
        x=1.003;
        for(int j=0;j<width;++j){
            for(int r=0;r<3;++r){
                image[(i*width+j)*3+r]=x*image[(i*width+j)*3+r];
            }
            x=x-0.003;
        }
    }
}

void shadowing3(unsigned char *image, int width, int height){//ino to bzn!!!!!!!(lotfan!!)
    double x=1.003;
    double y=1.003;
    for(int i=0;i<height;++i){
        x=1.003;
        for(int j=0;j<width;++j){
            for(int r=0;r<3;++r){
                image[(i*width+j)*3+r]=x*y*image[(i*width+j)*3+r];
            }
            x=x-0.003;
        }
        y=y-0.003;
    }
}

void shadowing4(unsigned char *image, int width, int height){
    double x=1.003;
    double y=1.003;
    double m=0;
    double n=0;
    for(int i=0;i<height;++i){
        x=1.003;
        m=0;
        for(int j=0;j<width;++j){
            for(int r=0;r<3;++r){
                image[(i*width+j)*3+r]=m*n*x*y*image[(i*width+j)*3+r];
            }
            x=x-0.003;
            m=m+0.003;
        }
        y=y-0.003;
        n=n+0.003;
    }
}

void applykalepaFilter(unsigned char *image, int width, int height){
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

void zigzagimage(unsigned char *image, int width, int height) {
    int dir = 1;  // 1 for moving right, -1 for moving left
    int row;
    int col;

    for (int y = 0; y < height; y++) {
        row = dir == 1 ? y : height - y - 1;
        for (int x = 0; x < width; x++) {
            col = dir == 1 ? x : width - x - 1;

            // Calculate the index for the current pixel
            int pixelIndex = (y * width + x) * 3;

            // Calculate the index for the zigzagged position
            int zigzagIndex = (row * width + col) * 3;

            // Copy the pixel values to the zigzagged position
            for (int c = 0; c < 3; c++) {
                image[zigzagIndex + c] = image[pixelIndex + c];
            }
        }
        dir *= -1;
    }
}


//*****************************************************************
//function to find if file name is actually a picture that the program can handle :
int validation_check(const char *filetype,const char *valid_types[],int types){
  int i;
  for(i=0;i<types;i++){
    if(str_cmp(filetype,valid_types[i]))
      return 1;
  }
  return 0;
}
//function to copy the whole image file into another array
void image_cpy(unsigned char *image,unsigned char *image_copy,int width, int height, int channels){
  int i;
  int file_size = width *height*channels;
  for(i=0;i<file_size;i++){
    image_copy[i] = image[i];
  }
}
//function to extract file type from file name
//returns the index where the dot of filetype starts
int find_filetype(char *filetype,char *filename){
  FILE *file = fopen(filename, "rb");
   if (file == NULL) {
       printf("Error opening file.\n");
   }
   unsigned char buffer[8];  // Read the first few bytes of the file
   size_t bytesRead = fread(buffer, sizeof(unsigned char), 8, file);
   fclose(file);
   fseek(file,0,SEEK_SET);
   if (bytesRead < 8) {
       printf("Error reading file.\n");
   }
   // Compare the file signature with known signatures
   if (buffer[0] == 0x89 && buffer[1] == 'P' && buffer[2] == 'N' && buffer[3] == 'G') {
     str_cpy(".png",filetype); // PNG file
   } else if (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF) {
     str_cpy(".jpg",filetype);// JPEG file
   } else if (buffer[0] == 0x42 && buffer[1] == 0x4D) {
     str_cpy(".bmp",filetype);// BMP file
   }
   printf("inside function %s",filetype);
   int i =1;
   while(filename[i]!='.')i++;
   return i;
}
//handling url:
/*2 functions to extract file name from the given url so that I can save the file to my desktop
with its' actual name*/
int find_last_slash(char *sample_url){
  int i=0;int index;
  while(sample_url[i]!=0){
    if(sample_url[i]=='/')
      index = i;
    i++;
  }
  return index;
}

char *downloaded_file_name(char *sample_url){
  int last_slash;last_slash = find_last_slash(sample_url);
  int len ;len = str_len(sample_url+last_slash);
  char *file_name = (char *)malloc(len*sizeof(char));
  str_cpy(sample_url+last_slash+1,file_name);
  return file_name;
}



//********************************************************************
//functions needed to open bmp picture
/*this function takes 4 bit hexadecimal numbers and converts it to decimal
(it actually is to extract data from bmp image header file)*/
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
//These functions are used to handle email
/*this function takes header_file and extracts the UID part from it so
that I would be able to find the next emails UID*/
int get_UID(FILE *header_file){
  int file_size,uid; uid = 0;
  char *uid_index;
  fseek(header_file, 0, SEEK_END); // seek to end of file
  file_size = ftell(header_file); // get current file pointer
  fseek(header_file, 0, SEEK_SET); // seek back to beginning of file
  char *buffer = (char *)malloc(sizeof(char) * (file_size + 1)); // allocate space for buffer
  if (!buffer) { // check if malloc failed
    printf("Failed to allocate buffer!\n");
    exit(1);
  }
  if (fread(buffer, 1, file_size, header_file) != file_size) { // check if fread failed, and that we read the right amount of data
    printf("Failed to read file!\n");
    free(buffer); // free previously allocated buffer before exiting
    exit(1);
  }
  buffer[file_size] = '\0'; // add null terminator to buffer
  uid_index = strstr(buffer,"UIDNEXT");
  while(*uid_index<'0'||*uid_index>'9')uid_index++;
  while(*uid_index>='0' && *uid_index<='9'){
    uid = uid*10+(*uid_index)-'0';
    uid_index++;
  }
  free(buffer);
  return uid;
}
void decode_and_save_image(const char* encoded_image, const char* file_name) {
    // Create a b64 decoder context
    base64_decodestate state;
    base64_init_decodestate(&state);

    // Determine the length of the decoded data
    size_t encoded_len = strlen(encoded_image);
    size_t max_decoded_len = (encoded_len * 3) / 4 + 4;
    char* decoded_image = (char*) malloc(max_decoded_len);

    // Decode the image data
    size_t num_decoded_bytes = base64_decode_block(encoded_image, encoded_len, decoded_image, &state);

    // Save the decoded data to disk
    char path[100];
    sprintf(path,"./%s",file_name);
    FILE* output_file = fopen(path, "wb");
    fwrite(decoded_image, num_decoded_bytes, 1, output_file);
    fclose(output_file);

    // Cleanup
    free(decoded_image);
}


char * parse_email(FILE *email_body,char *filename){
  char buffer[1000]={0};
  char *file_name=(char *)malloc(100);
  char boundary_string[100];
  int i,j;i=j=0;
  fread(buffer,1,999,email_body);
  char *find_boundary = strstr(buffer,"boundary=");
  find_boundary+=10;
  while(*find_boundary!='"'){boundary_string[i++]=*find_boundary;find_boundary++;}boundary_string[i]=0;
  fseek(email_body,0,SEEK_SET);
  while(fgets(buffer,1000,email_body)){
    if(strstr(buffer,boundary_string)){
      while(!strstr(buffer,"Content-Type:")){
        fgets(buffer,1000,email_body);}
      if((file_name = strstr(buffer,"Content-Type: image/"))){
        file_name =strstr(buffer,"=");file_name+=2;
        while(file_name[j++]!='"');
        file_name[j-1]=0;
        strcpy(filename,file_name);
        break;
      }
    }
  }
  for(int i=0;i<5;i++){
    fgets(buffer,1000,email_body);
  }
  char *image = (char *)malloc(100000000);
  int c,m;m=0;
  while ((c = fgetc(email_body)) != EOF) {
    image[m++]=c;
    }
  return image;
}




//********************************************************************
int main(){
  int operation = 1;
  int pic_num=0;
  int filename_end_index;
  char *filename;
  unsigned char picture_format_buffer[8];
  while(operation){
      filename = (char *)malloc(FILENAME_MAX*sizeof(char));
      printf("Enter 0 to close the app\nEnter 1 to open image from system files\nEnter 2 to open image using URL\nEnter 3 to email your image\n");
      operation=int_inp();
      if(operation==0)
        break;
      else if(operation==1){
        char enterfilename[FILENAME_MAX];
        printf("Enter file name: ");
        scanf("%s", enterfilename);
        str_cpy("./",filename);
        str_cat(filename,enterfilename);
      }
      else if(operation==2){
        char url[MAXURL_LEN];
        printf("Enter Url : ");
        scanf("%s",url);
        char *downloaded_name;
        downloaded_name = downloaded_file_name(url);
        FILE *fp;
        str_cpy("./",filename);
        str_cat(filename,downloaded_name);
        printf("%s",filename);
        // Initializing easy handle
        CURL *curl_handle = curl_easy_init();
        if (curl_handle) {
            // Setting up URL to download from
            curl_easy_setopt(curl_handle, CURLOPT_URL, url);
            // Open output file in binary write mode
            fp = fopen(filename,"wb");
            /*if (fp == NULL) {
                return 1;
            }*/
            // Setting write function to write data to output file
            curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, fwrite);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp);
            //setting up a res to store the downloaded file
            CURLcode res;
            res = curl_easy_setopt(curl_handle, CURLOPT_URL, url);
            //performing the transfer (easy transfer wich only works once)
            res = curl_easy_perform(curl_handle);

            curl_easy_cleanup(curl_handle);

            if (res != CURLE_OK) {
                printf("Error downloading file: %s\n", curl_easy_strerror(res));
                return 1;
            }
            // Clean up
            fclose(fp);
        }
      }
        else if(operation==3){
          //This operation is receiving from email :
          printf("Please send your image as an attachment to this email address : raha.rhm.1383@gmail.com\n");
          int UID_next,attempt_count;attempt_count=0;
          CURL *curl;
          CURL *curl_new;
          CURLcode res;
          FILE *header_file = fopen("./header_file.txt","w+");
          FILE *fp = fopen("./email_body.txt","w+");
          curl = curl_easy_init();
          //Initializing a curl easy handle with the folowing discribtion:
          if(curl) {
            curl_easy_setopt(curl, CURLOPT_USERNAME, "raha.rhm.1383@gmail.com");
            curl_easy_setopt(curl, CURLOPT_PASSWORD, "iflkoyzpeyjadloy");
            curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com");
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "SELECT INBOX");
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, header_file);
            res = curl_easy_perform(curl);

            //first request is to get back a sample headerfile to get the next emails UID
            if(res != CURLE_OK){
              printf("failed to login boo\n");
              fprintf(stderr, "curl_easy_perform() getting latest UID failed(probably network error check your network Please): %s\n",
                      curl_easy_strerror(res));
            }

            curl_easy_cleanup(curl);
            UID_next = get_UID(header_file);
            fclose(header_file);
            curl_new = curl_easy_init();
            //The second request is made to get back the latest email and get back its body
            if(curl_new){
              char url[100];
              sprintf(url,"imaps://imap.gmail.com/INBOX;UID=%d;SECTION=TEXT",UID_next);
              curl_easy_setopt(curl_new, CURLOPT_USERNAME, "raha.rhm.1383@gmail.com");
              curl_easy_setopt(curl_new, CURLOPT_PASSWORD, "iflkoyzpeyjadloy");
              curl_easy_setopt(curl_new, CURLOPT_URL, url);
              curl_easy_setopt(curl_new, CURLOPT_WRITEDATA, fp);
              res = curl_easy_perform(curl_new);
              //this part is for the app to wait untill the latest email is sent
              while(!strcmp(curl_easy_strerror(res),"Remote file not found")){
                res = curl_easy_perform(curl_new);
              }
              if(res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() getting the latest email body failed: %s\n", curl_easy_strerror(res));
              }
            }
            curl_easy_cleanup(curl_new);
          }
          fclose(fp);

          FILE *email_body= fopen("./email_body.txt","r");
          char *file_name = (char *)malloc(100);
          char *image_in;
          FILE *image_out = fopen("./image_out.txt","w");
          image_in = parse_email(email_body,file_name);
          decode_and_save_image(image_in,file_name);
          //saving file_name in filename so that i can open the file later to add filters on it
          str_cat(filename,file_name);
          fclose(email_body);
        }
      //this part of code is to check if the given type is actually a valid image type
      char *filetype = (char *)malloc(10*sizeof(char));
      int i;
      filename_end_index = find_filetype(filetype,filename);
      printf("file type     %s",filetype);
      const char *valid_types[]={".bmp" , ".jpg", ".png"};
      if(!validation_check(filetype,valid_types,3)){
        printf("Error : Invalid file type");
        return 1;
      }

      //*****************************************************

      printf("file name :    %s\n",filename);
      //**************************************************
      //taking image info according to file type
      unsigned char *image;
      unsigned char *image_copy;
      int j,width,height,channels,offset,file_size,filter,filter_count;
      char *bmp_header;
      FILE *fo ;
      //working on bmp image file :
      if(str_cmp(filetype,".bmp")){
          //opening file in binary mode
          fo = fopen(filename, "rb");
          // check if the file was opened successfully
          if(fo == NULL){
            printf("Error opening file.\n");
            return 1;
          }
          bmp_header = ( char *)malloc(54);
          fread(bmp_header, 1, 54, fo);
          width  = little_endian_hex_to_int(bmp_header+18);
          height = little_endian_hex_to_int(bmp_header+22);
          offset = little_endian_hex_to_int(bmp_header+10);
          int bpp = little_endian_hex_to_int_bpp(bmp_header+28);
          channels = bpp/8;
          //printf("%d",bpp);
          //printf("%d",bitsPerPixel);
          file_size = little_endian_hex_to_int(bmp_header+2);
          //printf("\n%d  %d",file_size-offset,little_endian_hex_to_int(bmp_header+34));

          if(offset>54){
            bmp_header = ( char *)realloc(bmp_header,offset);
            fread(bmp_header+54, 1, offset-54, fo);
          }
          image = (unsigned char *)malloc(file_size-offset);
          image_copy = (unsigned char *)malloc(file_size-offset);
          if(image==NULL){
            printf("Failed to allocate memory!");
            return 1;
          }
          fread(image, 1,file_size-offset, fo);
          fclose(fo);
        }
        else if(str_cmp(filetype,".jpg")){
          image = stbi_load(filename, &width, &height, &channels, 0);
          image_copy = (unsigned char *)malloc(width * height * channels);
          if (image == NULL) {
              printf("Failed to load jpg image from desktop.\n");
              return 1;
          }
        }
        else if(str_cmp(filetype,".png")){
          image = stbi_load(filename, &width, &height, &channels, 0);
          image_copy = (unsigned char *)malloc(width * height * channels);
          if (image == NULL) {
              printf("Failed to load png image from desktop.\n");
              return 1;
          }
        }
        //finished getting image file inside array
        //performing the desired filter :
        filter_count = 0;filter = 1;
        while(filter != 0){
          printf("Please enter the number of the filter you want to apply on your picture :\n");
          printf("0 if you want to end the editing proceess\n1 if you want to delete the latest applied filter\n2 for gray filter\n3 for sepia filter\n4 for chapaki filter\n5 for kalepa filter\n");
          filter = int_inp();
          if((filter ==1 && filter_count!=0) || filter_count==0)
            image_cpy(image,image_copy,width,height,channels);
          else{
            image_cpy(image_copy,image,width,height,channels);
          }
          if(filter ==2)
            convertToGray(image_copy,width,height);
          else if(filter ==3)
            converttoSepia(image_copy,width,height);
          else if(filter ==4)
            applychapakiFilter(image_copy,width,height,channels);
          else if(filter ==5)
            applykalepaFilter(image_copy,width,height);
          filter_count++;
        }


        //**************
        //taking out the filenames file format to save it in new name
        filename[filename_end_index]=0;
        printf("%s",filetype);
        if(str_cmp(filetype,".bmp")){
        //str_cat(filename,"_modified");
        sprintf(filename,"%s_modified%d.bmp",filename,pic_num);
        fo = fopen(filename, "wb");
        if (fo == NULL) {
          printf("could not create output file\n");
          return 1;
        }
        fwrite(bmp_header, 1,offset , fo);
        fwrite(image, 1, file_size-offset, fo);
        // closing the file when done
         fclose(fo);
         printf("modification done successfully\n");
      }
      else if(str_cmp(filetype,".jpg")){
        //str_cat(filename,"_modified.jpg");
        sprintf(filename,"_modified%d.jpg",pic_num);
        if (!stbi_write_jpg(filename, width, height, channels, image, width * channels)) {
          printf("Failed to save jpg image.\n");
          return 1;
        }
        //cleaning up
        stbi_image_free(image);
        printf("modification done successfully\n");
      }
      else if(str_cmp(filetype,".png")){
        //str_cat(filename,"_modified.png");
        sprintf(filename,"_modified%d.png",pic_num);
        if (!stbi_write_png(filename, width, height, channels, image, width * channels)) {
          printf("Failed to save png image.\n");
          return 1;
        }
        //cleaning up
        stbi_image_free(image);
        printf("modification done successfully\n");
      }
      pic_num++;
    }
    free(filename);
  return 0;
}
/*command prompt command for compiling  :
 gcc -o image_processing.out image_processing.c -L/Users/raha/Desktop/c_project/stb-master -I/Users/raha/Desktop/c_project/stb-master -lcurl -lb64*/
