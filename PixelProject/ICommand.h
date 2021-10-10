#pragma once
#include <SDL_stdinc.h>

/// <summary>
/// Command Pattern Implementation with a couple choices for Args
/// </summary>
class ICommand
{
public:
   virtual ~ICommand() = default;

   virtual void Execute() = 0;
   virtual void Execute(Uint32 value) = 0;
};
