#pragma once
#pragma warning(disable:4146)
#include <NTL/ZZ.h>
#include <NTL/RR.h>

using namespace NTL;

static uint64_t POWM_DEFAULT = 2U;
enum class op_t
{
	add,
	mul,
	div,
	pow,
	root
};

void init_rand(ZZ* array, size_t size, long bits);
void init_rand(RR* array, size_t size);

double ntl_test(ZZ* array, size_t size, op_t f);
double ntl_test(ZZ* array, size_t size, op_t f, long op);
double ntl_test(RR* array, size_t size, op_t f);
double ntl_test(RR* array, size_t size, op_t f, long op);

void inttest(FILE* fout, ZZ* array, size_t size, long bits);
void floattest(FILE* fout, RR* array, size_t size);

void ntl_testprogram(FILE* fout, size_t size);