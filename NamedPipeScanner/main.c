#include <stdio.h>
#include <string.h>  /* Added for memset */
#include "beacon.h"
#include <windows.h>
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#define MAX_PATH 260  /* Fixed */

DECLSPEC_IMPORT void* __cdecl MSVCRT$memset(void* dest, int ch, size_t count);  /* Added */
DECLSPEC_IMPORT WINBASEAPI HANDLE WINAPI KERNEL32$FindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);
DECLSPEC_IMPORT WINBASEAPI BOOL WINAPI KERNEL32$FindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData);  /* Added */
DECLSPEC_IMPORT WINBASEAPI BOOL WINAPI KERNEL32$FindClose(HANDLE hFindFile);  /* Added */
DECLSPEC_IMPORT WINBASEAPI DWORD WINAPI KERNEL32$GetLastError(void);

void findNamedPipe() {
    WIN32_FIND_DATAA findData;
    HANDLE hFind;
    const char *target = "\\\\.\\pipe\\*";

    MSVCRT$memset(&findData, 0, sizeof(findData));  /* Use resolved memset */

    hFind = KERNEL32$FindFirstFileA(target, &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        DWORD err = KERNEL32$GetLastError();
        BeaconPrintf(CALLBACK_OUTPUT, "FindFirstFileA failed, GetLastError=%lu", (unsigned long)err);
        return;
    }

    do {
        BeaconPrintf(CALLBACK_OUTPUT, "Named pipe: %s\n", findData.cFileName);
    } while (KERNEL32$FindNextFileA(hFind, &findData));

    DWORD err = KERNEL32$GetLastError();
    if (err != ERROR_NO_MORE_FILES) {  /* Simplified */
        BeaconPrintf(CALLBACK_OUTPUT, "FindNextFileA ended with error=%lu", (unsigned long)err);
    }

    KERNEL32$FindClose(hFind);
}

void go(char *args, int alen) {
    BeaconPrintf(CALLBACK_OUTPUT, "Finding named pipes on the system: %s \n", args ? args : "");
    findNamedPipe();  
}
