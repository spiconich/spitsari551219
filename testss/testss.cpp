#include <iostream>
#include <Windows.h>
using namespace std;

const char g_szClassName[] = "chckrClass";

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //std::string VolumeNum[] = { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" };
   // int DriveNum[31] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    HDC hDeviceContext;
    PAINTSTRUCT paintStruct;
    RECT rectPlace;
    HFONT hFont;
    switch (msg)
    {
    case WM_COMMAND:

        switch (LOWORD(wParam))
        {
        case 1:
        {
            typedef UINT(*MYFUNC)(UINT, UINT);
            // Указатель на функцию
            void (*dllCheckVolume) (HWND);
            void (*dllCheckPhysDrives) (HWND);
            // Загружаем динамически подключаемую библиотеку
            HMODULE hLib = LoadLibrary("assist.dll");
            if (hLib == NULL)
            {
                MessageBox(hWnd, "Error while loading DLL", "Error_1", NULL);
                break;
            }

            // Настраиваем адрес функции
            dllCheckVolume = (void (*)(HWND))GetProcAddress(hLib, "CheckVolume");
            if (!dllCheckVolume)
            {
                MessageBox(hWnd, "Error while getting func address", "Error_2", NULL);
                return GetLastError();
            }
            dllCheckPhysDrives = (void (*)(HWND))GetProcAddress(hLib, "CheckPhysDrives");
            if (!dllCheckPhysDrives)
            {
                MessageBox(hWnd, "Error while getting func address", "Error_7", NULL);
                return GetLastError();
            }
            // Вызываем функцию из библиотеки
            dllCheckPhysDrives(hWnd);
            dllCheckVolume(hWnd);

            // Отключаем библиотеку
            if (!FreeLibrary(hLib))
            {
                MessageBox(hWnd, "Error while dropping lib from memory", "Error_3", NULL);
                return GetLastError();
            }

        }
        break;

        case 2:
            DestroyWindow(hWnd);
            break;
        }


        break;
    //case WM_PAINT:
   // {   if (wm_paint_id == 1)
     //   {
      //     hDeviceContext = BeginPaint(hWnd, &paintStruct);
       //    GetClientRect(hWnd, &rectPlace);
       //    SetTextColor(hDeviceContext, NULL);
        //   hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0,
       //    DEFAULT_CHARSET,
        //   0, 0, 0, 0,
        //   "Arial Bold" );
        //   SelectObject(hDeviceContext, hFont);
         //  DrawText(hDeviceContext, "TEXT", 6, &rectPlace, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        //   EndPaint(hWnd, &paintStruct);
        // };
   // break;
   // }
    
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
   
    case WM_DESTROY:
    {
        PostQuitMessage(NULL);
        break;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{  
    WNDCLASSEX wc;
    HWND hwnd;
    HWND hwndButtonExit;
    HWND hwndButtonOk;
    MSG Msg;

    
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

 
    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Class not register", "Error_4", MB_ICONINFORMATION | MB_OK);
        return 0;
    }
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "CHECKER",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL,
        hInstance,
        NULL);
    if (hwnd == NULL)
    {
        MessageBox(NULL, " Window Creation Failed", "Error_5 ", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    hwndButtonExit = CreateWindow(
        "BUTTON", 
        "OK", 
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // styles
        500, 
        525, 
        100, 
        30, 
        hwnd, // parent window
        (HMENU)1, // ВОТ ЭТО ЗНАЧЕНИЕ
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL); // pointer not needed
    hwndButtonOk = CreateWindow(
        "BUTTON", 
        "EXIT", 
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
        600,
        525, 
        100, 
        30, 
        hwnd, // parent window
        (HMENU)2, // ВОТ ЭТО ЗНАЧЕНИЕ
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL); // pointer not needed

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}


