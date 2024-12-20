#include "gmptest.h"
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <ctime>

void init_rand(mpz_t* array, size_t size, mp_bitcnt_t bits, gmp_randstate_t rstate)
{
	for (size_t i = 0; i < size; i++)
	{
		mpz_init(array[i]);
		mpz_rrandomb(array[i], rstate, bits);
	}
}

void init_rand(mpf_t* array, size_t size, mp_bitcnt_t bits, gmp_randstate_t rstate)
{
	mpf_set_default_prec(10);
	for (size_t i = 0; i < size; i++)
	{
		mpf_init(array[i]);
		mpf_urandomb(array[i], rstate, bits);
	}
}

void clear_array(mpz_t* array, size_t size)
{
	for (size_t i = 0; i < size; i++)
		mpz_clear(array[i]);
}

void clear_array(mpf_t* array, size_t size)
{
	for (size_t i = 0; i < size; i++)
		mpf_clear(array[i]);
}

double gmp_test(mpz_t* array, size_t size, bmpz_func_t func)
{
	mpz_t result;
	mpz_init(result);

	clock_t clbeg = clock();
	for (size_t i = 0; i < size; i++)
		for (size_t j = 0; j < size; j++)
			func(result, array[i], array[j]);
	clock_t clend = clock();
	mpz_clear(result);

	return (clend - clbeg) / (double)CLOCKS_PER_SEC;
}

double gmp_test(mpz_t* array, size_t size, bmpz_ui_func_t func, mpir_ui op)
{
	mpz_t result;
	mpz_init(result);

	clock_t clbeg = clock();
	for (size_t i = 0; i < size; i++)
		func(result, array[i], op);		
	clock_t clend = clock();
	mpz_clear(result);

	return (clend - clbeg) / (double)CLOCKS_PER_SEC;
}

double gmp_test(mpf_t* array, size_t size, bmpf_func_t func)
{
	mpf_t result;
	mpf_init(result);

	clock_t clbeg = clock();
	for (size_t i = 0; i < size; i++)
		for (size_t j = 0; j < size; j++)
			func(result, array[i], array[j]);
	clock_t clend = clock();
	mpf_clear(result);

	return (clend - clbeg) / (double)CLOCKS_PER_SEC;
}

double gmp_test(mpf_t* array, size_t size, bmpf_ui_func_t func, mpir_ui op)
{
	mpf_t result;
	mpf_init(result);

	clock_t clbeg = clock();
	for (size_t i = 0; i < size; i++)
		func(result, array[i], op);
	clock_t clend = clock();
	mpf_clear(result);

	return (clend - clbeg) / (double)CLOCKS_PER_SEC;
}

double gmp_test(mpf_t* array, size_t size)
{
	mpf_t result;
	mpf_init(result);

	clock_t clbeg = clock();
	for (size_t i = 0; i < size; i++)
		mpf_sqrt(result, array[i]);
	clock_t clend = clock();
	mpf_clear(result);

	return (clend - clbeg) / (double)CLOCKS_PER_SEC;
}

void inttest(FILE* fout, mpz_t* array, size_t size, mp_bitcnt_t bits, gmp_randstate_t rstate)
{
	mpir_ui argui(POWM_DEFAULT);
	fprintf_s(fout, "TEST: int\nSIZE: %zu\nBITS: %zu\nRESULTS: {\n", size, bits);

	init_rand(array, size, bits, rstate);
	fprintf_s(fout, "   add: %.3fs\n", gmp_test(array, size, mpz_add));
	fprintf_s(fout, "   mul: %.3fs\n", gmp_test(array, size, mpz_mul));
	fprintf_s(fout, "   div: %.3fs\n", gmp_test(array, size, mpz_div));
	fprintf_s(fout, "   powui: %.3fs\n", gmp_test(array, size, mpz_pow_ui, argui));
	fprintf_s(fout, "   rootui: %.3fs\n}\n", gmp_test(array, size, mpz_root_ui, argui));
	clear_array(array, size);
}

void floattest(FILE* fout, mpf_t* array, size_t size, mp_bitcnt_t bits, gmp_randstate_t rstate)
{
	mpir_ui argui(POWM_DEFAULT);
	fprintf_s(fout, "TEST: float\nSIZE: %zu\nRESULTS: {\n", size);

	init_rand(array, size, bits, rstate);
	fprintf_s(fout, "   add: %.3fs\n", gmp_test(array, size, mpf_add));
	fprintf_s(fout, "   mul: %.3fs\n", gmp_test(array, size, mpf_mul));
	fprintf_s(fout, "   div: %.3fs\n", gmp_test(array, size, mpf_div));
	fprintf_s(fout, "   powui: %.3fs\n", gmp_test(array, size, mpf_pow_ui, argui));
	fprintf_s(fout, "   sqrt: %.3fs\n}\n", gmp_test(array, size));
	clear_array(array, size);
}

void gmp_testprogram(FILE* fout, size_t size)
{
	gmp_randstate_t rstate;
	gmp_randinit_default(rstate);

	mpz_t* zarray = new mpz_t[size];
	mpf_t* farray = new mpf_t[size];

	inttest(fout, zarray, size, 128U, rstate);
	inttest(fout, zarray, size, 256U, rstate);
	inttest(fout, zarray, size, 512U, rstate);
	inttest(fout, zarray, size, 1024U, rstate);

	floattest(fout, farray, size, 1024U, rstate);

	delete[] zarray;
	delete[] farray;
}