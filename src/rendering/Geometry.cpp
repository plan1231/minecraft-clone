//
// Created by Phil Lan on 2023-10-29.
//

#include "Geometry.h"

Geometry::Geometry(const VertexAttributes &attributes, int drawMode)
        : drawMode(drawMode) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    for (int i = 0; i < attributes.size(); i++) {
        const VertexAttribute &attribute = attributes[i];
        glVertexAttribPointer(i, attribute.size, attribute.type, attribute.normalized, attribute.stride,
                              attribute.pointer);
        glEnableVertexAttribArray(i);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Geometry::draw() const {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(drawMode, drawCount, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void
Geometry::bufferData(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, unsigned int usage) {
    bufferData(reinterpret_cast<const char *>(vertices.data()), sizeof(float) * vertices.size(), indices, usage);
}

void Geometry::bufferData(const char *data, unsigned int bytes, const std::vector<unsigned int> &indices,
                          unsigned int usage) {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ARRAY_BUFFER, bytes, data, usage);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    drawCount = indices.size();
}
