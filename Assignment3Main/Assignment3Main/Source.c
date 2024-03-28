#include <stdio.h>
#include <windows.h>

typedef void (*sayHello)(void);
typedef void (*sleepFun)(int);

int main(void) {
	HINSTANCE assignmentDLL = LoadLibrary(L"C:\\Users\\massi\\Documents\\csec201\\Assignment3DLL\\x64\\Debug\\Assignment3DLL.dll");
	
	sayHello dllFunction;
	dllFunction = GetProcAddress(assignmentDLL, "sayHello");
	dllFunction();

	sleepFun sleepFunc;
	sleepFunc = GetProcAddress(assignmentDLL, "sleepFun");
	sleepFunc(5);

}