
#include "NasmAPI.h"
#include <Windows.h>
#include <stdio.h>
#include "resource.h"

// from https://stackoverflow.com/questions/6218325/how-do-you-check-if-a-directory-exists-on-windows-in-c
BOOL FileExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributesA(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

BOOL ExtractNasm()
{
	BOOL bSuccess = FALSE;
	HRSRC hResource = NULL;
	HANDLE hNasmExeFile = INVALID_HANDLE_VALUE;

	hResource = FindResource(NULL, MAKEINTRESOURCEA(IDR_PE2), "PE");
	if (!hResource)
	{
		printf("failed FindResource nasm.exe resource %d\n", GetLastError());
		goto cleanup;
	}

	HGLOBAL hMemory = LoadResource(NULL, hResource);
	if (!hMemory)
	{
		printf("failed LoadResource nasm.exe resource %d\n", GetLastError());
		goto cleanup;
	}

	DWORD dwNasmSize = SizeofResource(NULL, hResource);
	LPVOID lpNasm = LockResource(hMemory);
	if (!lpNasm)
	{
		printf("failed LockResource nasm.exe %d\n", GetLastError());
		goto cleanup;
	}

	hNasmExeFile = CreateFile(NASM_PATH, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hNasmExeFile)
	{
		printf("failed CreateFile nasm.exe %d\n", GetLastError());
		goto cleanup;
	}

	DWORD dwBytesWritten = 0;
	BOOL bResult = WriteFile(hNasmExeFile, lpNasm, dwNasmSize, &dwBytesWritten, NULL);
	if (!bResult || /*short write*/ dwBytesWritten != dwNasmSize)
	{
		printf("failed WriteFile nasm.exe %d\n", GetLastError());
		goto cleanup;
	}

	bSuccess = TRUE;
cleanup:
	if(hResource) FreeResource(hResource);
	if (INVALID_HANDLE_VALUE != hNasmExeFile) CloseHandle(hNasmExeFile);

	return bSuccess;
}

BOOL EnsureNASMExists()
{
	if (!FileExists(NASM_PATH))
	{
		return ExtractNasm();
	}
	return TRUE;
}