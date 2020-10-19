#pragma once

#include<Windows.h>
#include<Psapi.h>
#include<iostream>

//TODO Don't release with this kind of junk built in (This won't work outside windows)
class MemoryUsage
{
		int CurrentFrameMemory = 0;
		int ReferenceFrameMemory = 0;

		int lastCheckTime = 0;
public:
		// Returns the memory used and updates internal checks, if 'useAccurate' is true, it checks against bytes instead of kb
		int ReturnMemoryUsed(bool useAccurate = false) {
				//get the handle to this process
				auto myHandle = GetCurrentProcess();
				//to fill in the process' memory usage details
				PROCESS_MEMORY_COUNTERS pmc;
				//return the usage (bytes), if I may
				if (GetProcessMemoryInfo(myHandle, &pmc, sizeof(pmc)))
				{
						CurrentFrameMemory = pmc.WorkingSetSize / (useAccurate ? 1 : 1024);
						if (SDL_GetTicks() - lastCheckTime > 2000) {
								ReferenceFrameMemory = CurrentFrameMemory;
								lastCheckTime = SDL_GetTicks();
						}
						return(pmc.WorkingSetSize);
				}
				else
						return -1;
		}
		bool isMemoryMore() {
				if (ReferenceFrameMemory >= CurrentFrameMemory) {
						return false;
				}
				return true;
		}
};