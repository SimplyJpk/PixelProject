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
      UpdateBounds();
      has_changed_ = true;
   }
   void SetScale(const glm::vec3 scale)
   {
      local_scale_ = scale;
      UpdateBounds();
      has_changed_ = true;
   }
   void SetRotation(const glm::vec3 rotation)
   {
      local_rotation_ = rotation;
      has_changed_ = true;
   }

   const glm::mat4& GetModel()
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

   const glm::vec4& GetBounds()
   {
      return bounding_box_;
   }

protected:
   //TODO This could probably be a component?
   inline void UpdateBounds()
   {
      float scaleX = (local_scale_.x / 2);
      float scaleY = (local_scale_.y / 2);
      bounding_box_.x = local_position_.x - scaleX; // Min-X
      bounding_box_.y = local_position_.y - scaleY; // Min-Y
      bounding_box_.z = local_position_.x + scaleX; // Max-X
      bounding_box_.w = local_position_.y + scaleY; // Max-Y
   }

   glm::mat4 model_transform_ = glm::mat4(1.0f);

   glm::vec4 bounding_box_;

   glm::vec3 local_position_ = vec3_zero;
   glm::vec3 local_scale_ = vec3_one;
   glm::vec3 local_rotation_ = vec3_zero;
private:
   bool has_changed_ = true;
};
