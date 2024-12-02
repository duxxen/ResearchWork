#include "gmptest.h"

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
	mpf_set_default_prec(bits);
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

double bin_gmptest(mpz_t* array, size_t size, bin_funcz_t func)
{
	mpz_t tempres;
	mpz_init(tempres);

	clock_t clbeg = clock();
	for (size_t i = 0; i < size; i++)
		for (size_t j = 0; j < size; j++)
			func(tempres, array[i], array[j]);
	clock_t clend = clock();

	mpz_clear(tempres);
	return (clend - clbeg) / (double)CLOCKS_PER_SEC;
}

double bin_gmptest(mpf_t* array, size_t size, bin_funcf_t func)
{
	mpf_t tempres;
	mpf_init(tempres);

	clock_t clbeg = clock();
	for (size_t i = 0; i < size; i++)
		for (size_t j = 0; j < size; j++)
			func(tempres, array[i], array[j]);
	clock_t clend = clock();

	mpf_clear(tempres);
	return (clend - clbeg) / (double)CLOCKS_PER_SEC;
}

double un_gmptest(mpz_t* array, size_t size, un_funcz_t func)
{
	mpz_t tempres;
	mpz_init(tempres);

	clock_t clbeg = clock();
	for (size_t i = 0; i < size; i++)
			func(tempres, array[i]);
	clock_t clend = clock();

	mpz_clear(tempres);
	return (clend - clbeg) / (double)CLOCKS_PER_SEC;
}

double un_gmptest(mpf_t* array, size_t size, un_funcf_t func)
{
	mpf_t tempres;
	mpf_init(tempres);

	clock_t clbeg = clock();
	for (size_t i = 0; i < size; i++)
			func(tempres, array[i]);
	clock_t clend = clock();

	mpf_clear(tempres);
	return (clend - clbeg) / (double)CLOCKS_PER_SEC;
}

void runtest(FILE* fout, mpz_t* array, size_t size, mp_bitcnt_t bits, gmp_randstate_t rstate)
{
	fprintf_s(fout, "TEST: int\nSIZE: %zu\nBITS: %zu\nRESULTS: {\n", size, bits);
	init_rand(array, size, bits, rstate);
	for (size_t i = 0; i < bin_funcs_count; i++)
	{
		double duration = bin_gmptest(array, size, bin_funcsz[i]);
		fprintf_s(fout, "    %s: %.3fs\n", bin_func_names[i], duration);
	}
	for (size_t i = 0; i < un_funcs_count; i++)
	{
		double duration = un_gmptest(array, size, un_funcsz[i]);
		fprintf_s(fout, "    %s: %.3fs\n", un_func_names[i], duration);
	}
	clear_array(array, size);
	fprintf_s(fout, "}\n");
}

void runtest(FILE* fout, mpf_t* array, size_t size, mp_bitcnt_t bits, gmp_randstate_t rstate)
{
	fprintf_s(fout, "TEST: float\nSIZE: %zu\nBITS: %zu\nRESULTS: {\n", size, bits);
	init_rand(array, size, bits, rstate);
	for (size_t i = 0; i < bin_funcs_count - 2; i++)
	{
		double duration = bin_gmptest(array, size, bin_funcsf[i]);
		fprintf_s(fout, "    %s: %.3fs\n", bin_func_names[i], duration);
	}
	for (size_t i = 0; i < un_funcs_count; i++)
	{
		double duration = un_gmptest(array, size, un_funcsf[i]);
		fprintf_s(fout, "    %s: %.3fs\n", un_func_names[i], duration);
	}
	clear_array(array, size);
	fprintf_s(fout, "}\n");
}

void testprogram(FILE* fout, size_t arrsize)
{
	gmp_randstate_t rstate;
	gmp_randinit_default(rstate);

	mpz_t* zarray = new mpz_t[arrsize];
	mpf_t* farray = new mpf_t[arrsize];

	runtest(fout, zarray, arrsize, 128U, rstate);
	runtest(fout, zarray, arrsize, 256U, rstate);
	runtest(fout, zarray, arrsize, 512U, rstate);
	runtest(fout, zarray, arrsize, 1024U, rstate);

	runtest(fout, farray, arrsize, 128U, rstate);
	runtest(fout, farray, arrsize, 256U, rstate);
	runtest(fout, farray, arrsize, 512U, rstate);
	runtest(fout, farray, arrsize, 1024U, rstate);
	
	delete[] zarray;
	delete[] farray;
}
