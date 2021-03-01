#pragma once
#include <glm/ext/matrix_transform.hpp>

#include "VectorHelper.h"

using namespace PixelProject::Constant::Vector;

class Transform
{
public:
   Transform(){};
   ~Transform(){};

   glm::vec3 GetPosition() { return local_position_; }
   glm::vec3 GetScale() { return local_scale_; }
   glm::vec3 GetRotation() { return local_rotation_; }

   void SetPosition(const glm::vec3 position)
   {
      local_position_ = position;
      has_changed_ = true;
   }
   void SetScale(const glm::vec3 scale)
   {
      local_scale_ = scale;
      has_changed_ = true;
   }
   void SetRotation(const glm::vec3 rotation)
   {
      local_rotation_ = rotation;
      has_changed_ = true;
   }

   glm::mat4& GetModel()
   {
      if (has_changed_)
      {
         model_transform_ = glm::mat4(1.0f);
         model_transform_ = glm::translate(model_transform_, local_position_);
         model_transform_ = glm::scale(model_transform_, local_scale_);
         model_transform_ = glm::rotate(model_transform_, local_rotation_.y, vec3_up);
         model_transform_ = glm::rotate(model_transform_, local_rotation_.x, vec3_left);
         model_transform_ = glm::rotate(model_transform_, local_rotation_.z, vec3_forward);
         has_changed_ = false;
      }
      return model_transform_;
   }

protected:
   glm::mat4 model_transform_ = glm::mat4(1.0f);

   glm::vec3 local_position_ = vec3_zero;
   glm::vec3 local_scale_ = vec3_one;
   glm::vec3 local_rotation_ = vec3_zero;
private:
   bool has_changed_ = true;
};
