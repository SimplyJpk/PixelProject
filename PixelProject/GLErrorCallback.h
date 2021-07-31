#pragma once
#include <cstdio>

#include "ConsoleUtility.h"
#include "GL/glew.h"

class GLErrorCallback
{
public:

   inline static bool use_colour = false;

   static void LinkCallback(bool useColour = false)
   {
      use_colour = useColour;

      // GL_DEBUG_OUTPUT - much faster
      // GL_DEBUG_OUTPUT_SYNCHRONOUS - can be stacktraced
      glEnable(GL_DEBUG_OUTPUT);
      glDebugMessageCallback(MessageCallback, 0);
   }

   static void GLAPIENTRY
      MessageCallback(GLenum source,
         GLenum type,
         GLuint id,
         GLenum severity,
         GLsizei length,
         const GLchar* message,
         const void* userParam)
   {
      const std::string source_ = GetErrorName(source);
      const std::string type_ = GetErrorType(type);
      const std::string severity_ = GetErrorSeverity(severity);

      static char error_message[400];

      sprintf_s(error_message, sizeof(error_message), "GL CALLBACK: %s type = %s, severity = %s, message = %s\n", source_.c_str(), type_.c_str(), severity_.c_str(), message);
      if (use_colour)
         ConsoleUtility::PrintText(error_message, GetErrorColour(severity));
      else
         ConsoleUtility::PrintText(error_message);
   }

   static std::string GetErrorName(const GLenum source)
   {
      switch (source)
      {
      case GL_DEBUG_SOURCE_API:
         return "API";
      case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
         return "WINDOW SYSTEM";
      case GL_DEBUG_SOURCE_SHADER_COMPILER:
         return "SHADER COMPILER";
      case GL_DEBUG_SOURCE_THIRD_PARTY:
         return "THIRD PARTY";
      case GL_DEBUG_SOURCE_APPLICATION:
         return "APPLICATION";

      case GL_DEBUG_SOURCE_OTHER:
      default:
         return "UNKNOWN";
      }
   }

   static std::string GetErrorType(const GLenum type)
   {
      switch (type)
      {
      case GL_DEBUG_TYPE_ERROR:
         return "ERROR";
      case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
         return "DEPRECATED BEHAVIOUR";
      case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
         return "UNDEFINED BEHAVIOUR";
      case GL_DEBUG_TYPE_PORTABILITY:
         return "PORTABILITY";
      case GL_DEBUG_TYPE_PERFORMANCE:
         return "PERFORMANCE";
      case GL_DEBUG_TYPE_OTHER:
         return "OTHER";
      case GL_DEBUG_TYPE_MARKER:
         return "MARKER";
      default:
         return "UNKNOWN";
      }
   }

   static std::string GetErrorSeverity(const GLenum severity)
   {
      switch (severity)
      {
      case GL_DEBUG_SEVERITY_NOTIFICATION:
         return "NOTIFICATION";
      case GL_DEBUG_SEVERITY_LOW:
         return "LOW";
      case GL_DEBUG_SEVERITY_MEDIUM:
         return "MEDIUM";
      case GL_DEBUG_SEVERITY_HIGH:
         return "HIGH";
      default:
         return "UNKNOWN";
      }
   }

   static ConsoleColour GetErrorColour(GLenum severity)
   {
      switch (severity)
      {
      case GL_DEBUG_SEVERITY_NOTIFICATION:
         return ConsoleColour::Grey;
      case GL_DEBUG_SEVERITY_LOW:
         return ConsoleColour::LightYellow;
      case GL_DEBUG_SEVERITY_MEDIUM:
         return ConsoleColour::Yellow;
      case GL_DEBUG_SEVERITY_HIGH:
         return ConsoleColour::Red;
      default:
         return ConsoleColour::LightRed;
      }
   }
};
