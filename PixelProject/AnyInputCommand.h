#pragma once
#include "ICommand.h"

class AnyInputCommand : public ICommand
{
public:
   void Execute() override = 0;
};
