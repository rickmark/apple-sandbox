#include "scheme.h"
#include "dynload.h"

#include <string.h>
#include <stdlib.h>


#ifndef WIN32
# include <unistd.h>
#endif
#ifdef WIN32
# include <io.h> /* access */
# include <sys/types.h> /* off_t */
#define snprintf _snprintf
#endif

#define banner "TinyScheme 1.41"

#ifndef InitFile
# define InitFile "init.scm"
#endif


#if defined(__APPLE__) && !defined (OSX)
int main()
{
     extern MacTS_main(int argc, char **argv);
     char**    argv;
     int argc = ccommand(&argv);
     MacTS_main(argc,argv);
     return 0;
}
int MacTS_main(int argc, char **argv) {
#else

int main(int argc, char **argv) {
#endif

  scheme *sc;
  FILE *fin;
  char *file_name=InitFile;
  int retcode;
  int isfile=1;

  if(argc==1) {
    printf("%s\n", banner);
  }

  if(argc==2 && strcmp(argv[1],"-?")==0) {
    printf("Usage: tinyscheme -?\n");
    printf("or:    tinyscheme [<file1> <file2> ...]\n");
    printf("followed by\n");
    printf("          -1 <file> [<arg1> <arg2> ...]\n");
    printf("          -c <Scheme commands> [<arg1> <arg2> ...]\n");
    printf("assuming that the executable is named tinyscheme.\n");
    printf("Use - as filename for stdin.\n");
    return 1;
  }

  printf("initializing scheme environment\n");
  if(!scheme_init(&sc)) {
    fprintf(stderr,"Could not initialize!\n");
    return 2;
  }

  printf("setting standard ports\n");
  scheme_set_input_port_file(sc, stdin);
  printf("setting standard ports\n");
  scheme_set_output_port_file(sc, stdout);

#if USE_DL
  printf("defining extension loader\n");
  scheme_define(sc,scheme_global_env(sc),mk_symbol(sc,"load-extension"),mk_foreign_func(sc, scm_load_ext));
#endif

  argv++;
  if(access(file_name,0)!=0) {
    char *p=getenv("TINYSCHEMEINIT");
    if(p!=0) {
      file_name=p;
    }
  }
  do {
    if(strcmp(file_name,"-")==0) {
      fin=stdin;
    } else if(strcmp(file_name,"-1")==0 || strcmp(file_name,"-c")==0) {
      pointer args=scheme_nil(sc);
      isfile=file_name[1]=='1';
      file_name=*argv++;
      if(strcmp(file_name,"-")==0) {
        fin=stdin;
      } else if(isfile) {
        fin=fopen(file_name,"r");
      }
      for(;*argv;argv++) {
        pointer value=mk_string(sc,*argv);
        args=cons(sc,value,args);
      }
      args=scheme_reverse_in_place(sc,scheme_nil(sc),args);
      scheme_define(sc,scheme_global_env(sc),mk_symbol(sc,"*args*"),args);

    } else {
      fin=fopen(file_name,"r");
    }
    if(isfile && fin==0) {
      fprintf(stderr,"Could not open file %s\n",file_name);
    } else {
      if(isfile) {
        scheme_load_named_file(sc,fin,file_name);
      } else {
        scheme_load_string(sc,file_name);
      }
      if(!isfile || fin!=stdin) {
        if(scheme_retcode(sc) != 0) {
          fprintf(stderr,"Errors encountered reading %s\n",file_name);
        }
        if(isfile) {
          fclose(fin);
        }
      }
    }
    file_name=*argv++;
  } while(file_name!=0);
  if(argc==1) {
    scheme_load_named_file(sc,stdin,0);
  }
  retcode=scheme_retcode(sc);
  scheme_deinit(sc);

  return retcode;
}
