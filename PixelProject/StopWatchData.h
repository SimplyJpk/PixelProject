#pragma once
#include <string>
#include <vector>

template <typename T>
class StopWatchData
{
public:
   std::string name;
   int store = 1;

   StopWatchData(std::string name, int count)
   {
      this->name = name;
      this->store = count;
      this->data_.resize(count);
   }

   void Set(T data)
   {
      //TODO Incrememnt from here
      data_ = data;
   }
   T Get(int index = 0)
   {
      return data_[index];
   }

   std::string GetDataLine()
   {
      return name + " " + data_;
   }

   std::string GetDataLineAvg()
   {
      return name + ":\t" + GetAverage();
   }
private:
   T GetAverage()
   {
      T average;
      for (size_t i = 0; i < data_.size(); i++)
      {
         average += data_[i];
      }
      average /= data_.size();
      return average;
   }
   std::vector<T> data_;
};
