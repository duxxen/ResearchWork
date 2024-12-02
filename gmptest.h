#pragma once
#include <gmp.h>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <ctime>

typedef void (*bin_funcz_t)(mpz_ptr, mpz_srcptr, mpz_srcptr);
typedef void (*bin_funcf_t)(mpf_ptr, mpf_srcptr, mpf_srcptr);

typedef void (*un_funcz_t)(mpz_ptr, mpz_srcptr);
typedef void (*un_funcf_t)(mpf_ptr, mpf_srcptr);

static const bin_funcz_t bin_funcsz[] = {
	mpz_add,
	mpz_mul,
	mpz_div,
	mpz_and,
	mpz_xor
};

static const bin_funcf_t bin_funcsf[] = {
	mpf_add,
	mpf_mul,
	mpf_div
};

static const un_funcf_t un_funcsf[] = {
	mpf_abs,
	mpf_neg
};

static const un_funcz_t un_funcsz[] = {
	mpz_abs,
	mpz_neg
};

static const char* bin_func_names[] = {
	"add",
	"mul",
	"div",
	"and",
	"xor",
};

static const char* un_func_names[] = {
	"abs",
	"neg"
};

static constexpr size_t un_funcs_count = sizeof(un_funcsz) / sizeof(un_funcz_t);
static constexpr size_t bin_funcs_count = sizeof(bin_funcsz) / sizeof(bin_funcz_t);

void init_rand(mpz_t* array, size_t size, mp_bitcnt_t bits, gmp_randstate_t rstate);
void init_rand(mpf_t* array, size_t size, mp_bitcnt_t bits, gmp_randstate_t rstate);

void clear_array(mpz_t* array, size_t size);
void clear_array(mpf_t* array, size_t size);

double bin_gmptest(mpz_t* array, size_t size, bin_funcz_t func);
double bin_gmptest(mpf_t* array, size_t size, bin_funcf_t func);

double un_gmptest(mpz_t* array, size_t size, un_funcz_t func);
double un_gmptest(mpf_t* array, size_t size, un_funcf_t func);

void runtest(FILE* fout, mpz_t* zarray, size_t arrsize, mp_bitcnt_t bits, gmp_randstate_t rstate);
void runtest(FILE* fout, mpf_t* farray, size_t arrsize, mp_bitcnt_t bits, gmp_randstate_t rstate);
void testprogram(FILE* fout, size_t arrsize);