//
// Created by Phil Lan on 2023-11-08.
//

#ifndef MINECRAFT_CLONE_BLOCKTYPE_H
#define MINECRAFT_CLONE_BLOCKTYPE_H
#include <array>
#include <glm/glm.hpp>
enum class BlockType: uint32_t {
    AIR,
    DIRT,
};


struct BlockTypeInfo {
    static BlockTypeInfo blockTypes[];
    static BlockTypeInfo get(BlockType blockType);
    bool isTransparent;
    bool isPlant;

    union {
        // left, right, top, bottom, front, back
        std::array<int, 6> textureIndices; // for blocks
        int textureIndex; // for plants
    };

    glm::vec2 textCoords(int face, int corner) const;
};


#endif //MINECRAFT_CLONE_BLOCKTYPE_H
