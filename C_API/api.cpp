#include <stdio.h>
#include "cpp_code/api_impl.hpp"
#include "api.h"

// ______ test calls ______

#ifdef __cplusplus
extern "C" {
#endif

EXPORT_API const char* hello_get_message(void) {
    return "Hello from the C API DLL!";
}

EXPORT_API void hello_say_hello(const char* name) {
    printf("Hello, %s! This message comes from the DLL.\n", name);
}

EXPORT_API void get_info_from_cpp(void) {
    printf("get_info_from_cpp called\n");
    int h = getHay();
    printf("hay=%d\n", h);
}

// ______ Image Handeling ______

EXPORT_API void transverImage(const uint8_t* data, int width, int height, int stride) {
    printf("transverImage called\n");
    printf("transverImage: width=%d height=%d stride=%d\n", width, height, stride);
    // print first up-to-10 bytes of the first row (guard width)
    int show = width < 10 ? width : 10;
    if (show > 0 && data != nullptr) {
        printf("transverImage: first row bytes:");
        for (int i = 0; i < show; ++i) printf(" %u", static_cast<unsigned>(data[i]));
        printf("\n");
    }
    constructImage(data, width, height, stride);
}

#ifdef __cplusplus
}
#endif