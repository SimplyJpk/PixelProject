#pragma once
#include <unordered_map>
#include <SDL.h>
#include <iostream>
#include <string>
#include "imgui.h"

//TODO Create a data class that can hold more than a name and a float.
//IE; Name, current value, last value, last update?

class DebugStopWatch
{
public:
   char out_string[50] = "\0";

   void AddTimer(const char* timer_name)
   {
      if (!KeyExists(timer_name))
         times_[timer_name] = 0.0f;
   }
   float GetTime(const char* timer_name)
   {
      if (KeyExists(timer_name))
         return times_[timer_name];
      return 0.0f;
   }

   void AddToGroup(const char* group_name, const char* timer_name, float default_value = 0.0f)
   {
      if (!GroupExists(group_name))
         groups_.insert({ group_name, std::vector<const char*>() });

      AddTimer(timer_name);
      groups_[group_name].push_back(timer_name);

      if (default_value != 0.0f)
      {
         times_[timer_name] = default_value;
      }
   }

   void DrawGroupData(const char* group_name)
   {
      if (GroupExists(group_name)) {
         ImGui::Begin(group_name);

         auto keys = groups_[group_name];
         for (short i = 0; i < keys.size(); i++)
         {
            ImGui::Text("%0.1f\t%s\n", times_[keys[i]], keys[i]);
         }

         ImGui::End();
      }
   }

   void UpdateTime(const char* timer_name, const float time)
   {
      if (!KeyExists(timer_name))
         AddTimer(timer_name);

      times_[timer_name] = time;
   }
   void UpdateTimeSince(const char* timer_name, const float time)
   {
      times_[timer_name] = time - times_[timer_name];
   }

protected:
   bool KeyExists(const char* key) const
   {
      return (times_.count(key) != 0);
   }
   bool GroupExists(const char* group) const
   {
      return (groups_.count(group) != 0);
   }

private:
   std::unordered_map<const char*, float> times_;

   std::unordered_map<const char*, std::vector<const char*>> groups_;
};
