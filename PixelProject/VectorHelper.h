#pragma once
#include <glm/ext/matrix_transform.hpp>

namespace PixelProject::Constant::Vector
{
   constexpr glm::vec3 vec3_up = glm::vec3(0, 1, 0);
   constexpr glm::vec3 vec3_down = glm::vec3(0, -1, 0);
   constexpr glm::vec3 vec3_right = glm::vec3(1, 0, 0);
   constexpr glm::vec3 vec3_left = glm::vec3(-1, 0, 0);
   constexpr glm::vec3 vec3_forward = glm::vec3(0, 0, 1);
   constexpr glm::vec3 vec3_back = glm::vec3(0, 0, -1);
   
   constexpr glm::vec3 vec3_one = glm::vec3(1, 1, 1);
   constexpr glm::vec3 vec3_zero = glm::vec3(0, 0, 0);
}