#pragma once

#include<Windows.h>
#include<Psapi.h>

//TODO Don't release with this kind of junk built in (This won't work outside windows)
class MemoryUsage
{
   int current_frame_memory_ = 0;
   int reference_frame_memory_ = 0;

   int last_check_time_ = 0;
public:
   // Returns the memory used and updates internal checks, if 'useAccurate' is true, it checks against bytes instead of kb
   int ReturnMemoryUsed(const bool use_accurate = false)
   {
      //get the handle to this process
      const HANDLE myHandle = GetCurrentProcess();
      //to fill in the process' memory usage details
      PROCESS_MEMORY_COUNTERS pmc;
      //return the usage (bytes), if I may
      if (GetProcessMemoryInfo(myHandle, &pmc, sizeof(pmc)))
      {
         current_frame_memory_ = static_cast<int>(pmc.WorkingSetSize) / (use_accurate ? 1 : 1024);
         if (SDL_GetTicks() - last_check_time_ > 2000)
         {
            reference_frame_memory_ = current_frame_memory_;
            last_check_time_ = SDL_GetTicks();
         }
         return (static_cast<int>(pmc.WorkingSetSize));
      }
      else
         return -1;
   }

   bool IsMemoryMore() const
   {
      if (reference_frame_memory_ >= current_frame_memory_)
      {
         return false;
      }
      return true;
   }
};
