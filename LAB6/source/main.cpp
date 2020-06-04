#include <Windows.h>
#include <iostream>
#include <string.h>
#include <ctime>
#include <cstdlib>
#include "res.h"

#pragma comment( lib, "winmm.lib")

INT_PTR CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  
 

  switch (uMsg)
  {
  case WM_COMMAND:
    switch (HIWORD(wParam))
    {
    case BN_CLICKED: // zdarzenie klikniecia na UI
      switch (LOWORD(wParam))
      {
      case IDC_BUTTON1:
        WAVEFORMATEX pcmWaveFormat;                  //Definicja formatu dŸwieku
        pcmWaveFormat.wFormatTag = WAVE_FORMAT_PCM;  //Format próbek
        pcmWaveFormat.nChannels = 1;                 //Liczba kana³ów
        pcmWaveFormat.nSamplesPerSec = 44100L;       //Próbkowanie
        pcmWaveFormat.wBitsPerSample = 8;            //Iloœæ bitów na próbkê
        pcmWaveFormat.nAvgBytesPerSec = 44100L;      //Iloœæ Bajtów na sekundê 
        pcmWaveFormat.nBlockAlign = 1;
        pcmWaveFormat.cbSize = 0;

        MMRESULT mmResult;
        HWAVEOUT hwo{};                     //Uchwyt do interfejsu urz¹dzenia odtwarzaj¹cego
        UINT devId;                       //Identyfikator urz¹dzenia 

        for (devId = 0; devId < waveOutGetNumDevs(); devId++)
        {
          mmResult = waveOutOpen(&hwo, devId, &pcmWaveFormat, 0, 0, CALLBACK_NULL);
          if (mmResult == MMSYSERR_NOERROR)
          {
            break;
          }
        }
        if (mmResult != MMSYSERR_NOERROR)
        {
          MessageBox(hwnd, TEXT("Nie znaleziono karty d¿wiekowej o wymaganych paramtrach"), TEXT("Error"), MB_OK);
          return mmResult;
        }
        // Przygotowanie próbek do odtworzenia 
        WAVEHDR whdr;
        ZeroMemory(&whdr, sizeof(WAVEHDR));
        whdr.lpData = new char[pcmWaveFormat.nAvgBytesPerSec * 1]; //Iloœæ bajtów na sekundê
        whdr.dwBufferLength = pcmWaveFormat.nAvgBytesPerSec * 1;   // razy iloœæ sekund 
        whdr.dwUser = 0;
        whdr.dwFlags = 0;
        whdr.dwLoops = 0;
        whdr.dwBytesRecorded = 0;
        whdr.lpNext = 0;
        whdr.reserved = 0;
        //Wype³nienie buffora próbek próbkami sygna³u o czêstotliwoœci 880 Hz 
        for (int i = 0; i < whdr.dwBufferLength; i++)
        {
          whdr.lpData[i] = 128 * sin(i * 880.0 / (double)pcmWaveFormat.nSamplesPerSec) + 128;
        }

        waveOutSetVolume(hwo, 0xFFFFFFFF); //Ustawienie g³oœnoœci
        mmResult = waveOutPrepareHeader(hwo, &whdr, sizeof(WAVEHDR)); //Wys³anie nag³ówka 
        if (mmResult != MMSYSERR_NOERROR)                                //Przygotowanie 
        {                                                             //urz¹dzenia
          MessageBox(hwnd, TEXT("Nie Mo¿na zainicjowaæ karty"), TEXT("Error"), MB_OK);
          return mmResult;
        }

        mmResult = waveOutWrite(hwo, &whdr, sizeof(WAVEHDR));//Wys³anie próbek do urzadznia 

        if (mmResult != MMSYSERR_NOERROR)
        {
          MessageBox(hwnd, TEXT("Nie mozna za³adowaæ próbek"), TEXT("Error"), MB_OK);
          return mmResult;
        }

        while ((whdr.dwFlags & WHDR_DONE) != WHDR_DONE)
        {
        
            Sleep(200); //Czekanie na koniec 
         
        }
        //otwarzania     
        //Zamkniecie urz¹dzenia 
        mmResult = waveOutUnprepareHeader(hwo, &whdr, sizeof(WAVEHDR));
        mmResult = waveOutClose(hwo); delete[] whdr.lpData; //Usuniêcie bufora z pamiec
    
        break;
      }
    }
    return FALSE;
  case WM_CLOSE:
    DestroyWindow(hwnd); //zniszczenie okienka
    PostQuitMessage(0);
    return TRUE;
  }
  return FALSE;
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  
  HWND hwnd = CreateDialog(hinstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwnd, iCmdShow);
  SetForegroundWindow(hwnd);                                                      
  SetFocus(hwnd);

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }


  return 0;
}