#pragma once
#include <iostream>
#include <windows.h>
enum ConsoleColour
{
   Black = 0,
   Blue,
   Green,
   Aqua,
   Red,
   Purple,
   Yellow,
   White,
   Grey,
   LightBlue,
   LightGreen,
   LightAqua,
   LightRed,
   LightPurple,
   LightYellow,
   BrightWhite
};

class ConsoleUtility
{
public:
   static HANDLE GetHandle()
   {
      static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
      return handle;
   }
   static const ConsoleColour DefaultColour = ConsoleColour::BrightWhite;

   static void PrintText(const std::string& text, const ConsoleColour colour = ConsoleColour::BrightWhite)
   {
      SetConsoleTextAttribute(GetHandle(), colour);
      std::cout << text;
      SetConsoleTextAttribute(GetHandle(), DefaultColour);
   }

   static void SetColour(const ConsoleColour colour)
   {
      SetConsoleTextAttribute(GetHandle(), colour);
   }

   static void ResetColour()
   {
      SetConsoleTextAttribute(GetHandle(), DefaultColour);
   }

};
