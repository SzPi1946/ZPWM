#include <Windows.h>
#include <iostream>
#include <string.h>
#include <ctime>
#include <cstdlib>
#include "res.h"

using namespace std;
int number;
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  srand(unsigned int(time(NULL)));
  switch (uMsg)
  {
  case WM_COMMAND:
    char wartosc[10];
    switch (HIWORD(wParam))
    {
    case BN_CLICKED: // zdarzenie klikniecia na UI
      switch (LOWORD(wParam))
      {
      case IDC_BUTTON: //klikniecie na nasz przycisk
        {
        char temp_buffer[10];
        //MessageBox(hwndDlg, "Text", "Kikniecie", MB_OK);
        HWND hwndEditBox = GetDlgItem(hwndDlg, IDC_EDIT);
        int iTextLength = GetWindowTextLength(hwndEditBox);
        GetWindowText(hwndEditBox, temp_buffer, iTextLength + 1);
        int Guess = atoi(temp_buffer);
        if (number == Guess)
          {
          MessageBox(hwndDlg, "Gratulacje!", "Gratulacje!", MB_OK);
          }
        else if (number > Guess)
          {
          MessageBox(hwndDlg, "Zbyt mala liczba!", "Sprobuj jeszcze raz", MB_OK);
          }
        else if (number < Guess)
          {
          MessageBox(hwndDlg, "Zbyt duza liczba!", "Sprobuj jeszcze raz!", MB_OK);
          }
        //IDC_EDIT1
        //int iTextLength = GetWindowTextLength(hwndEditBox);
        //CHAR szText[500];
        //GetWindowText(hwndEditBox, szText, iTextLength + 1);
        //SetWindowText((HWND)lParam, szText);
        return TRUE;
        }
      case IDC_BUTTON2:
        {
        number = rand() % 40 + 1;
        wsprintf(wartosc, "%d", number);
        break;
        }
      }
    }
    return FALSE;
  case WM_LBUTTONDOWN:
    CHAR szText[200];
    wsprintf(szText, TEXT("kliknales muszka x=%d, y=%d"), LOWORD(lParam), HIWORD(lParam));
    MessageBox(hwndDlg, szText, TEXT("Klikniecie"), MB_OK);
    return TRUE;
  case WM_CLOSE:
    DestroyWindow(hwndDlg); //zniszczenie okienka
    PostQuitMessage(0);
    return TRUE;
  }
  return FALSE;
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  //git tortoise 

  //w repozytorium tylko niezbedne pliki
  //nie umieszczamy: plików tymczasowych, pliku .exe nie umieszczamy
  //nie umieszczamy debug i release 

  //push aby wrzucic cos na repozytorium 
  //zielony znaczek to znaczy ze repozytorium dosta³o dobre pliki i jestgit
  // inny znaczek oznacza ze byly zmiany i trzeba na nowo je zrobiæ 

  // Okienka systemu windows 9.03.2020
  //okienko bierzem z Zasobó , czyli pliki dodane w momencie kompilacji do programu 
  //dzieki temu nie trzeba samemu tworzyc np okienek, tylko z szablonu korzystamy
  //plik zasobów umieszcza sie w: *.rc 
  //plik mozna edytowac jako plik tekstowy, lub za pomoca edytora wizualnego Visuala

  HWND hwndMainWindow = CreateDialog(hinstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);

  }
  //stworz gre w jaka liczbe mam na mysli, ale tym razem komputer ja wybral a my musimy zgadnac, 
  // on losuje od 1 do 40 a myu w pole tekstowe wpisujemy liczbe i sprawdzamy czy trafimy

  return 0;
}