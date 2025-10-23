#pragma once

#ifdef __cplusplus // if this is compiled by a C++ compiler
  extern "C" { // tell the C++ compiler to treat the following code as C code
#endif

#ifdef _WIN32
  #ifdef BUILDING_API
    #define EXPORT_API __declspec(dllexport)
  #else
    #define EXPORT_API __declspec(dllimport)
  #endif
#else
  #define EXPORT_API
#endif

// Returns a simple greeting message
EXPORT_API const char* hello_get_message(void);

// Prints a greeting to the console
EXPORT_API void hello_say_hello(const char* name);

#ifdef __cplusplus
}
#endif