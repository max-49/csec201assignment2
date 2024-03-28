// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <stdio.h>
#include <process.h>
#include <tlhelp32.h>

extern "C" __declspec(dllexport) void sayHello(void) {
    int helloBox = MessageBox(
        NULL,
        TEXT("Hello World"),
        TEXT("Message from sayHello"),
        MB_OK
    );
}

extern "C" __declspec(dllexport) void sleepFun(int duration) {

    char timeoutCmd[50];

    snprintf(timeoutCmd, 50, "/c timeout %d", duration);

    STARTUPINFO startInfo = { sizeof(startInfo) };
    PROCESS_INFORMATION procInfo;

    if (CreateProcessA((LPCSTR)"C:\\Windows\\System32\\cmd.exe", (LPSTR)timeoutCmd, NULL, NULL, FALSE, 0, NULL, NULL, (LPSTARTUPINFOA)&startInfo, &procInfo)) {
        
        int processID = _getpid();
        printf("sleepFun Process ID: %d\n", processID);

        DWORD timeoutProcessID = procInfo.dwProcessId;
        printf("Timeout Command Process ID: %d\n", timeoutProcessID);
        
        DWORD parentPID = 0;
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (snapshot != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32 processEntry;
            processEntry.dwSize = sizeof(PROCESSENTRY32);

            if (Process32First(snapshot, &processEntry)) {
                do {
                    if (processEntry.th32ProcessID == timeoutProcessID) {
                        parentPID = processEntry.th32ParentProcessID;
                        break;
                    }
                } while (Process32Next(snapshot, &processEntry));
            }

            CloseHandle(snapshot);
        }

        printf("Timeout Command Parent Process PID: %d\n", parentPID);

        CloseHandle(procInfo.hProcess);
        CloseHandle(procInfo.hThread);
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
        printf("DLL process attached\n");
        break;
    case DLL_THREAD_ATTACH:
        printf("DLL thread attached\n");
        break;
    case DLL_THREAD_DETACH:
        printf("DLL thread detached\n");
        break;
    case DLL_PROCESS_DETACH:
        printf("DLL process detached\n");
        break;
    }
    return TRUE;
}

