#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#define MAXURL_LEN 100
#define FILENAME_MAX 100

//reading file from internet

int main(){
    char url[MAXURL_LEN];
    scanf("%s",url);
    FILE *fp;
    char outfilename[FILENAME_MAX] = "out_file.bin";
    // Initializing easy handle
    CURL *curl_handle = curl_easy_init();
    if (curl_handle) {
        // Setting up URL to download from
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        // Opening output file in binary write mode
        fp = fopen(outfilename,"rb");
        //cleaning up the handle in case the file doesnt open properly
        if (fp == NULL) {
            curl_easy_cleanup(curl_handle);
            return 1;
        }

        // Setting write function to write data to output file
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, fwrite);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp);
        //setting up a res to store the downloaded file
        CURLcode res;
        res = curl_easy_setopt(easy_handle, CURLOPT_URL, url);
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

    // Open downloaded file in binary read mode for editing
    fp = fopen(outfilename,"rb");
    if (fp == NULL) {
        return 1;
    }

    //after this i will need to extract width hight and bpp so that i can work on the picture
    //adding filters and stuff
    // closing the file when done
    fclose(fp);

    return 0;
}
