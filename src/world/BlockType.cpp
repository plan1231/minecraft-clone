//
// Created by Phil Lan on 2023-11-08.
//
#include "BlockType.h"
constexpr int ITEMS_PER_ROW = 16;
constexpr float ITEM_LENGTH = 1.0f / ITEMS_PER_ROW;

BlockTypeInfo BlockTypeInfo::blockTypes[] = {
    // AIR
    BlockTypeInfo{
        .isTransparent = true,
        .isPlant = false,
    },
    // DIRT
    BlockTypeInfo{
        .isTransparent = false,
        .isPlant = false,
     //   .textureIndices = {16, 16, 32, 0, 16, 16},
        .textureIndices = {16, 16, 16, 16, 32, 0},

    }
};

BlockTypeInfo BlockTypeInfo::get(BlockType blockType) {
    return blockTypes[static_cast<uint32_t>(blockType)];
}

/* Corners
 * 0 - Bottom left
 * 1 - Bottom right
 * 2 - Top right
 * 3 - Top left
 */
glm::vec2 BlockTypeInfo::textCoords(Face face, int corner) const {
    int row = textureIndices[static_cast<int>(face)] / ITEMS_PER_ROW;
    int col = textureIndices[static_cast<int>(face)] % ITEMS_PER_ROW;

    glm::vec2 res = {ITEM_LENGTH * col,ITEM_LENGTH * row};

    if(corner == 1 || corner == 2) {
        res.x += ITEM_LENGTH;
    }
    if(corner >= 2) {
        res.y += ITEM_LENGTH;
    }
    return res;
}
