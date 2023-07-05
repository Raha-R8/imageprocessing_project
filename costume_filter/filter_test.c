#include <stdio.h>
#include <stdlib.h>
#define MAXFILTERLEN 27
#define MAX_LINE_LEN 1000
struct costume{
  int number;
  char *filters;
  struct costume *next;
};

struct costume *creat_struct(int number,char *filters){
  struct costume *new = (struct costume *)malloc(sizeof(struct costume));
  new->filters = filters;new->number = number;new->next = NULL;
  return new;
}



int int_inp(char *last_char){
  int n,num;
  while(((n=getchar())>'9')||(n<'0'));
  num = n-'0';
  while((n=getchar())&&(n<='9')&&(n>='0')){
      num = num*10+(n-'0');
    }
  *last_char = n;
  return num;
}
void extractNumbers(char *array, int filters[]) {
    char *each = (char *)malloc(10);
    int i,j,index;i=j=index = 0;
    while (array[i]!=0) {
        while(array[i]!=0&&array[i]!=' '){
          each[j] = array[i];i++;j++;
        }each[j]=0;j=0;
        filters[index] = atoi(each);
        index++;
        i++;
    }
    filters[index]=0;
}
void print_costume_filter(char *filter_string){
    int filters[MAXFILTERLEN];
    extractNumbers(filter_string,filters);
    int i=0;
    while(filters[i]!=0){
      if(filters[i]<=20){
        switch (filters[i]) {
          case 2 : printf("applychapakiFilter ");break;
          case 3 : printf("applykalepaFilter ");break;
          case 4 : printf("filter4in1 ");break;
          case 5 : printf("blurFilter ");break;
          case 6 : printf("invertColor ");break;
          case 7 : printf("morecolorful ");break;
          case 8 : printf("purpleimage ");break;
          case 9 : printf("greenimage ");break;
          case 10 : printf("zigzagimage ");break;
          case 11 : printf("shadowing ");break;
          case 12 : printf("shadowing2 ");break;
          case 13 : printf("shadowing3 ");break;
          case 14 : printf("badquality ");break;
          case 15 : printf("saabet ");break;
          case 16 : printf("saabet2 ");break;
          case 17 : printf("convertToGray ");break;
          case 18 : printf("converttoSepia ");break;
          case 19 : printf("nesf ");break;
          case 20 : printf("threshold ");break;
        }
      }
      else{
        if(filters[i]==21){
          printf("brightness :");i++;
          printf("percentage : %d ",filters[i]);
        }
        else if(filters[i]==22){
          printf("contrast :");i++;
          printf("percentage : %d ",filters[i]);
        }
        else if(filters[i]==23){
          printf("tint :");i++;
          printf("percentage : %d ",filters[i]);
        }
      }
      i++;
    }
}
void apply_costume_filter(char *filter_string,unsigned char *image,int width,int height,int channels){
    int filters[MAXFILTERLEN];
    extractNumbers(filter_string,filters);
    int i=0;
    int percentage;
    while(filters[i]!=0){
      if(filters[i]<=20){
        switch (filters[i]) {
          case 2 : applychapakiFilter(image,width,height,channels);break;
          case 3 : applykalepaFilter(image,width,height,channels);break;
          case 4 : filter4in1(image,width,height,channels);break;
          case 5 : blurFilter(image,width,height,channels);break;
          case 6 : invertColor(image,width,height,channels);break;
          case 7 : morecolorful(image,width,height,channels);break;
          case 8 : purpleimage(image,width,height,channels);break;
          case 9 : greenimage(image,width,height,channels);break;
          case 10 : zigzagimage(image,width,height,channels);break;
          case 11 : shadowing(image,width,height,channels);break;
          case 12 : shadowing2(image,width,height,channels);break;
          case 13 : shadowing3(image,width,height,channels);break;
          case 14 : badquality(image,width,height,channels);break;
          case 15 : saabet(image,width,height,channels);break;
          case 16 : saabet2(image,width,height,channels);break;
          case 17 : convertToGray(image,width,height,channels);break;
          case 18 : converttoSepia(image,width,height,channels);break;
          case 19 : nesf(image,width,height,channels);break;
          case 20 : threshold(image,width,height,channels);break;
        }
      }
      else{
        if(filters[i]==21){
          i++;percentage = filters[i];
          change_brightness(image,width,height,channels,percentage);
        }
        else if(filters[i]==22){
          i++;percentage = filters[i];
          adjust_contrast(image,width,height,channels,percentage);
        }
        else if(filters[i]==23){
          i++;percentage = filters[i];
          tint(image,width,height,channels,percentage);
        }
      }
      i++;
    }
}

void show_costume_filters(struct costume *root){
  if(root==NULL){
    printf("There are no costume filters please create one first.\n");
    return;
  }
  while(root->next!=NULL){
    print_costume_filter(root->filters);
    printf("\n");
  }
}



struct costume *read_filters_file(char *filename){
  struct costume *root=NULL;
  struct costume *new = NULL;
  FILE *fp = fopen(filename,"r");
  char *filter_string = (char *)malloc(100*sizeof(char));
  int i=0;
  while(fgets(filter_string,100,fp)){
    new = creat_struct(i,filter_string);
    if(i==0){
      root = new;
    }
    else{
      root->next = new;
    }
    filter_string = (char *)malloc(100*sizeof(char));
    i++;
  }
  return root;
}

void add_filter_to_struct(struct costume*root,char *filter_string){
  if(root==NULL)return;
  struct costume *new;
  int i=0;
  while(root->next){
    root = root->next;
    i++;
  }
  new = creat_struct(i+1,filter_string);
  root->next = new;
}
void append_string_to_file(FILE *file, const char *string) {
    fseek(file, 0, SEEK_END);
    fprintf(file, "%s\n", string);
}

struct costume *get_user_filter(char *filename){
  int c,num,i,j;i=j=0;
  FILE *fp = fopen(filename,"r+");
  struct costume *root;
  struct costume *temp;
  char *filter_string = (char *)malloc(MAX_LINE_LEN*sizeof(char));
  while(c!='\n'){
    while((c=getchar())!='\n'&& (c<'0'||c>'9'));
    if(c=='\n')break;
    filter_string[j++]=c;
    while((c=getchar())!='\n'&&(c<='9'&&c>='0')){
      filter_string[j++]=c;
    }filter_string[j++]=' ';
  }
  filter_string[j-1]=0;
  //now I will read the file of previous costume filters
  root=read_filters_file(filename);
  if(root==NULL){
    root = creat_struct(0,filter_string);
  }
  else{
    temp = root;
    add_filter_to_struct(temp,filter_string);
  }
  append_string_to_file(fp,filter_string);
  return root;
}


int main(){
  char *file_name;
  struct costume *root;
  file_name = "./costume_filters.txt";
  root = read_filters_file(file_name);
  root = get_user_filter(file_name);
  return 0;
}
