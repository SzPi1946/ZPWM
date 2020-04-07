#include <Windows.h>
#include <iostream>
#include <string.h>
#include <ctime>
#include <cstdlib>
#include "res.h"

HINSTANCE g_hlnstance;

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_INITDIALOG:
  {
    HICON hIcon = LoadIcon(g_hlnstance, MAKEINTRESOURCE(IDI_MAINICON));
    SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

    HMENU hMenu = LoadMenu(g_hlnstance, MAKEINTRESOURCE(IDR_MENU1));
    SetMenu(hwndDlg, hMenu);
    return FALSE;
  }
  case WM_COMMAND:
    switch (HIWORD(wParam))
    {
    case BN_CLICKED:
      {
      switch (LOWORD(wParam))
        {
      case IDC_BUTTON1:
        MessageBox(hwndDlg, "Witam tu Kajko", "Hej!", MB_OK);
        break;
        }
      }
    }
    return 0;
  case WM_CLOSE:
  {
    DestroyWindow(hwndDlg); //zniszczenie okienka
    PostQuitMessage(0);
    break;
  }
  }
  return 0;
}

INT_PTR CALLBACK SecProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_INITDIALOG:
  {
    HICON hIcon2 = LoadIcon(g_hlnstance, MAKEINTRESOURCE(IDI_DRUGA));
    SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon2);

    HMENU hMenu2 = LoadMenu(g_hlnstance, MAKEINTRESOURCE(IDR_MENU2));
    SetMenu(hwndDlg, hMenu2);
    return FALSE;
  }
  case WM_COMMAND:
    switch (HIWORD(wParam))
    {
    case BN_CLICKED:
    {
      switch (LOWORD(wParam))
      {
      case IDC_BUTTON1:
        MessageBox(hwndDlg, "Witam tu Kokosz", "Hej!", MB_OK);
        break;
      }
    }
    }
    return 0;
  case WM_CLOSE:
  {
    DestroyWindow(hwndDlg); //zniszczenie okienka
    PostQuitMessage(0);
    break;
  }
  }
  return 0;
}


int WINAPI WinMain(HINSTANCE hlnstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  g_hlnstance = hlnstance;
  HWND hwndMainWindow = CreateDialog(hlnstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  HWND hwndSecondWindow = CreateDialog(hlnstance, MAKEINTRESOURCE(IDD_SECONDVIEW), NULL, SecProc);
  ShowWindow(hwndMainWindow, iCmdShow);
  ShowWindow(hwndSecondWindow, iCmdShow);

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);

  }

  return 0;
}