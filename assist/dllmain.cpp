// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <Windows.h>
#include <string>
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
#pragma pack(push,1)

typedef struct
{ BYTE Padding1[3];
  BYTE OEM_Name[8];
} NTFS_BootRecord;


#pragma pack(pop)
std::string byte2ch(BYTE* data, int size)
{
    return std::string((char*)data, size);
}
extern "C" __declspec(dllexport) void CheckSystem(HWND mainhWnd)
{
  //  MessageBox(mainhWnd, "dll func ready to work v2", "pobeda_6", NULL);

  //  InvalidateRect(mainhWnd, NULL, TRUE); // помечает окно как устаревшее,требует его перересовки (WM_PAINT)
}

extern "C" __declspec(dllexport) void CheckPhysDrives(HWND mainhWnd)
{    
     std::string fileName = "\\\\.\\PhysicalDrive";
     int foundDriveCount = 0;
     int msgDriveX = 50;
     int msgDriveY = 50;

     for (int DriveNum = 0; DriveNum < 30; DriveNum++)
     {
         std::string fullPath = fileName + std::to_string(DriveNum);
         HANDLE fileHandle = CreateFileA(
             fullPath.c_str(),
             GENERIC_READ,
             FILE_SHARE_READ | FILE_SHARE_WRITE,
             NULL,
             OPEN_EXISTING,
             FILE_ATTRIBUTE_NORMAL,
             NULL);
         if (fileHandle != INVALID_HANDLE_VALUE)
         {
             foundDriveCount++;
         }
     }   
         if (foundDriveCount == 0)
         {
             MessageBox(mainhWnd, "No drives found on this PC", "error_8", NULL);
         }
         else
         {
             HDC hdc = (HDC)GetDC(mainhWnd);
             ::TextOut(hdc, msgDriveX, msgDriveY, "Found Drives list:", 19);
             for (int DriveNum = 0; DriveNum < 30; DriveNum++)
             {
                 std::string fullPath = fileName + std::to_string(DriveNum);
                 HANDLE fileHandle = CreateFileA(
                     fullPath.c_str(),
                     GENERIC_READ,
                     FILE_SHARE_READ | FILE_SHARE_WRITE,
                     NULL,
                     OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL,
                     NULL);

                 if (fileHandle != INVALID_HANDLE_VALUE)
                 {
                     msgDriveY = msgDriveY + 20;
                     ::TextOut(hdc, msgDriveX, msgDriveY, fullPath.c_str(), 19);
                 }
                 CloseHandle(fileHandle);
             }
             
         }
    

}

extern "C" __declspec(dllexport) void CheckVolume(HWND mainhWnd)
{
    std::string fileName = "\\\\.\\";
    std::string letters[26] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
    const int msgVolumeX = 200;
    int msgVolumeY = 50;
    int foundVolumeCount = 0;
    for (int DriveNum = 0; DriveNum < 26; DriveNum++)
    {   std::string fullPath = fileName + letters[DriveNum]+":";
        HANDLE fileHandle = CreateFileA(fullPath.c_str(),GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
        if (fileHandle != INVALID_HANDLE_VALUE)
        {
            foundVolumeCount++;
        };
        
    }
    if (foundVolumeCount == 0)
      {
         MessageBox(mainhWnd, "No volumes found on this PC", "error_6", NULL);
      }
    else
    {
        
        HDC hdc = (HDC)GetDC(mainhWnd);
        ::TextOut(hdc, msgVolumeX, msgVolumeY, "Found Volumes list:", 20);
        for (int DriveNum = 0; DriveNum < 26; DriveNum++)
          {  
            std::string fullPath = fileName + letters[DriveNum] + ":";
            HANDLE fileHandle = CreateFileA(fullPath.c_str(),GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
            
            if (fileHandle != INVALID_HANDLE_VALUE)
              {
                ULONGLONG startOffset = 0;
                BYTE buffer[512];
                DWORD bytesToRead = 512; 
                DWORD bytesRead;
                LARGE_INTEGER sectorOffset;
                sectorOffset.QuadPart = 0;
                NTFS_BootRecord *pNTFS_BootRecord = reinterpret_cast <NTFS_BootRecord*> (buffer);
                // Задаем позицию
                unsigned long currentPosition = SetFilePointer(fileHandle,sectorOffset.LowPart,NULL,FILE_BEGIN);
                if (currentPosition != sectorOffset.LowPart)
                {
                    //обработчик ошибки 
                    MessageBox(mainhWnd, "Set file pointer error", "error_9", NULL);
                }
                else
                {
                    bool readResult = ReadFile( fileHandle,buffer,bytesToRead, &bytesRead,NULL);
                    if (!readResult || bytesRead != bytesToRead)
                    {
                        //Обработка ошибки чтения
                        MessageBox(mainhWnd, "Error when reading the disk", "error_10", NULL);
                        
                    }
                    else
                    {
                       // MessageBox(mainhWnd, byte2ch(pNTFS_BootRecord->OEM_Name, 4).c_str(), "error_10", NULL);
                        ::TextOut(hdc, msgVolumeX+50, msgVolumeY+20, byte2ch(pNTFS_BootRecord->OEM_Name, 4).c_str(), 5);
                        msgVolumeY = msgVolumeY + 20;
                        ::TextOut(hdc, msgVolumeX, msgVolumeY, letters[DriveNum].c_str(), 1);
                    };
                }
                
              }
            CloseHandle(fileHandle);
          }
      }
    
}
