//
// Created by Phil Lan on 2023-10-29.
//

#ifndef MINECRAFT_CLONE_TEXTURE_H
#define MINECRAFT_CLONE_TEXTURE_H
#include <string>
#include <memory>
#include <entt/entt.hpp>
class Texture {
protected:
    unsigned int texture;

public:
    Texture(const std::string& filename);

    Texture(const Texture &) = delete;
    Texture& operator=(Texture &) = delete;

    void use(unsigned int texUnit) const;
};

using TexturePtr = std::shared_ptr<Texture>;
#endif //MINECRAFT_CLONE_TEXTURE_H
