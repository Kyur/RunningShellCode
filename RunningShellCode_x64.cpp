#include <stdio.h>
#include <windows.h>

#define ERROR -1
#define GOOD 1

typedef void (*FNPTR)();

int main(int argc, char** argv)
{
	char* shellCodeFileName;
	HANDLE hShellCode = NULL;
	unsigned int fileSize = 0;
	PVOID pAllocMem = NULL;
	DWORD NumberOfBytesRead = 0;
	FNPTR fPtr = NULL;

	if(argc != 2)
	{
		printf(" [!] Need argument.\n");
		return ERROR;
	}

	shellCodeFileName = argv[1];
	hShellCode = CreateFile(shellCodeFileName, GENERIC_ALL, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(!hShellCode)
	{
		printf(" [!] Fail CreateFile\n");
		return ERROR;
	}

	fileSize = GetFileSize(hShellCode, NULL);
	if(!fileSize)
	{
		printf(" [!] File size error\n");
		return ERROR;
	}

	pAllocMem = VirtualAlloc(NULL, fileSize + 0x1000, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if(!pAllocMem)
	{
		printf(" [!] VirtualAlloc ERROR.\n");
		return ERROR;
	}

	ReadFile(hShellCode, pAllocMem, fileSize, &NumberOfBytesRead, NULL);
	
	fPtr = (FNPTR) pAllocMem;
	OutputDebugString("===== INTO HERE ====");
	fPtr();

	VirtualFree(pAllocMem, fileSize + 0x1000, MEM_RELEASE);
	CloseHandle(hShellCode);
	
	return GOOD;
}