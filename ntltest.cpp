#include "ntltest.h"

void init_rand(ZZ* array, size_t size, long bits)
{
	for (int i = 0; i < size; i++) 
	{
		array[i] = RandomBits_ZZ(bits);
	}
}

void init_rand(RR* array, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		array[i] = random_RR() * 100000000;
	}
}

double ntl_test(ZZ* array, size_t size, op_t f)
{
	ZZ result;

	clock_t clbeg = clock();
	for (size_t i = 0; i < size; i++)
		for (size_t j = 0; j < size; j++)
			switch (f)
			{
			case op_t::add:
				result = array[i] + array[j];
				break;
			case op_t::mul:
				result = array[i] * array[j];
				break;
			case op_t::div:
				result = array[i] / array[j];
				break;

			default:
				break;
			}
	clock_t clend = clock();
	return (clend - clbeg) / (double)CLOCKS_PER_SEC;
}

double ntl_test(ZZ* array, size_t size, op_t f, long op)
{
	ZZ result;

	clock_t clbeg = clock();
	for (size_t i = 0; i < size; i++)
		switch (f)
		{
		case op_t::pow:
			result = power(array[i], op);
			break;
		case op_t::root:
			result = SqrRoot(array[i]);
			break;
		default:
			break;
		}
	clock_t clend = clock();
	return (clend - clbeg) / (double)CLOCKS_PER_SEC;
}

double ntl_test(RR* array, size_t size, op_t f)
{
	RR result;

	clock_t clbeg = clock();
	for (size_t i = 0; i < size; i++)
		for (size_t j = 0; j < size; j++)
			switch (f)
			{
			case op_t::add:
				result = array[i] + array[j];
				break;
			case op_t::mul:
				result = array[i] * array[j];
				break;
			case op_t::div:
				result = array[i] / array[j];
				break;

			default:
				break;
			}
	clock_t clend = clock();
	return (clend - clbeg) / (double)CLOCKS_PER_SEC;
}

double ntl_test(RR* array, size_t size, op_t f, long op)
{
	RR result;

	clock_t clbeg = clock();
	for (size_t i = 0; i < size; i++)
		switch (f)
		{
		case op_t::pow:
			result = power(array[i], op);
			break;
		case op_t::root:
			result = SqrRoot(array[i]);
			break;
		default:
			break;
		}
	clock_t clend = clock();
	return (clend - clbeg) / (double)CLOCKS_PER_SEC;
}

void inttest(FILE* fout, ZZ* array, size_t size, long bits)
{
	fprintf_s(fout, "TEST: int\nSIZE: %zu\nBITS: %d\nRESULTS: {\n", size, bits);

	init_rand(array, size, bits);
	fprintf_s(fout, "   add: %.3fs\n", ntl_test(array, size, op_t::add));
	fprintf_s(fout, "   mul: %.3fs\n", ntl_test(array, size, op_t::mul));
	fprintf_s(fout, "   div: %.3fs\n", ntl_test(array, size, op_t::div));
	fprintf_s(fout, "   powui: %.3fs\n", ntl_test(array, size, op_t::pow));
	fprintf_s(fout, "   sqrt: %.3fs\n}\n", ntl_test(array, size, op_t::root));
}

void floattest(FILE* fout, RR* array, size_t size)
{
	fprintf_s(fout, "TEST: float\nSIZE: %zu\nRESULTS: {\n", size);

	init_rand(array, size);
	fprintf_s(fout, "   add: %.3fs\n", ntl_test(array, size, op_t::add));
	fprintf_s(fout, "   mul: %.3fs\n", ntl_test(array, size, op_t::mul));
	fprintf_s(fout, "   div: %.3fs\n", ntl_test(array, size, op_t::div));
	fprintf_s(fout, "   powui: %.3fs\n", ntl_test(array, size, op_t::pow));
	fprintf_s(fout, "   sqrt: %.3fs\n}\n", ntl_test(array, size, op_t::root));
}

void ntl_testprogram(FILE* fout, size_t size)
{
	ZZ* zarray = new ZZ[size];
	RR* farray = new RR[size];

	inttest(fout, zarray, size, 128);
	inttest(fout, zarray, size, 256);
	inttest(fout, zarray, size, 512);
	inttest(fout, zarray, size, 1024);

	floattest(fout, farray, size);

	delete[] zarray;
	delete[] farray;
}