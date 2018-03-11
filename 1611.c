#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
void printdir(DIR *mydir, char *name)
{    DIR *a = mydir;
     struct dirent *myfile;
     struct stat mystat;
     int total = 0;    
     while((myfile = readdir(a)) != NULL)
     {
       char buf3[512];
       sprintf(buf3, "%s/%s", name, myfile->d_name);
       if((strncmp(myfile->d_name,".",1)==0)||(strncmp(myfile->d_name,"..",2)==0))
            ;
       else{
        stat(buf3, &mystat);
        total = (mystat.st_blocks/2) + total; //
        }
     }
     printf("total : %d\n",total);

}; //for print total disk when command ls -l
 
void printoption_l(DIR *mydir, char *name)
{
     struct dirent *myfile;
     char buf[512];
     struct stat mystat;

     while((myfile = readdir(mydir)) != NULL)
      {
        sprintf(buf, "%s/%s", name, myfile->d_name);
        if((strncmp(myfile->d_name,".",1)==0)||(strncmp(myfile->d_name,"..",2)==0))
            ;
        else{
        stat(buf, &mystat);
        char buf2[512]; //for time
        if(S_ISREG(mystat.st_mode))
             printf("-"); //regular
        else if(S_ISDIR(mystat.st_mode))
              printf("d");//dir
        else if(S_ISCHR(mystat.st_mode))
             printf("c"); //char
        else if(S_ISBLK(mystat.st_mode))
             printf("b");//block
        else if(S_ISFIFO(mystat.st_mode))
             printf("p");//fifo
        else if(S_ISLNK(mystat.st_mode))
             printf("l");//link
        else
             printf("s");//socket
        printf("%s%s%s",(mystat.st_mode & S_IRUSR)?"r":"-",(mystat.st_mode & S_IWUSR)?"w":"-",(mystat.st_mode &S_IXUSR)?"x":"-");
        printf("%s%s%s",(mystat.st_mode & S_IRGRP)?"r":"-",(mystat.st_mode & S_IWGRP)?"w":"-",(mystat.st_mode &S_IXGRP)?"x":"-");
        printf("%s%s%s",(mystat.st_mode & S_IROTH)?"r":"-",(mystat.st_mode & S_IWOTH)?"w":"-",(mystat.st_mode &S_IXOTH)?"x":"-");
        printf(" %d ", mystat.st_nlink);
        printf(" %s ",getpwuid(mystat.st_uid)->pw_name);           printf(" %s ",getgrgid(mystat.st_gid)->gr_name);
        printf("  %d", mystat.st_size);
        strftime(buf2,512,"%Y-%m-%d %H:%M",localtime(&mystat.st_mtime));
        printf(" %s", buf2);
        printf("  %s\n", myfile->d_name);
      }}

} //function for print various options, command -l

int main(int argc, char *argv[])
{
  DIR *mydir;
  struct dirent *myfile;
  struct stat mystat; 
  char buf[512];
  char name[512];

  getcwd(name,100); //get location

 if(argc==1) //command 'ls', no argument
 {
  mydir = opendir(name); //opendir
  while((myfile = readdir(mydir)) != NULL)
  {     sprintf(buf, "%s/%s", name, myfile->d_name);
 	if((strncmp(myfile->d_name,".",1)==0)||(strncmp(myfile->d_name,"..",2)==0))
	    ;  //if file's name start with '.' or '..', skip to print
        else
         printf("%s   ", myfile->d_name); 
  }
  closedir(mydir);
  printf("\n");
 }// command 'ls' , no argument
 else if(argc ==2)
 {
  if(strcmp(argv[1],"-a")== 0)
  {   mydir = opendir(name);
      int a=0; //for print 3 items per line
      while((myfile = readdir(mydir)) != NULL) //read all files in dir
      { sprintf(buf, "%s/%s", name, myfile->d_name);//by doing this buf will be file name
        printf(" %-20s ", myfile->d_name); //print file name
        a++;
        if(a!=0 && a%3 ==0)
         printf("\n"); 
      }
  closedir(mydir);
  printf("\n");
  } //option -a, command 'ls -a' show all files or directories
    //////////////////////////////////////////////////////////// 
  else if(strcmp(argv[1],"-l")==0)
  {  
     mydir = opendir(name); //open directory
     printdir(mydir,name);  //this function print total disk
     closedir(mydir);       //close direcoty

     mydir = opendir(name); //open directory
     printoption_l(mydir,name); //print detail
     closedir(mydir);  //close directoty
   //option - l, command 'ls -l' show detail
  }
  else
  {
   if((mydir = opendir(argv[1]))==NULL) //if argument is not directory, 
     {
        if(stat(argv[1],&mystat) == -1) 
	 printf("wrong argument is passed \n"); //if argument is not directory or file
        else
         printf("%s\n",argv[1]); //if argument is file, then print file name
     }
   else{
     while((myfile = readdir(mydir)) != NULL)
     {  sprintf(buf, "%s/%s", name, myfile->d_name); 
        if((strncmp(myfile->d_name,".",1)==0)||(strncmp(myfile->d_name,"..",2)==0))
            ;
        else
         printf("%s   ", myfile->d_name);
     }
    closedir(mydir);
    printf("\n");
   }
  }//no option, argument is diretocy name or file name, command 'ls /bin/'
 }
 else if(argc==3)
 { //  .\b [option] [name] //
   //////////////////////////
   if(strcmp(argv[1],"-a") == 0) //print all
   { if((mydir = opendir(argv[2]))==NULL)
     {                                  //if argument is not directory
        if(stat(argv[2],&mystat) == -1)
         printf("wrong argument is passed \n"); //if argument is not direcory or file
        else
         printf("%s\n",argv[2]);//printf file name   
     }
   else{  //if first argument is '-a' and second argument is directory
     int a=0;
     while((myfile = readdir(mydir)) != NULL)
     {  sprintf(buf, "%s/%s", name, myfile->d_name);
        printf(" %-10s ", myfile->d_name);
        a++;
        if(a!=0 && a%3 ==0)
         printf("\n");
     } //print all files and directoies in 'mydir'
    closedir(mydir);
    printf("\n");
   } 
   }
   else if(strcmp(argv[1],"-l") == 0)
     { //if first argument is '-l'
      if((mydir = opendir(argv[2]))==NULL)
      {
       if(stat(argv[2],&mystat) == -1)
           printf("wrong argument is passed \n");//if second argument is not file and directory
       else 
       {//if aurgument is file name
        char buf2[512]; //for time
        if(S_ISREG(mystat.st_mode))
             printf("-"); //regular
        else if(S_ISDIR(mystat.st_mode))
              printf("d");//dir
        else if(S_ISCHR(mystat.st_mode))
             printf("c"); //char
        else if(S_ISBLK(mystat.st_mode))
             printf("b");//block
        else if(S_ISFIFO(mystat.st_mode))
             printf("p");//fifo
        else if(S_ISLNK(mystat.st_mode))
             printf("l");//link
        else
             printf("s");//socket
        printf("%s%s%s",(mystat.st_mode & S_IRUSR)?"r":"-",(mystat.st_mode & S_IWUSR)?"w":"-",(mystat.st_mode &S_IXUSR)?"x":"-");
        printf("%s%s%s",(mystat.st_mode & S_IRGRP)?"r":"-",(mystat.st_mode & S_IWGRP)?"w":"-",(mystat.st_mode &S_IXGRP)?"x":"-");
        printf("%s%s%s",(mystat.st_mode & S_IROTH)?"r":"-",(mystat.st_mode & S_IWOTH)?"w":"-",(mystat.st_mode &S_IXOTH)?"x":"-");
        printf(" %d ", mystat.st_nlink);
        printf(" %s ",getpwuid(mystat.st_uid)->pw_name);           printf(" %s ",getgrgid(mystat.st_gid)->gr_name);
        printf("  %d", mystat.st_size);
        strftime(buf2,512,"%Y-%m-%d %H:%M",localtime(&mystat.st_mtime));
        printf(" %s", buf2);
        printf("  %s\n",argv[2]);//print arguemnt (= file) name
       } //print detail about file(=argument)
      }
     else{//second argument is directory
      printdir(mydir,argv[2]);
      closedir(mydir); //print total disk
     
      mydir = opendir(argv[2]);
      printoption_l(mydir,argv[2]); //print detail about files and directories in 'mydir'
      closedir(mydir);
     }
     }
  else
     printf("other options are not supported\n"); //except -a and -l
 }
}

