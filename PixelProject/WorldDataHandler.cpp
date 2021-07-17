#include "WorldDataHandler.h"

WorldDataHandler& WorldDataHandler::Instance()
{
   static WorldDataHandler instance;
   return instance;
}
