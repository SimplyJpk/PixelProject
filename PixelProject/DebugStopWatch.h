#pragma once
#include <unordered_map>
#include <SDL.h>
#include <iostream>
#include <string>

class DebugStopWatch
{
public:
		char out_string[50] = "\0";

		void AddTimer(const char* timerName, bool enabled) {
				StopWatchEnabled[timerName] = enabled;
				StopWatchTime[timerName] = SDL_GetTicks();
				keys.push_back(timerName);
		}

		void SetTimerState(const char* timerName, bool enabled) {
				StopWatchEnabled[timerName] = enabled;
		}

		std::string GetData() {
				std::string result;
				// Early abort since this can fire before all times are filled.
				// TODO Fix? Do I care? eh
				if (keys.size() != times.size())
						return result;
				for (short i = 0; i < keys.size(); i++)
				{
						if (StopWatchEnabled[keys[i]]) {
								snprintf(out_string, 50, "%i\tms\t%s\n", times[keys[i]], keys[i]);
								result += out_string;
						}
				}
				return result;
		}

		void StoreTime(const char* timerName) {
				times[timerName] = (SDL_GetTicks() - StopWatchTime[timerName]);
		};
		void UpdateTime(const char* timerName) {
				StopWatchTime[timerName] = SDL_GetTicks();
		}
		DebugStopWatch() {};
		~DebugStopWatch() {};

private:
		std::vector<const char*> keys;
		std::unordered_map<const char*, Uint32> times;
		std::unordered_map<const char*, bool> StopWatchEnabled;
		std::unordered_map<const char*, Uint32> StopWatchTime;
};