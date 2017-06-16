#include "minunit.h"
#include <lcthw/bstrlib.h>


char *create_bstring(){
  
  bstring string = bfromcstr("Hello");
  printf("String is %s\n",bdata(string));

  bstring string_2 = blk2bstr("Hello",4);
  printf("String is %s\n",bdata(string_2));

  return NULL;
}

char *copy_bstr(){

  bstring string = bfromcstr("I love bstrings");
  bstring string_copy = bstrcpy(string);
  
  printf("Copy of string is %s\n",bdata(string_copy));
  
  return NULL;
}

char *assign_bstring(){

  bstring string = bfromcstr("I love assigning bstrings");
  bstring string_2 = bfromcstr("Hello shit");
  bassign(string_2,string);

  printf("Assignment of string to string_2 is %s\n",bdata(string_2));

  bstring string_3 = bfromcstr("Hello shit face poo");
  bassignblk(string_3,string_2,7);

  printf("Assignment of string_2 to string_3 is %s\n",bdata(string_3));

  return NULL;
}

char *bstring_concat(){

  bstring string = bfromcstr("Hello I am");
  bstring string_2 = bfromcstr(" Harry the wizard!!!!");

  bconcat(string, string_2);
  printf("New string is %s\n",bdata(string));

  return NULL;
}


char *bstring_destroy(){

  bstring string = bfromcstr("Hello there don't destroy me");
  bdestroy(string);

  return NULL;
}

char *bstr_cmp(){

  bstring string_1 = bfromcstr("Hello");
  bstring string_2 = bfromcstr("Hello");

  if(bstricmp(string_1,string_2) == 0) printf("Strings are the same\n");

  bstring string_3 = bfromcstr("hellOs");
  bstring string_4 = bfromcstr("Hello");

  if(bstricmp(string_3,string_4) > 0) printf("Strings are different\n");

  return NULL;
}

char *bstr_equal(){

  
  bstring string_1 = bfromcstr("Hello");
  bstring string_2 = bfromcstr("Hello");

  if(biseq(string_1,string_2)) printf("Strings are the same\n");

  bstring string_3 = bfromcstr("hellOs");
  bstring string_4 = bfromcstr("Hello");

  if(biseq(string_3,string_4) == 0) printf("Strings are different\n");

  return NULL;
}

char *binstr_sub(){

  bstring string_1 = bfromcstr("Hello");
  bstring string_2 = bfromcstr("lo");

  int position = binstr(string_1,0,string_2);
  printf("Position is %d\n",position);

  return NULL;
}

char *bstr_replace(){

  bstring string_1 = bfromcstr("Hello there how may I help you?");
  bfindreplace(string_1,bfromcstr("there"),bfromcstr("fuck you and die, also"),0);

  printf("Edited string is %s\n",bdata(string_1));
  return NULL;
}

char *bstr_split(){
  int i = 0;
  bstring string = bfromcstr("Hello,how,are,you?");
  struct bstrList * list = bsplit(string,',');
  printf("The string was split into %d pieces\n",list->qty);
  printf("The strings split were:\n");
  for(i = 0; i < list->qty;i++){
    printf("%s\n",bdata(list->entry[i]));
  }

  return NULL;
}

char *bstr_format(){

  bstring string = bfromcstr("Hello,how,are,you?");
  bstring format_string = bformat("%s %s %d",bdata(string),bdata(string),55);

  printf("Formatted string is %s\n",bdata(format_string));

  return NULL;
}

char *bstr_length(){

  bstring string = bfromcstr("Hello,how,are,you?");
  printf("Length of string \"%s\" is : %d characters\n",bdata(string),blength(string));

  return NULL;
}

char *bstr_char(){

  bstring string = bfromcstr("Hello,how,are,you?");
  printf("First char of string is %c and last char is %c\n",bchar(string,0),bchar(string,blength(string) - 1));

  return NULL;
}


char *all_tests(){

  mu_suite_start();

  mu_run_test(create_bstring);
  mu_run_test(copy_bstr);
  mu_run_test(assign_bstring);
  mu_run_test(bstring_concat);
  mu_run_test(bstring_destroy);
  mu_run_test(bstr_cmp);
  mu_run_test(bstr_equal);
  mu_run_test(binstr_sub);
  mu_run_test(bstr_replace);
  mu_run_test(bstr_split);
  mu_run_test(bstr_format);
  mu_run_test(bstr_length);
  mu_run_test(bstr_char);

  return NULL;
}

RUN_TESTS(all_tests);
