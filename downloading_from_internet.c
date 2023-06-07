#include <curl/curl.h>
#include <stdio.h>
//#include <FreeImage.h>
#include <stdlib.h>
#include <string.h>
/*#include <limits.h>

#undef PNG_Z_DEFAULT_COMPRESSION
#include <png.h>
#undef PNG_Z_DEFAULT_COMPRESSION
#include <jpeglib.h>*/
#define MAXURL_LEN 1000000

struct dimension{
  int width;
  int height;
};
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

//*****************************************
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
//reading file from internet

int main(){
    char url[MAXURL_LEN];
    printf("Enter Url : ");
    scanf("%s",url);
    char *downloaded_name;
    downloaded_name = downloaded_file_name(url);
    FILE *fp;
    //char outfilename[FILENAME_MAX] = "outfile.bin";
    char filename[FILENAME_MAX];
    str_cpy("/Users/raha/Desktop/",filename);
     strcat(filename,downloaded_name);

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
    const char* filetype;
    int i=0;
    while(filename[i]!='.')i++;
    filetype = filename+i;
    const char *valid_types[]={".bmp", ".gif", ".jpg", ".jpeg", ".png"};
    if(!validation_check(filetype,valid_types,5)){
      printf("Error : Invalid file type");
      return 1;
    }

    //*****************************************************
    //opening file in binary mode
    fp = fopen(filename, "rb");
    // check if the file was opened successfully
    if(fp == NULL){
      printf("Error opening file.\n");
      return 1;
    }
    //*************************************************
  
    //**************************************************
    //adding filters and stuff
    // closing the file when done
     fclose(fp);
    return 0;
  }
