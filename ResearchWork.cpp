#include "gmptest.h"

int main(int argc, char* argv[])
{
	FILE* fout = stdout;
	size_t arrsize = 1000;

	switch (argc)
	{
	case 3:
		fout = fopen(argv[2], "w");
		if (fout == NULL) {
			printf_s("Cannot open the file (%s)", argv[0]);
			return 1;
		}
	case 2:
		arrsize = strtoull(argv[1], NULL, 10);
		break;
	default:
		break;
	}

	testprogram(fout, arrsize);
	if (fout != stdout)
		fclose(fout);

	return 0;
}