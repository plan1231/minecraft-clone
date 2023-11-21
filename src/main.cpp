

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include "Application.h"
#include <iostream>


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main() {

    stbi_set_flip_vertically_on_load(true);
    Application app;
    app.run();
}
