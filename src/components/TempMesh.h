//
// Created by Phil Lan on 2023-12-06.
//
#pragma once
#include <vector>

/**
 * \brief Represents a mesh in main memory that hasn't been uploaded to the gpu yet.
 * \tparam T Struct for per-vertex data
 */
template<typename T>
struct TempMesh {
    std::vector<T> vertices;
    std::vector<unsigned int> indices;
};