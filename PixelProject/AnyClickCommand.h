#pragma once
#include "ICommand.h"

class AnyClickCommand : public ICommand
{
public:
   void Execute() override = 0;
};
