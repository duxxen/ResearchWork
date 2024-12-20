#include "common.h"

#define PATH_FLAG 'p'
#define SIZE_FLAG 'n'
#define POWM_FLAG 'w'

void fatal_exit(int code)
{
	fprintf_s(stderr, "Fatal exit code: %d\n", code);
	exit(code);
}

int main(int argc, char* argv[])
{
	FILE* fout = stdout;
	size_t arrsize = 1000;

	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
			switch (argv[i][1])
			{
			case PATH_FLAG:
				fout = fopen(argv[i + 1], "w+");
				if (fout == NULL) {
					fprintf_s(stderr, "Cannot open the file!\n");
					fatal_exit(1);
				}
				break;

			case SIZE_FLAG:
				arrsize = strtoull(argv[i + 1], NULL, 10);
				break;

			case POWM_FLAG:
				POWM_DEFAULT = strtoull(argv[i + 1], NULL, 10);
				break;

			default:
				fprintf_s(stderr, "Undefined flag \"%c\"!\n", argv[i][1]);
				fatal_exit(2);
				break;
			}
	}

	clock_t global_clbeg = clock();

	#if IS_GMP_TEST
	fprintf_s(fout, "GMP Lib Test Prorgam\n\n");
	gmp_testprogram(fout, arrsize);
	#else
	fprintf_s(fout, "NTL Lib Test Prorgam\n\n");
	ntl_testprogram(fout, arrsize);
	#endif

	clock_t global_clend = clock();
	double elapsed = (global_clend - global_clbeg) / (double)CLOCKS_PER_SEC;
	fprintf_s(fout, "\nTest time: %.3fs\n", elapsed);
	
	if (fout != stdout && fout != NULL)
		fclose(fout);

	return 0;
}