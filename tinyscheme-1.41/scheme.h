/* SCHEME.H */

#ifndef _SCHEME_H
#define _SCHEME_H

#include "config.h"

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct scheme scheme;
typedef struct cell *pointer;

typedef void * (*func_alloc)(size_t);
typedef void (*func_dealloc)(void *);

/* num, for generic arithmetic */
typedef struct num {
     char is_fixnum;
     union {
          long ivalue;
          double rvalue;
     } value;
} num;

SCHEME_EXPORT scheme *scheme_init_new();
SCHEME_EXPORT scheme *scheme_init_new_custom_alloc(func_alloc malloc, func_dealloc free);
SCHEME_EXPORT int scheme_init(scheme **sc);
SCHEME_EXPORT int scheme_init_custom_alloc(scheme **sc, func_alloc, func_dealloc);
SCHEME_EXPORT void scheme_deinit(scheme *sc);
SCHEME_EXPORT void scheme_set_input_port_file(scheme *sc, FILE *fin);
SCHEME_EXPORT void scheme_set_input_port_string(scheme *sc, char *start, char *past_the_end);
SCHEME_EXPORT void scheme_set_output_port_file(scheme *sc, FILE *fin);
SCHEME_EXPORT void scheme_set_output_port_string(scheme *sc, char *start, char *past_the_end);
SCHEME_EXPORT void scheme_load_file(scheme *sc, FILE *fin);
SCHEME_EXPORT void scheme_load_named_file(scheme *sc, FILE *fin, const char *filename);
SCHEME_EXPORT void scheme_load_string(scheme *sc, const char *cmd);
SCHEME_EXPORT pointer scheme_apply0(scheme *sc, const char *procname);
SCHEME_EXPORT pointer scheme_call(scheme *sc, pointer func, pointer args);
SCHEME_EXPORT pointer scheme_eval(scheme *sc, pointer obj);
SCHEME_EXPORT void scheme_set_external_data(scheme *sc, void *p);
SCHEME_EXPORT void scheme_define(scheme *sc, pointer env, pointer symbol, pointer value);

SCHEME_EXPORT int scheme_retcode(scheme *sc);
SCHEME_EXPORT pointer scheme_global_env(scheme *sc);
SCHEME_EXPORT pointer scheme_nil(scheme *sc);
SCHEME_EXPORT pointer scheme_true(scheme *sc);
SCHEME_EXPORT pointer scheme_false(scheme *sc);

typedef pointer (*foreign_func)(scheme *, pointer);

SCHEME_EXPORT pointer _cons(scheme *sc, pointer a, pointer b, int immutable);
SCHEME_EXPORT pointer mk_integer(scheme *sc, long num);
SCHEME_EXPORT pointer mk_real(scheme *sc, double num);
SCHEME_EXPORT pointer mk_symbol(scheme *sc, const char *name);
SCHEME_EXPORT pointer gensym(scheme *sc);
SCHEME_EXPORT pointer mk_string(scheme *sc, const char *str);
SCHEME_EXPORT pointer mk_counted_string(scheme *sc, const char *str, int len);
SCHEME_EXPORT pointer mk_empty_string(scheme *sc, int len, char fill);
SCHEME_EXPORT pointer mk_character(scheme *sc, int c);
SCHEME_EXPORT pointer mk_foreign_func(scheme *sc, foreign_func f);
SCHEME_EXPORT void putstr(scheme *sc, const char *s);
SCHEME_EXPORT int list_length(scheme *sc, pointer a);
SCHEME_EXPORT int eqv(pointer a, pointer b);

SCHEME_EXPORT pointer scheme_reverse(scheme *sc, pointer a);
SCHEME_EXPORT pointer scheme_reverse_in_place(scheme *sc, pointer term, pointer list);


#if USE_INTERFACE

struct scheme_interface {
  void (*scheme_define)(scheme *sc, pointer env, pointer symbol, pointer value);
  pointer (*cons)(scheme *sc, pointer a, pointer b);
  pointer (*immutable_cons)(scheme *sc, pointer a, pointer b);
  pointer (*reserve_cells)(scheme *sc, int n);
  pointer (*mk_integer)(scheme *sc, long num);
  pointer (*mk_real)(scheme *sc, double num);
  pointer (*mk_symbol)(scheme *sc, const char *name);
  pointer (*gensym)(scheme *sc);
  pointer (*mk_string)(scheme *sc, const char *str);
  pointer (*mk_counted_string)(scheme *sc, const char *str, int len);
  pointer (*mk_character)(scheme *sc, int c);
  pointer (*mk_vector)(scheme *sc, int len);
  pointer (*mk_foreign_func)(scheme *sc, foreign_func f);
  void (*putstr)(scheme *sc, const char *s);
  void (*putcharacter)(scheme *sc, int c);

  int (*is_string)(pointer p);
  char *(*string_value)(pointer p);
  int (*is_number)(pointer p);
  num (*nvalue)(pointer p);
  long (*ivalue)(pointer p);
  double (*rvalue)(pointer p);
  int (*is_integer)(pointer p);
  int (*is_real)(pointer p);
  int (*is_character)(pointer p);
  long (*charvalue)(pointer p);
  int (*is_list)(scheme *sc, pointer p);
  int (*is_vector)(pointer p);
  int (*list_length)(scheme *sc, pointer vec);
  long (*vector_length)(pointer vec);
  void (*fill_vector)(pointer vec, pointer elem);
  pointer (*vector_elem)(pointer vec, int ielem);
  pointer (*set_vector_elem)(pointer vec, int ielem, pointer newel);
  int (*is_port)(pointer p);

  int (*is_pair)(pointer p);
  pointer (*pair_car)(pointer p);
  pointer (*pair_cdr)(pointer p);
  pointer (*set_car)(pointer p, pointer q);
  pointer (*set_cdr)(pointer p, pointer q);

  int (*is_symbol)(pointer p);
  char *(*symname)(pointer p);

  int (*is_syntax)(pointer p);
  int (*is_proc)(pointer p);
  int (*is_foreign)(pointer p);
  char *(*syntaxname)(pointer p);
  int (*is_closure)(pointer p);
  int (*is_macro)(pointer p);
  pointer (*closure_code)(pointer p);
  pointer (*closure_env)(pointer p);

  int (*is_continuation)(pointer p);
  int (*is_promise)(pointer p);
  int (*is_environment)(pointer p);
  int (*is_immutable)(pointer p);
  void (*setimmutable)(pointer p);
  void (*load_file)(scheme *sc, FILE *fin);
  void (*load_string)(scheme *sc, const char *input);
};

typedef struct scheme_interface scheme_interface;

SCHEME_EXPORT void scheme_init_interface(scheme_interface *sci);

#endif

typedef struct scheme_registerable
{
  foreign_func  f;
  const char *  name;
} scheme_registerable;

void scheme_register_foreign_func_list(scheme * sc,
                                       scheme_registerable * list,
                                       int n);

#ifdef __cplusplus
}
#endif

#endif

