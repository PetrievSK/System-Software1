#define WINVER 0x0A00

#include <iostream>
#include <windows.h>
#include <stdio.h>

using namespace std;

void Punct1()
{
    OSVERSIONINFO osvi;
    BOOL bIsWindowsXPorLater;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&osvi);
}

int main()
{
    //Пункт 1.1
    OSVERSIONINFO osvi;
    BOOL bIsWindowsXPorLater;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&osvi);

    cout<<"1.1. Operation Sistem Version: "<<osvi.dwMajorVersion<<"."<<osvi.dwMinorVersion<<endl;
    cout<<"Build: "<<osvi.dwBuildNumber<<endl;

    //Пункт 1.2
    int INFO_BUFFER_SIZE=256;
    TCHAR infoBuf[INFO_BUFFER_SIZE];
    DWORD  bufCharCount = INFO_BUFFER_SIZE;
    GetSystemDirectoryA(infoBuf, INFO_BUFFER_SIZE);
    printf("\n1.2.  System directory: %s\n", infoBuf);

    //Пункт 1.3
    char lpBuffer[INFO_BUFFER_SIZE];
    DWORD pcbBuffer = INFO_BUFFER_SIZE;
    GetUserNameA(lpBuffer, &pcbBuffer);
    printf("\n1.3 User Name: %s", lpBuffer);
    GetComputerNameA(lpBuffer,&pcbBuffer);
    printf("      Device Name: %s", lpBuffer);

    //Пункт 1.4
    char buffer[MAX_PATH*4];
    char PathName[MAX_PATH];
    DWORD lpcchReturnLength = MAX_PATH*4;
    _ULARGE_INTEGER total, available, free;

    HANDLE search = FindFirstVolumeA(buffer, sizeof(buffer));
    printf("\n1.4");

    do {
    FindNextVolumeA(search, buffer, sizeof(buffer));
    GetVolumePathNamesForVolumeNameA(buffer, PathName, sizeof(buffer), &lpcchReturnLength);
    GetDiskFreeSpaceExA(buffer, &total, &available, &free);
    printf("\nService Name: %s", buffer);
    printf("\nPath Name: %s", PathName);
    printf("\nDisk Available space: %llu\nVolume disk space: %llu\n", available, total);

    }
    while (FindNextVolume(search, buffer, sizeof(buffer)));

    if (GetLastError() != ERROR_NO_MORE_FILES) {
        printf("\nA system error has occurred");
    }
    FindVolumeClose(search);

    //Пункт 1.5
    HKEY hKey;
    LPCSTR lpSubKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    DWORD  ulOptions;
    REGSAM samDesired;
    PHKEY  phkResult;
    DWORD count = 0;

    char Progbuffer[MAX_PATH*4];
    DWORD Pblen = 32767;

    RegOpenKeyExA(HKEY_CURRENT_USER ,lpSubKey, 0, KEY_ALL_ACCESS, &hKey);

    while(RegEnumValueA(hKey, count, Progbuffer, &Pblen, 0, NULL, NULL, NULL)==ERROR_SUCCESS)
    {
        RegEnumValueA(hKey, count, Progbuffer, &Pblen, 0, NULL, NULL, NULL);
        printf("\n%s", Progbuffer);
        count++;
    }

    //Пункт 2.1
    LARGE_INTEGER Freq;
    QueryPerformanceFrequency(&Freq);
    cout<<"\nCPU frequency : "<<Freq.QuadPart<<" HZ";

    //Пункт 2.2
    LARGE_INTEGER t0,t;
    QueryPerformanceCounter(&t0);
    Punct1();
    QueryPerformanceCounter(&t);
    double ticks = t.QuadPart - t0.QuadPart;
    double ticks_per_sec = Freq.QuadPart;
    double usec_per_sec = 1e6;
    double usec = usec_per_sec * ticks / ticks_per_sec;
    printf("\nDuration point 1.1 complete=%f.3 usec\n", usec);

    return 0;
}
