#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <glob.h>
#include <stdbool.h>
#include <stdarg.h>
#include "dbg.h"
#include "colors.h"


#define MAX_LENGTH 100
// these variable argument functions are absolutley horrible and bad design,
// but i did it for practice and fun ;)
// could have just used an use_or integer if I wanted to use or comparision
typedef int (*scan_type)(int num,...);

int scan_and(int found_it, ...){
  
  va_list args;
  int search_num = 0;

  va_start(args,found_it);

  search_num = va_arg(args,int);

  if(found_it == search_num) return 1;

  va_end(args);
  return 0;

}
// could use variable arguments
int scan_or(int found_it, ...){

  va_list args;

  va_start(args,found_it);

  if(found_it > 0) return 1;

  va_end(args);
  return 0;

}

int scan_file(char *file_path,char **words,int search_num,int (*scan)(int num,...)){

  int i = 0;
  int line_num = 0;
  int found_it = 0;

  FILE *log_file = fopen(file_path,"r");
  char *line = NULL;
  size_t buf = 0;
  ssize_t read = 0;

  check(log_file,"Failed to open file");

  // read each line of the file
  while((read = getline(&line,&buf,log_file)) != -1){
    check_mem(line);

    // for every word in the words wanting to be searched, check if the word is
    // in the line, if so, incrmemnt the found it integer
    for(i = 0;i<search_num;i++){
      //size_t word_length = strnlen(words[i],MAX_LENGTH);
      if(strcasestr(line,words[i]) != NULL){
        found_it++;
      }
     }
      // once the line was scanned, check if it meets the criteria, depending
      // on each scan type
      if(scan(found_it,search_num)){
        // if so, output the file path and line number 
        printf(SET_GREEN("%s") ":" SET_CYAN("%d")"\n",file_path,line_num);
        // then break, since we have found all the words we needed, and we can
        // go onto the next file
        break;
      }else{
        // if no words were found, depending on scan type, reset found_it and
        // go to the next line
        found_it = 0;
      }
      line_num++;
    }


  free(line);
  fclose(log_file);
  return 0;


error:
  free(line);
  fclose(log_file);
  return -1;
}


int find_files(glob_t *glob_results){

  int rc = -1;
  // find list of files
  rc = glob("/var/log/*.log",GLOB_TILDE,NULL,glob_results);

  check(rc == 0 || rc == GLOB_NOMATCH,"Failed to glob");
    
  rc = 0;

  // nice little fallthrough trick, maybe not good design
error:
  return rc;
}


int parse_args(int *argc,char **argv[],scan_type *scan){
  
   (*argc)--;
   (*argv)++;
   check(*argc > 0, "Too few arguments"); 

   if(strcmp((*argv)[0],"-o") == 0){
      (*argc)--; // skip the  -o
      (*argv)++;
      *scan = scan_or;
      check(*argc > 0, "Too few arguments"); 
   }else{
      *scan = scan_and;
   }

   return 0;
error:
   return -1;

}

int main(int argc, char *argv[]){
  
   int i = 0;
   //int (*scan_type)(int num,...) = scan_or;
   scan_type scan = scan_or;
   glob_t glob_results;
    
   check(parse_args(&argc,&argv,&scan) == 0, "USAGE: logfind [-o] words");
    
   // find list of files
   check(find_files(&glob_results) == 0, "Failed to find files");

   for(i = 0;i<glob_results.gl_pathc;i++){
      scan_file(glob_results.gl_pathv[i],argv,argc,scan);
   }

  globfree(&glob_results);
  return 0;

error:
    return -1;
}
