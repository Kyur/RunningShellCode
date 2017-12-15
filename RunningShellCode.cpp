#include <stdio.h>
#include <windows.h>

//	Function pointer for shell code
typedef void (*FNPTR)();

int main(int argc, char** argv)
{
	char* shellCodeFileName;
	HANDLE hShellCode = NULL;
	unsigned int fileSize = 0;
	unsigned int realAllocSize = 0;
	PVOID pAllocMem = NULL;
	DWORD NumberOfBytesRead = 0;
	FNPTR fPtr = NULL;

	if(argc != 2)
	{
		printf(" [!] Usage : RunningShellCode_x64.exe ShellCodeFile\n");
		return -1;
	}

	shellCodeFileName = argv[1];
	hShellCode = CreateFile(shellCodeFileName, GENERIC_ALL, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(!hShellCode)
	{
		printf(" [!] Fail : Fail to CreateFile().\n");
		return -1;
	}

	fileSize = GetFileSize(hShellCode, NULL);
	if(fileSize <= 0)
	{
		printf(" [!] Fail : Abnormal file size.\n");
		return -1;
	}

	//	+ 0x1000 Extra size to allocate memory.
	realAllocSize = fileSize + 0x1000;

	pAllocMem = (PVOID)VirtualAlloc(NULL, realAllocSize, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if(!pAllocMem)
	{
		printf(" [!] Fail : Fail to VirtualAlloc().\n");
		return -1;
	}

	//	Set 0x90 to memory area.
	memset(pAllocMem, 0x90, realAllocSize);
	//	Set 0xCC to memory start.
	memset(pAllocMem, 0xCC, 0x01);

	//	Read shell code to allocated memory.
	if(!ReadFile(hShellCode, ((PBYTE)pAllocMem + 1), fileSize, &NumberOfBytesRead, NULL))
	{
		printf(" [!] Fail : Fail to ReadFile().\n");
		return -1;
	}
	
	fPtr = (FNPTR)pAllocMem;

	//	Call allocated memory area.
	fPtr();

	VirtualFree(pAllocMem, realAllocSize, MEM_RELEASE);
	CloseHandle(hShellCode);
	
	return 1;
}

