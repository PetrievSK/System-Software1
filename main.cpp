#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>


#define BUFSIZE MAX_PATH
#define FILESYSNAMEBUFSIZE MAX_PATH
#define INFO_BUFFER_SIZE 32767
#define MAX_KEY_LENGTH 255

using namespace std;

double PCFreq = 0.0;
__int64 CounterStart = 0;

void Freq()
{
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        cout << "Function QueryPerformanceFrequency() failed!\n";

    PCFreq = double(li.QuadPart);

    printf("\n2.1.  CPU frequency: %u  Hz\n", li);
    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double ((li.QuadPart - CounterStart)* 1000000) / PCFreq;
}
int main()
{
    OSVERSIONINFO osvi;
    BOOL bIsWindowsXPorLater;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&osvi);

    cout<<"1.1. Operation Sistem Version: "<<osvi.dwMajorVersion<<"."<<osvi.dwMinorVersion<<endl;
    cout<<"Build: "<<osvi.dwBuildNumber<<endl;

    TCHAR  infoBuf[INFO_BUFFER_SIZE];
    DWORD  bufCharCount = INFO_BUFFER_SIZE;
    GetSystemDirectory(infoBuf, INFO_BUFFER_SIZE);
    printf("\n1.2.  System directory: %s\n", infoBuf);

    bufCharCount = INFO_BUFFER_SIZE;
    GetComputerName(infoBuf, &bufCharCount);
    GetUserName(infoBuf, &bufCharCount);
    printf("\n1.3. Computer Name: %s", infoBuf);
    printf("\n User Name: %s", infoBuf);

    char buffer[MAX_PATH +1];
    DWORD  CBufLen = MAX_PATH;
    char Names[MAX_PATH +1];
    CHAR VolumeName[MAX_PATH];
    __int64 total, available, free;


    HANDLE search = FindFirstVolume(VolumeName, BUFSIZE);
    printf("\n %s", VolumeName);

    do {
        printf("\n      %s", Names);
        GetVolumePathNamesForVolumeName(Names, buffer, CBufLen, &CBufLen);
        char* path = buffer;
        printf("\n Ñurrent path: %s", path);
        GetDiskFreeSpaceEx(buffer, (PULARGE_INTEGER)&available, (PULARGE_INTEGER)&total, (PULARGE_INTEGER)&free);
        if (GetDiskFreeSpaceEx(buffer, (PULARGE_INTEGER)&available, (PULARGE_INTEGER)&total, (PULARGE_INTEGER)&free)==0) {
            printf("\n No information available.\n ");
        }
        else{
        printf("\n Total memory: %u  bytes ", total);
        printf("\n Free: %u  bytes\n", available);
        }

    } while (FindNextVolume(search, Names, BUFSIZE));
    FindVolumeClose(search);

    DWORD dwSize;
    TCHAR szName[MAX_KEY_LENGTH];
    HKEY hKey;
    DWORD dwIndex = 0;
    DWORD retCode;
    DWORD BufferSize = 8192;
    PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK)malloc(BufferSize);
    DWORD cbData = BufferSize;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        0, KEY_ALL_ACCESS, &hKey) == !ERROR_SUCCESS)
    {
        printf("Function RegOpenKeyEx() failed!\n");
    }
    else printf("\n1.5.  Startup commands:\n");

    while (1) {
        dwSize = sizeof(szName);
        retCode = RegEnumValue(hKey, dwIndex, szName, &dwSize, NULL, NULL, NULL, NULL);
        if (retCode == ERROR_SUCCESS)
        {
            RegQueryValueEx(hKey, szName, NULL, NULL, (LPBYTE)PerfData, &cbData);
            printf("      %d: %s:  %s\n", dwIndex + 1, szName, PerfData);
            dwIndex++;
        }
        else break;
    }

    RegCloseKey(hKey);

    Freq();
    cout << "2.2.  CPU cycle count: " << GetCounter()  << "  us \n";
    return 0;
}
