#pragma once
#include <gmp.h>

static void mpz_root_ui(mpz_ptr rop, mpz_srcptr op, mpir_ui n) { mpz_root(rop, op, n); }
static mpir_ui POWM_DEFAULT = 2U;

typedef void (*bmpz_func_t)(mpz_ptr, mpz_srcptr, mpz_srcptr);
typedef void (*bmpz_ui_func_t)(mpz_ptr, mpz_srcptr, mpir_ui);

typedef void (*bmpf_func_t)(mpf_ptr, mpf_srcptr, mpf_srcptr);
typedef void (*bmpf_ui_func_t)(mpf_ptr, mpf_srcptr, mpir_ui);

void init_rand(mpz_t* array, size_t size, mp_bitcnt_t bits, gmp_randstate_t rstate);
void init_rand(mpf_t* array, size_t size, mp_bitcnt_t bits, gmp_randstate_t rstate);

void clear_array(mpz_t* array, size_t size);
void clear_array(mpf_t* array, size_t size);

double gmp_test(mpz_t* array, size_t size, bmpz_func_t func);
double gmp_test(mpz_t* array, size_t size, bmpz_ui_func_t func, mpir_ui op);

double gmp_test(mpf_t* array, size_t size, bmpf_func_t func);
double gmp_test(mpf_t* array, size_t size, bmpf_ui_func_t func, mpir_ui op);
double gmp_test(mpf_t* array, size_t size); // sqrt

void inttest(FILE* fout, mpz_t* array, size_t size, mp_bitcnt_t bits, gmp_randstate_t rstate);
void floattest(FILE* fout, mpf_t* array, size_t size, mp_bitcnt_t bits, gmp_randstate_t rstate);

void gmp_testprogram(FILE* fout, size_t size);