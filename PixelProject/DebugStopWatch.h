#pragma once
#include <unordered_map>
#include <SDL.h>
#include <iostream>
#include <string>

class DebugStopWatch
{
public:
   char out_string[50] = "\0";

   void AddTimer(const char* timer_name, const bool enabled) {
      stop_watch_enabled_[timer_name] = enabled;
      stop_watch_time_[timer_name] = SDL_GetTicks();
      keys_.push_back(timer_name);
   }

   void SetTimerState(const char* timer_name, const bool enabled) {
      stop_watch_enabled_[timer_name] = enabled;
   }

   std::string GetData() {
      std::string result;
      // Early abort since this can fire before all times are filled.
      // TODO Fix? Do I care? eh
      if (keys_.size() != times_.size())
         return result;
      for (short i = 0; i < keys_.size(); i++)
      {
         if (stop_watch_enabled_[keys_[i]]) {
            snprintf(out_string, 50, "%0.1f\tms\t%s\n", times_[keys_[i]], keys_[i]);
            result += out_string;
         }
      }
      return result;
   }

   void StoreTime(const char* timer_name) {
      times_[timer_name] = (SDL_GetTicks() - stop_watch_time_[timer_name]);
   };
   void StoreTime(const char* timer_name, const float time)
   {
      times_[timer_name] = time;
   }
   void UpdateTime(const char* timer_name) {
      stop_watch_time_[timer_name] = SDL_GetTicks();
   }
   DebugStopWatch() {};
   ~DebugStopWatch() {};

private:
   std::vector<const char*> keys_;
   std::unordered_map<const char*, float> times_;
   std::unordered_map<const char*, bool> stop_watch_enabled_;
   std::unordered_map<const char*, Uint32> stop_watch_time_;
};