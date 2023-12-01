

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include "Application.h"
#include <iostream>

#include "collide.h"
#include "components/physics/geometry/AABB.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main() {

    stbi_set_flip_vertically_on_load(true);
    auto [collided, ctime, normal] = collide(
        AABB{.min = glm::vec3{0.0, 0.0, 0.0}, .max = glm::vec3{1.0, 1.0, 1.0}},
        AABB{.min=glm::vec3{2.0, 2.0, 1.0}, .max={3.0, 3.0, 1.0}},
        glm::vec3{-2.0, 0.0, 0.0});
    std::cout << collided << std::endl;
    Application app;
    app.run();
}
