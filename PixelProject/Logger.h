#pragma once
#include <iostream>

namespace Logger
{
   void Log(const char* message)
   {
      std::cout << message;
   }
}
