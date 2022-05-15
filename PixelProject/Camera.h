#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <SDL.h>

#include "IVec2.h"

class Camera
{
public:
   Camera();
   ~Camera();

   // Update camera Position and Rotation
   virtual void Update(float delta);
   void SetOrtho(float left, float right, float bottom, float top);

   void ApplyZoom(float zoom);
   float GetZoom() const { return cameraZoom; }

   // Set Perspective view of camera
   void SetPerspective(float field_of_view, float aspect_ratio, float near, float far);
   // Look at Vector
   void SetLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up);
   // Set position of Camera
   void SetPosition(glm::vec3 position);

   glm::mat4 GetWorldTransform();
   glm::mat4 GetView();
   glm::mat4 GetProjection();
   glm::mat4 GetProjectionView();

   void SetWorldTrans(glm::mat4 transform);

   glm::vec3 GetPosition();
   glm::vec3 Row(int row);
   glm::vec4* GetFrustumPlanes();

protected:
   float width, height;

   float cameraZoom = 1.0f;

   bool isOrtho = false;
   // Generates the Frustum when ever the Camera moves
   void GenerateFrustum();
   glm::vec4 frustum_[6];
private:
   glm::mat4 world_transform_;
   glm::mat4 view_transform_;
   glm::mat4 projection_transform_;
   glm::mat4 projection_view_transform_;

   void UpdateProjectionView();
   void UpdateWorldTransform();
};

