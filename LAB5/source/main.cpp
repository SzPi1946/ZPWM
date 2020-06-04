#include <Windows.h>
#include <iostream>
#include <string.h>
#include <ctime>
#include <cstdlib>
#include "res.h"
#define SERVER_ADDR "127.0.0.1"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


 const int LINES = 10;
 char asMessageList[LINES][1024];

 bool gNotExit = true;

 SOCKET serverSocket;
 SOCKET clientSocket;

 typedef struct { HWND hwnd; SOCKET socket; bool server; } sData;


void sendMessage(HWND hwnd)
{
 //for (int i = 1; i < LINES - 1; i++)
 //{
  // strcpy_s(asMessageList[i], asMessageList[i + 1]);
  //}

  char sMessage[1024];
  HWND hWndEditMessage;
  hWndEditMessage = GetDlgItem(hwnd, ID_EDIT_MESSAGE);
  GetWindowText(hWndEditMessage, asMessageList[LINES - 1]/* sMessage*/, 1024);

  //MessageBox(0, sMessage, TEXT("Wiadomoœæ"), MB_OK);
  struct sockaddr_in si_other; 
  memset((char*)&si_other, 0, sizeof(si_other)); 
  si_other.sin_family = AF_INET; 
  si_other.sin_port = htons(SERVER_PORT);
  si_other.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDR); 
  int slen = sizeof(si_other); 
  if (sendto(clientSocket, sMessage, strlen(sMessage), 0, (struct sockaddr*) & si_other, slen) == SOCKET_ERROR) 
  { 
    MessageBox(0, TEXT("Nie uda³o sie wys³aæ danych b³ad"), TEXT("WinSock Error"), MB_OK); 
    return;
  }
  //Wymusznie odmalowania okna
  RECT rc;
  GetClientRect(hwnd, &rc); //Pobranie obszaru klienciego 
  InvalidateRect(hwnd, NULL, false); //Uniewaznienie tresci
  RedrawWindow(hwnd,&rc,NULL,RDW_ERASE); //Wymuszenie odmalowania niewa¿nej
}

void displayMessageList(HDC hDC) 
{ 
  SelectObject(hDC, GetStockObject(ANSI_VAR_FONT)); 
  SetTextColor(hDC, RGB(255, 0, 0)); 
  for (int i = 0; i < LINES; i++)
  {
    TextOut(hDC, 0, 70 + i * 20, asMessageList[i], strlen(asMessageList[i]));
 }
}

void recieveMessage(sData* data)
{
  struct sockaddr_in si_other;
  int slen, recv_len;
  char buf[1024];
  while (gNotExit)
  {//Wyzerowanie buffora
    memset(buf, '\0', 1024);//Odbierz dane
    if ((recv_len = recvfrom(serverSocket, buf, 1024, 0, (struct sockaddr*) & si_other, &slen)) == SOCKET_ERROR)
    {
      MessageBox(0, TEXT("Nie uda³o sie odebraæ danych b³ad"), TEXT("WinSock            Error"), MB_OK);
      return;
    }
  }
  if (data->server) {//Przeœlij do innych osób
    int slen=sizeof(si_other);//send the message
    if (sendto(data->socket,buf, strlen(buf) , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
    {
      MessageBox(0,TEXT("Nie uda³o sie wys³aæ danych b³ad"),TEXT("WinSock Error"),MB_OK);
      return;
    }  
  }
  sprintf_s(asMessageList[LINES - 1], "%s:%d %s", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
  //Wymusznie odmalowania okna
  RECT rc;GetClientRect(data->hwnd, &rc); //Pobranie obszaru klienciego 
  InvalidateRect(data->hwnd, NULL, false); //Uniewaznienie tresci
  RedrawWindow(data->hwnd,&rc,NULL,RDW_ERASE); //Wymuszenie odmalowania niewaznej 
}

void createClient(HWND hwnd)
{//Utwórz Soket Klienta
  if((clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
  {
    MessageBox(0,TEXT("NIe mo¿na utworzyæ Soketa"),TEXT("WinSock Error"),MB_OK);
    //WSAGetLastError()
  }
  sendMessage(hwnd);
  sData* data = new sData; 
  data->hwnd = hwnd;
  data->socket = clientSocket;
  data->server = false; 
  DWORD dwThreadId; 
  CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&recieveMessage, data, 0, &dwThreadId);
}

void createServer(HWND hwnd)
{//Utwórz Soket Serwera
  if((serverSocket = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
  {
    MessageBox(0,TEXT("NIe mo¿na utworzyæ Soketa"),TEXT("WinSock Error"),MB_OK);
    //WSAGetLastError()
  }
  //Przygotój opis adresu serwera
  struct sockaddr_in server, si_other;server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;server.sin_port = htons( SERVER_PORT );//Podpiêcie adresy pod socket
  if( bind(serverSocket ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
  {
    MessageBox(0,TEXT("NIe mo¿na utworzyæ podpi¹æ soketa pod wybrany adress"),TEXT("WinSock Error"),MB_OK);
    return;
  }
  //odbierz dane
  //recieveMessage();
  sData* data = new sData; 
  data->hwnd = hwnd; 
  data->socket = serverSocket;
  data->server = true;
  DWORD dwThreadId; 
  CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&recieveMessage, NULL, 0, &dwThreadId);
}

INT_PTR CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  HWND hWndButtonCreateServer;
  HWND hWndButtonSendMessage;
  HWND hWndButtonCreateClient;
  HWND hWndEditMessage;
  switch (uMsg)
  {
  case WM_CREATE:
    hWndButtonSendMessage = CreateWindowA(TEXT("BUTTON"), TEXT("Wyœlij wiadomoœæ"), BS_FLAT | WS_VISIBLE | WS_CHILD, 10, 20, 150, 20, hwnd, (HMENU)ID_SEND_MESSAGE,  NULL, NULL);
    hWndEditMessage = CreateWindowA(TEXT("EDIT"), TEXT("Moja Wiadomoœæ"), WS_BORDER | WS_VISIBLE | WS_CHILD, 10, 50, 250, 20, hwnd, (HMENU)ID_EDIT_MESSAGE, NULL , NULL);
    hWndButtonCreateServer = CreateWindowA(TEXT("BUTTON"), TEXT("Stwórz Serwer"), BS_FLAT | WS_VISIBLE | WS_CHILD, 170, 20, 150, 20, hwnd, (HMENU)ID_CREATE_SERVER, NULL, NULL);
    hWndButtonCreateClient = CreateWindow(TEXT("BUTTON"), TEXT("Stwórz Klienta"), BS_FLAT | WS_VISIBLE | WS_CHILD, 350, 20, 150, 20, hwnd, (HMENU)ID_CREATE_CLIENT,NULL, NULL);
    break;
  case WM_PAINT: 
    HDC hDC; // uchwyt do kontekstu urz¹dzenia 
    hDC = GetDC( hwnd ); // pobranie uchwytu do kontekstu obszaru roboczego
    displayMessageList(hDC);
    ReleaseDC(hwnd, hDC);
    break;
  case WM_COMMAND:
    switch (HIWORD(wParam))
    {
    case BN_CLICKED: // zdarzenie klikniecia na UI
      switch (LOWORD(wParam))
      {
      case ID_SEND_MESSAGE:
        sendMessage(hwnd); //Wyslij wiadomoœæ
        break;
      case ID_CREATE_SERVER:
        createServer(hwnd);
        break;
      case ID_CREATE_CLIENT:
        createClient(hwnd); 
        break;
      }
    }
    return FALSE;
  case WM_LBUTTONDOWN:
    CHAR szText[200];
    wsprintf(szText, TEXT("kliknales muszka x=%d, y=%d"), LOWORD(lParam), HIWORD(lParam));
    MessageBox(hwnd, szText, TEXT("Klikniecie"), MB_OK);
    return TRUE;
  case WM_CLOSE:
    DestroyWindow(hwnd); //zniszczenie okienka
    PostQuitMessage(0);
    return TRUE;
  }
  return FALSE;
}



int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  HWND hwndMainWindow = CreateDialog(hinstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);

  int iResult; WSADATA wsaData;// Initializacja Winsock
  iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
  if (iResult != NO_ERROR) 
  {
    MessageBox(0,TEXT("Initialization error."),TEXT("WinSock"),MB_OK);
    return 1;
  }

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  closesocket(serverSocket);
  WSACleanup();
  return 0;
}