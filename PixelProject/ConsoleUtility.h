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

static class ConsoleUtility
{
public:
   inline static HANDLE ConsoleHandle = nullptr;
   inline static ConsoleColour DefaultColour = ConsoleColour::BrightWhite;

   //TODO Make this better? Shouldn't have to check this every time
   static void SetHandle()
   {
      if (ConsoleHandle == nullptr)
         ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   }

   static void PrintText(char* text, const ConsoleColour colour = DefaultColour)
   {
      SetHandle();
      SetConsoleTextAttribute(ConsoleHandle, colour);
      std::cout << text;
      SetConsoleTextAttribute(ConsoleHandle, DefaultColour);
   }

   static void SetColour(const ConsoleColour colour)
   {
      SetHandle();
      SetConsoleTextAttribute(ConsoleHandle, colour);
   }

   static void ResetColour()
   {
      SetHandle();
      SetConsoleTextAttribute(ConsoleHandle, DefaultColour);
   }

};
