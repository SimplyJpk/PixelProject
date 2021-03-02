#include "Camera.h"

Camera::Camera()
{
   SetLookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}



Camera::~Camera()
{
}

void Camera::Update(float delta)
{

}

void Camera::SetOrtho(float left, float right, float bottom, float top)
{
   projection_transform_ = glm::ortho(left, right, bottom, top);
   isOrtho = true;
   UpdateProjectionView();
}

// Set the camera Perspective
void Camera::SetPerspective(float field_of_view, float aspect_ratio, float near, float far)
{
   projection_transform_ = glm::perspective(field_of_view, aspect_ratio, near, far);
   isOrtho = false;
   UpdateProjectionView();
}

// Look at vector passed through
void Camera::SetLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up)
{
   view_transform_ = glm::lookAt(from, to, up);
   UpdateProjectionView();
   UpdateWorldTransform();
}

// Sets camera Position
void Camera::SetPosition(glm::vec3 position)
{
   world_transform_[3] = glm::vec4(position, 1);
   view_transform_ = glm::inverse(world_transform_);
   UpdateProjectionView();
   UpdateWorldTransform();
}

glm::mat4 Camera::GetWorldTransform()
{
   return world_transform_;
}

glm::mat4 Camera::GetView()
{
   return view_transform_;
}

glm::mat4 Camera::GetProjection()
{
   return projection_transform_;
}

glm::mat4 Camera::GetProjectionView()
{
   return projection_view_transform_;
}

glm::vec4* Camera::GetFrustumPlanes()
{
   return frustum_;
}

// Set world Transfrom and update the projection view
void Camera::SetWorldTrans(glm::mat4 transform)
{
   world_transform_ = transform;
   view_transform_ = glm::inverse(world_transform_);
   UpdateProjectionView();
   UpdateWorldTransform();
}

glm::vec3 Camera::GetPosition()
{
   glm::vec3 pos = Row(3);
   return pos;
}

// returns a vector from the WorldTransform matrix based on the row
glm::vec3 Camera::Row(int row)
{
   return glm::vec3(world_transform_[row].x, world_transform_[row].y, world_transform_[row].z);
}


// Script that generates the Frustum from the Camera for culling
void Camera::GenerateFrustum()
{
   // right side
   frustum_[0] = glm::vec4(projection_view_transform_[0][3] - projection_view_transform_[0][0],
      projection_view_transform_[1][3] - projection_view_transform_[1][0],
      projection_view_transform_[2][3] - projection_view_transform_[2][0],
      projection_view_transform_[3][3] - projection_view_transform_[3][0]);
   // left side 
   frustum_[1] = glm::vec4(projection_view_transform_[0][3] + projection_view_transform_[0][0],
      projection_view_transform_[1][3] + projection_view_transform_[1][0],
      projection_view_transform_[2][3] + projection_view_transform_[2][0],
      projection_view_transform_[3][3] + projection_view_transform_[3][0]);
   // top 
   frustum_[2] = glm::vec4(projection_view_transform_[0][3] - projection_view_transform_[0][1],
      projection_view_transform_[1][3] - projection_view_transform_[1][1],
      projection_view_transform_[2][3] - projection_view_transform_[2][1],
      projection_view_transform_[3][3] - projection_view_transform_[3][1]);
   // bottom 
   frustum_[3] = glm::vec4(projection_view_transform_[0][3] + projection_view_transform_[0][1],
      projection_view_transform_[1][3] + projection_view_transform_[1][1],
      projection_view_transform_[2][3] + projection_view_transform_[2][1],
      projection_view_transform_[3][3] + projection_view_transform_[3][1]);
   // far 
   frustum_[4] = glm::vec4(projection_view_transform_[0][3] - projection_view_transform_[0][2],
      projection_view_transform_[1][3] - projection_view_transform_[1][2],
      projection_view_transform_[2][3] - projection_view_transform_[2][2],
      projection_view_transform_[3][3] - projection_view_transform_[3][2]);
   // near 
   frustum_[5] = glm::vec4(projection_view_transform_[0][3] + projection_view_transform_[0][2],
      projection_view_transform_[1][3] + projection_view_transform_[1][2],
      projection_view_transform_[2][3] + projection_view_transform_[2][2],
      projection_view_transform_[3][3] + projection_view_transform_[3][2]);
   // plane normalisation, based on length of normal 
   for (int i = 0; i < 6; i++)
   {
      float d = glm::length(glm::vec3(frustum_[i]));
      frustum_[i] /= d;
   }
}

void Camera::UpdateProjectionView()
{
   if (!isOrtho)
   {
      projection_view_transform_ = projection_transform_ * view_transform_;
      // We've moved projection, re-generate Frustum
      GenerateFrustum();
   }
}
void Camera::UpdateWorldTransform()
{
   world_transform_ = glm::inverse(view_transform_);
}

