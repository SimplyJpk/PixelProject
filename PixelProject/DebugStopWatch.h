#pragma once
#include <unordered_map>
#include <SDL.h>
#include <iostream>
#include <string>

class DebugStopWatch
{
public:
   char out_string[50] = "\0";

   void AddTimer(const char* timer_name)
   {
      keys_.push_back(timer_name);
   }

   float GetTime(const char* timer_name)
   {
      return times_[timer_name];
   }

   std::string GetData()
   {
      std::string result;
      //// Early abort since this can fire before all times are filled.
      //// TODO Fix? Do I care? eh
      //if (keys_.size() != times_.size())
      //   return result;
      for (short i = 0; i < keys_.size(); i++)
      {
         if (times_[keys_[i]] == 0.0f)
            continue;

            snprintf(out_string, 50, "%0.1f\t%s\n", times_[keys_[i]], keys_[i]);
            result += out_string;
      }
      return result;
   }
   void UpdateTime(const char* timer_name, const float time)
   {
      times_[timer_name] = time;
   }
   void UpdateTimeSince(const char* timer_name, const float time)
   {
      times_[timer_name] = time - times_[timer_name];
   }

private:
   std::vector<const char*> keys_;
   std::unordered_map<const char*, float> times_;
};
