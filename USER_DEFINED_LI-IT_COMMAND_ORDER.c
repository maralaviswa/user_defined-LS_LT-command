#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<dirent.h>
#include<string.h>

int cnt=0,i;
typedef struct node
{
   int tym;
   char arr[256];
   char str[256];
}S;
char* find(int i)
{
        switch(i)
        {
                case 0:return "JAN";
                case 1:return "FEB";
                case 2:return "MAR";
                case 3:return "APR";
                case 4:return "MAY";
                case 5:return "JUN";
                case 6:return "JUY";
                case 7:return "AUG";
                case 8:return "SEP";
                case 9:return "OCT";
                case 10:return"NOV";
                case 11:return"DEC";
        }
}
void convert(int mode)
{
    if(S_ISREG(mode)) printf("-");
    else if(S_ISDIR(mode)) printf("d");
    else if(S_ISCHR(mode)) printf("c");
    else if(S_ISBLK(mode)) printf("b");
    else if(S_ISFIFO(mode))  printf("p");
    else
    printf("l");
    printf((mode & 0400)?"r":"-");
    printf((mode & 0200)?"w":"-");
    printf((mode & 0100)?"x":"-");
    printf((mode & 0040)?"r":"-");
    printf((mode & 0020)?"w":"-");
    printf((mode & 0010)?"x":"-");
    printf((mode & 0004)?"r":"-");
    printf((mode & 0002)?"w":"-");
    printf((mode & 0001)?"x":"-");
}

S* sort(S *var)
{
         S temp;
        for(int i=cnt-1;i>0;i--)
        {
                for(int j=0;j<i;j++)
                {
                        if(var[j].tym<var[j+1].tym)
                        {
                             temp=var[j];
                             var[j]=var[j+1];
                             var[j+1]=temp;
                        }
                }
        }
        return var;
}
int main(int argc,char *argv[])
{
        int total=0;
        S *w=NULL;
        if(argc<2)
        {
                printf("invalid input\n");
                exit(0);
        }
        DIR *p=opendir(argv[1]);
        if(p==NULL)
        {
                perror("opendir");
                exit(0);
        }
        struct stat var,var1;
        struct dirent *q=NULL;
        char fullpath[1050];
        while((q=readdir(p))!=NULL)
        {
         total+=var1.st_blocks;
        snprintf(fullpath, sizeof(fullpath), "%s/%s", argv[1], q->d_name);
        if(stat(fullpath,&var1)==-1)
        //if(stat(q->d_name,&var)==-1)
        {
                perror("stat");
                exit(0);
        }
         w=realloc(w,(cnt+1)*sizeof(S));
         if(w==NULL)
         {
                 perror("realloc");
                 closedir(p);
                 exit(0);
         }
         w[cnt].tym=var1.st_atime;
         strcpy(w[cnt].arr,fullpath);
         strcpy(w[cnt++].str,q->d_name);
        }
        closedir(p);
        w=sort(w);
        for(int i=0;i<cnt;i++)
        {
          if(stat((w[i].arr),&var)==-1)
          {
                  perror("stat1");
                  exit(0);
          }
        struct passwd *pw=getpwuid(var.st_uid);
        struct tm *time=localtime(&var.st_atime);
        struct group *gr=getgrgid(var.st_gid);
        if(pw==NULL || gr==NULL)
        {
                perror("getpwuid");
        }
        convert(var.st_mode);
        printf("% ld ",var.st_nlink);
        printf("%s %s ",pw->pw_name,gr->gr_name);
        printf("%5ld ",var.st_size);
        printf("%s %2d %2d:%2d ",find(time->tm_mon),time->tm_mday,time->tm_hour,time->tm_min);
        printf("%s\n",w[i].str);
        }
}