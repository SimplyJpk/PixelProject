#pragma once
#include <GL/glew.h>

class RenderObject
{
public:
   // Vertex Buffer Object
   unsigned int VBO{};
   // Vertex Array Object
   unsigned int VAO{};
   // Element Buffer Object
   unsigned int EBO{};

   RenderObject()
   {
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);
   }

   ~RenderObject()
   {
      glDeleteVertexArrays(1, &VAO);
      glDeleteBuffers(1, &VBO);
      glDeleteBuffers(1, &EBO);
   }
};
