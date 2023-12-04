

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include "Application.h"
#include <iostream>

int main() {

    stbi_set_flip_vertically_on_load(true);
    Application app;
    app.run();
}
