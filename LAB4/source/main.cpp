#include <Windows.h>
#include <cstdio>
#include "res.h"

using namespace std;

LRESULT CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static BITMAP bmInfo = {};

  switch (uMsg)
  {
  case WM_CREATE: //Tworzenie okna
    break;
  case WM_PAINT: //Rysowanie okna
    HDC hDC; // uchwyt do kontekstu urz¹dzenia
    hDC = GetDC(hwnd); // pobranie uchwytu do kontekstu okna 
    HBITMAP hBitmap;
    hBitmap = (HBITMAP)LoadImageA(GetModuleHandleW(NULL), MAKEINTRESOURCE("SZPI.BMP"), 0, 124 , 124/*910, 380*/ , LR_LOADFROMFILE);
    
    GetObjectW(hBitmap, sizeof(bmInfo), &bmInfo);
   

    HDC hDCBitmap;
    hDCBitmap = CreateCompatibleDC(hDC); //Utworzenie kompatybilengo kontekstu

    SelectObject(hDCBitmap, hBitmap); //Wybranie bitmapy w kontekscie
    
   // BitBlt(hDC, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, hDCBitmap, 0, 0, SRCCOPY); //Skopiowanie obrazu
    StretchBlt(hDC, 0, 0, 124,124, hDCBitmap, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, SRCCOPY); 
    DeleteDC(hDCBitmap); //Usuniecie kontekstu
    DeleteObject(hBitmap);

    //SelectObject(hDC, GetStockObject(ANSI_VAR_FONT)); //Pobranie i aktywacja
    //czcionki
    //SetTextColor(hDC, RGB(255, 0, 0)); //Ustawienie koloru
    //TextOut(hDC, 0, 0, TEXT("Text output to client area."), 27);

    ReleaseDC(hwnd, hDC); // Zwolnienie kontekstu urz¹dzenia
    break;
  case WM_COMMAND:
  {
    switch (HIWORD(wParam))
    {
    case BN_CLICKED:
      switch (LOWORD(wParam))
      {
      case IDC_BUTTON1:
        MessageBox(hwnd, "Working", "Dzia³a", MB_OK);
        break;
      }
    }
  }
  return 0;
  case WM_CLOSE:
  {
    DestroyWindow(hwnd); //zniszczenie okienka
    PostQuitMessage(0);
    break;
  }
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  }
  return 0;
}


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{

  HWND hwndMainWindow = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}


