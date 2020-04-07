#include <Windows.h>
#include <iostream>
#include <string.h>

using namespace std;

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, PSTR szCmdLine, int iCmdShow)
{

  int trafienie;
  int polowa;
  int strzal = 20;
  char tablica[40];
  int odp = MessageBox(0, TEXT("Hej, zagramy w gre?"), TEXT("Witaj!"), MB_ICONQUESTION | MB_YESNO);
  int podpowiedz;
  if (odp == IDYES)
  {

    do
    {

      sprintf_s(tablica, "Czy ta liczba to : %d?", strzal);
      trafienie = MessageBox(0, tablica, "O to chodzi", MB_ICONQUESTION | MB_YESNO);
      if (trafienie == IDNO)
      {
        if (strzal == 0)
        {
          strzal = 1;
        }

        sprintf_s(tablica, "Czy ta liczba jest wieksza od : %d?", strzal);
        podpowiedz = MessageBox(0, tablica, "Wieksza czy mniejsza?", MB_ICONQUESTION | MB_YESNO);


        if ((strzal / 2) % 2 != 0)
        {
          polowa = (strzal + 1) / 2;
        }
        else
        {
          polowa = strzal / 2;
        }

        if (podpowiedz == IDYES)
        {
          strzal = polowa + strzal;
        }
        else
        {
          strzal = polowa - strzal;
        }
      }
      else
      {
        MessageBox(0, "Hura! Udalo sie zgadnac!", "Yay!", MB_OK);
      }

    } while (trafienie != IDYES);

  }

  //MessageBox(0, , "Aplikacja", MB_OK | MB_ICONQUESTION);// to drugie to po prostu znak zapytania jako grafika

  return 0;
}