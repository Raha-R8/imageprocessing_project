#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
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
int main(){
  char filename[100];
  printf("Enter file name: ");
  scanf("%s", filename);
  //this part of code is to check if the given type is actually a valid image type
  const char* filetype;
  int i=0;
  while(filename[i]!='.')i++;
  filetype = filename+i;
  const char *valid_types[]={".bmp", ".gif", ".jpg", ".jpeg", ".png"};
  if(!validation_check(filetype,valid_types)){
    printf("Error : Invalid file type");
    return 1;
  }
  //********************************
  //opening file in binary mode
  FILE *fp = fopen(filename, "rb");
  // check if the file was opened successfully
  if(fp == NULL){
    printf("Error opening file.\n");
    return 1;
  }
  //adding filters and stuff
  // closing the file when done
   fclose(fp);
  return 0;
}
