#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include "/usr/local/Cellar/libb64/1.2.1_1/include/b64/cdecode.h"
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
    sprintf(path,"/Users/raha/Desktop/%s",file_name);
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



int main(void){
  int UID_next,attempt_count;attempt_count=0;
  CURL *curl;
  CURL *curl_new;
  CURLcode res;
  FILE *header_file = fopen("/Users/raha/Desktop/c_project/email/gmail/header_file.txt","w+");
  FILE *fp = fopen("/Users/raha/Desktop/c_project/email/gmail/email_body.txt","w+");
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_USERNAME, "raha.rhm.1383@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "iflkoyzpeyjadloy");
    curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com");
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "SELECT INBOX");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, header_file);
    res = curl_easy_perform(curl);


    if(res != CURLE_OK){
      printf("failed to login boo\n");
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
    UID_next = get_UID(header_file);
    fclose(header_file);
    curl_new = curl_easy_init();
    if(curl_new){
      char url[100];
      sprintf(url,"imaps://imap.gmail.com/INBOX;UID=%d;SECTION=TEXT",UID_next);
      curl_easy_setopt(curl_new, CURLOPT_USERNAME, "raha.rhm.1383@gmail.com");
      curl_easy_setopt(curl_new, CURLOPT_PASSWORD, "iflkoyzpeyjadloy");
      curl_easy_setopt(curl_new, CURLOPT_URL, url);
      curl_easy_setopt(curl_new, CURLOPT_WRITEDATA, fp);
      res = curl_easy_perform(curl_new);
      while(!strcmp(curl_easy_strerror(res),"Remote file not found")){
        res = curl_easy_perform(curl_new);
      }
      if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
      }
    }
    curl_easy_cleanup(curl_new);
  }
  fclose(fp);
  FILE *email_body= fopen("/Users/raha/Desktop/c_project/email/gmail/email_body.txt","r");
  char *file_name = (char *)malloc(100);
  char *image;
  FILE *image_out = fopen("/Users/raha/Desktop/c_project/email/gmail/image_out.txt","w");
  image = parse_email(email_body,file_name);
  decode_and_save_image(image,file_name);
  fclose(email_body);
  return 0;
}
