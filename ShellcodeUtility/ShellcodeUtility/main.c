
#include "NasmAPI.h"

#include <Windows.h>
#include <stdio.h>




int main()
{

	if (!EnsureNASMExists())
	{
		printf("error: can't find or create nasm\n");
		return EXIT_FAILURE;
	}
	printf("found nasm\n");
	return EXIT_SUCCESS;
}