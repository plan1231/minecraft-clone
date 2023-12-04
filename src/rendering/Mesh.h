//
// Created by Phil Lan on 2023-10-29.
//

#ifndef MINECRAFT_CLONE_GEOMETRY_H
#define MINECRAFT_CLONE_GEOMETRY_H

#include <vector>
#include "glad/glad.h"


struct VertexAttribute {
    int size;
    int type;
    unsigned char normalized;
    int stride;
    const void *pointer;
};

typedef std::vector<VertexAttribute> VertexAttributes;

class Mesh {
    unsigned int vbo, vao, ebo;
    int drawMode;
    unsigned int drawCount;
public:
    Mesh(const VertexAttributes &attributes, int drawMode = GL_TRIANGLES);

    void draw() const;

    void bufferData(const std::vector<float> &vertices, const std::vector<unsigned int> &indices,
                    unsigned int usage = GL_STATIC_DRAW);

    void bufferData(const void* data, unsigned int bytes, const std::vector<unsigned int> &indices,
                    unsigned int usage = GL_STATIC_DRAW);
};

using MeshPtr = std::shared_ptr<Mesh>;

#endif //MINECRAFT_CLONE_GEOMETRY_H
