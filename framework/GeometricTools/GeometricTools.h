#ifndef GEOMETRICTOOLS_H_
#define GEOMETRICTOOLS_H_

#define ON_X_Y_AXIS 0
#define ON_X_Z_AXIS 1
#define ON_Y_Z_AXIS 2

#include <array>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "MatrixOperations.h"

namespace GeometricTools
{
    inline std::array<float, 3 * 2 * 6> axisVertices = {
        // positions        // colors
         0.f, 0.f, 0.f,     1.f, 0.f, 0.f,  // X axis (red)
         1.f, 0.f, 0.f,     1.f, 0.f, 0.f,

         0.f, 0.f, 0.f,     0.f, 1.f, 0.f,  // Y axis (green)
         0.f, 1.f, 0.f,     0.f, 1.f, 0.f,

         0.f, 0.f, 0.f,     0.f, 0.f, 1.f,  // Z axis (blue)
         0.f, 0.f, 1.f,     0.f, 0.f, 1.f
    };


    inline std::array<float, 3 * 2> UnitTriangle2D = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.0f, 0.5f }; // [2,3]

    inline std::array<float, 3 * 2 * 2> UnitSquare2D = {
        // triangle 1
        -0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, -0.5f,
        // triganle 2
        0.5f, 0.5f,
        -0.5f, 0.5f,
        0.5f, -0.5f 
    }; // [2,6]

    inline std::array<float, 11 * 4> UnitSquare3DWithTexCoordsVertices = {
        // pos              // Normals        // T.C.     // Color
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // close left
        -0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // far left
         0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // far right
         0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // close right
    };

    inline std::array<unsigned int, 3 * 2> UnitSquare3DWithTexCoordsIndices = {
        0, 1, 2,
        3, 0, 2,
    };

    inline std::array<float, 3 * 4 * 6> UnitCube3DVertices = {
        // front
        -0.5,  0.5,  0.5,
        -0.5, -0.5,  0.5,
        0.5, -0.5,  0.5,
        0.5,  0.5,  0.5,
        // back
        -0.5,  0.5, -0.5,
        -0.5, -0.5, -0.5,
        0.5, -0.5, -0.5,
        0.5,  0.5, -0.5,
        // left
        -0.5,  0.5, -0.5,
        -0.5, -0.5, -0.5,
        -0.5, -0.5,  0.5,
        -0.5,  0.5,  0.5,
        // right
        0.5,  0.5, -0.5,
        0.5, -0.5, -0.5,
        0.5, -0.5,  0.5,
        0.5,  0.5,  0.5,
        // top
        -0.5,  0.5,  0.5,
        -0.5,  0.5, -0.5,
        0.5,  0.5, -0.5,
        0.5,  0.5,  0.5,
        // bottom
        -0.5, -0.5,  0.5,
        -0.5, -0.5, -0.5,
        0.5, -0.5, -0.5,
        0.5, -0.5,  0.5,
    };

    inline std::array<unsigned int, 3 * 2 * 6> UnitCube3DIndices = {
        // first triangle    second triangle
            0, 1, 2,          2, 3, 0,       // front
            6, 5, 4,          4, 7, 6,       // back
            8, 9,10,         10,11, 8,       // left
           14,13,12,         12,15,14,       // right
           18,17,16,         16,19,18,       // top
           20,21,22,         22,23,20,       // bottom
    };

    // Cube with 24 vertices (each vertex repeated 3 times for each face it participates in)
    // Each vertex has: position (x,y,z) + normal (nx,ny,nz) = 6 floats
    inline std::array<float, 3 * 24 * 2> UnitCube3D24WNormals = {
        // FRONT FACE (normal: 0, 0, 1)
        // Positions          // Normals
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // Bottom-left
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // Bottom-right
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // Top-right
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // Top-left

        // BACK FACE (normal: 0, 0, -1)
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // Bottom-right
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // Bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // Top-left
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // Top-right

        // TOP FACE (normal: 0, 1, 0)
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  // Front-left
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  // Front-right
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  // Back-right
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  // Back-left

        // BOTTOM FACE (normal: 0, -1, 0)
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  // Back-left
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  // Back-right
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  // Front-right
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  // Front-left

        // RIGHT FACE (normal: 1, 0, 0)
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  // Bottom-front
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  // Bottom-back
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  // Top-back
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  // Top-front

        // LEFT FACE (normal: -1, 0, 0)
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  // Bottom-back
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  // Bottom-front
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  // Top-front
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f   // Top-back
    };

    // Topology indices for 24-vertex cube (6 faces * 2 triangles * 3 vertices = 36 indices)
    inline std::array<unsigned int, 6 * 3 * 2> UnitCube3DTopologyTriangles24 = {
        // FRONT FACE
        0, 1, 2,    // Triangle 1
        2, 3, 0,    // Triangle 2

        // BACK FACE
        4, 5, 6,    // Triangle 1
        6, 7, 4,    // Triangle 2

        // TOP FACE
        8, 9, 10,   // Triangle 1
        10, 11, 8,  // Triangle 2

        // BOTTOM FACE
        12, 13, 14, // Triangle 1
        14, 15, 12, // Triangle 2

        // RIGHT FACE
        16, 17, 18, // Triangle 1
        18, 19, 16, // Triangle 2

        // LEFT FACE
        20, 21, 22, // Triangle 1
        22, 23, 20  // Triangle 2
    };


    /**
     * @brief Creates a UnitCube3D with an aspect ratio of width:height
     *
     * Generates a std::pair<std::array<float, 3 * 3 * 2 * 3 * 4* 6>, std::array<unsigned int, 3 * 2 * 6>>
     * Where std::array<float, 3 * 3 * 2 * 3 * 4 * 6> -> VBO (vertex positions (3), normals(3), tex  coords(2) and color (3))
     * Where std::array<unsigned int, 3 * 2 * 6> -> EBO (triangle indices)
     * Default values returns a cube
     *
     * @param width scale on X axis
     * @param height scale on Y axis
     * @return pair with VBO and EBO
     */
    inline std::pair<std::array<float, 11 * 4 * 6>, std::array<unsigned int, 3 * 2 * 6>> GetUnitCube3D(glm::vec3 color = glm::vec3{1.0f,1.0f,1.0f})
    {
        float r = std::clamp(color.r, 0.0f, 1.0f);
        float g = std::clamp(color.g, 0.0f, 1.0f);
        float b = std::clamp(color.b, 0.0f, 1.0f);


        std::array<float, 11 * 4 * 6> VBO = {
            // FRONT FACE (normal: 0, 0, 1)
            // Positions          // Normals           // T.C.      // Color
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,  r, g, b,  // Bottom-left
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,  r, g, b,  // Bottom-right
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,  r, g, b,  // Top-right
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,  r, g, b,  // Top-left

            // BACK FACE (normal: 0, 0, -1)                                      
             0.5f, -0.5f , -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,  r, g, b,  // Bottom-right
            -0.5f, -0.5f , -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,  r, g, b,  // Bottom-left
            -0.5f,  0.5f , -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,  r, g, b,  // Top-left
             0.5f,  0.5f , -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,  r, g, b,  // Top-right

             // TOP FACE(normal: 0, 1, 0)                                         
             -0.5f,  0.5f ,  0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f,  r, g, b,  // Front-left
              0.5f,  0.5f ,  0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,  r, g, b,  // Front-right
              0.5f,  0.5f , -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f,  r, g, b,  // Back-right
             -0.5f,  0.5f , -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,  r, g, b,  // Back-left

             // BOTTOM FAE (normal: 0, -1, 0)                                     
             -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,  0.0f, 0.0f,  r, g, b,  // Back-left
              0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,  1.0f, 0.0f,  r, g, b,  // Back-right
              0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,  1.0f, 1.0f,  r, g, b,  // Front-right
             -0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,  0.0f, 1.0f,  r, g, b,  // Front-left

             // RIGHT FACE (normal: 1, 0, 0)                                      
              0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f,  r, g, b,  // Bottom-front
              0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  r, g, b,  // Bottom-back
              0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  r, g, b,  // Top-back
              0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  r, g, b,  // Top-front

              // LEFT FACE(normal: -1, 0, 0)                                       
              -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, r, g, b,  // Bottom-back
              -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, r, g, b,  // Bottom-front
              -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, r, g, b,  // Top-front
              -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, r, g, b,  // Top-back
        };

        std::array<unsigned int, 6 * 3 * 2> EBO = {
            // FRONT FACE
            0, 1, 2,    // Triangle 1
            2, 3, 0,    // Triangle 2

            // BACK FACE
            4, 5, 6,    // Triangle 1
            6, 7, 4,    // Triangle 2

            // TOP FACE
            8, 9, 10,   // Triangle 1
            10, 11, 8,  // Triangle 2

            // BOTTOM FACE
            12, 13, 14, // Triangle 1
            14, 15, 12, // Triangle 2

            // RIGHT FACE
            16, 17, 18, // Triangle 1
            18, 19, 16, // Triangle 2

            // LEFT FACE
            20, 21, 22, // Triangle 1
            22, 23, 20  // Triangle 2
        };

        return std::make_pair(VBO, EBO);
    }

    /**
     * @brief Creates a UnitGrid of dimension (divisionsX x divisionsY)
     *
     * Generates a std::pair<std::vector<float>, std::vector<int>>
     * Where std::vector<float> -> VBO (vertex positions)
     * Where std::vector<int> -> EBO (triangle indices)
     *
     * @param divisionsX Number of divisions along X axis
     * @param divisionsY Number of divisions along Y axis
     * @return pair with VBO and EBO
     */
    inline std::pair<std::vector<float>, std::vector<unsigned int>> GenerateUnitGrid2D(int divisionsX, int divisionsY)
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        // Calculate spacing between tiles
        const float stepX = 1.0f / divisionsX;
        const float stepY = 1.0f / divisionsY;

        unsigned int vertexIndex = 0;

        // Create separate vertices for each tile (no sharing between tiles)
        for (unsigned int row = 0; row < divisionsY; ++row)
        {
            for (unsigned int col = 0; col < divisionsX; ++col)
            {
                // Calculate tile position
                float x = -0.5f + row * stepX;
                float y = 0.5f - col * stepY;

                // Create 4 vertices for this tile
                // Bottom-left
                vertices.insert(vertices.end(), { x, y - stepY });
                // Bottom-right
                vertices.insert(vertices.end(), { x + stepX, y - stepY });
                // Top-right
                vertices.insert(vertices.end(), { x + stepX, y });
                // Top-left
                vertices.insert(vertices.end(), { x, y });

                // Create indices for 2 triangles that form this tile
                // First triangle (bottom-left, bottom-right, top-left)
                indices.insert(indices.end(), { vertexIndex, vertexIndex + 1, vertexIndex + 3 });
                // Second triangle (bottom-right, top-right, top-left)
                indices.insert(indices.end(), { vertexIndex + 1, vertexIndex + 2, vertexIndex + 3 });

                vertexIndex += 4; // Move to next set of 4 vertices
            }
        }

        return std::make_pair(vertices, indices);
    }

    inline std::pair<std::vector<float>, std::vector<unsigned int>> GenerateUnitGrid2DWTCoords(int divisionsX, int divisionsY)
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        // Calculate spacing between tiles
        const float stepX = 1.0f / divisionsX;
        const float stepY = 1.0f / divisionsY;

        float s, t;

        unsigned int vertexIndex = 0;

        // Create separate vertices for each tile (no sharing between tiles)
        for (int row = 0; row < divisionsY; ++row)
        {
            for (int col = 0; col < divisionsX; ++col)
            {
                // Calculate tile position
                float x = -0.5f + row * stepX;
                float y = 0.5f - col * stepY;

                // Create 4 vertices for this tile
                // Bottom-left
                s = 0.0f;
                t = 0.0f;
                vertices.insert(vertices.end(), { x, y - stepY, s, t });
                // Bottom-right
                s = 1.0f;
                t = 0.0f;
                vertices.insert(vertices.end(), { x + stepX, y - stepY, s, t });
                // Top-right
                s = 1.0f;
                t = 1.0f;
                vertices.insert(vertices.end(), { x + stepX, y, s, t });
                // Top-left
                s = 0.0f;
                t = 1.0f;
                vertices.insert(vertices.end(), { x, y, s, t });

                // Create indices for 2 triangles that form this tile
                // First triangle (bottom-left, bottom-right, top-left)
                indices.insert(indices.end(), { vertexIndex, vertexIndex + 1, vertexIndex + 3 });
                // Second triangle (bottom-right, top-right, top-left)
                indices.insert(indices.end(), { vertexIndex + 1, vertexIndex + 2, vertexIndex + 3 });

                vertexIndex += 4; // Move to next set of 4 vertices
            }
        }

        return std::make_pair(vertices, indices);
    }

    inline std::pair<std::vector<float>, std::vector<unsigned int>> GenerateGrid3D(int numTilesFirstAxis, int numTilesSecondAxis, glm::vec3 color, glm::vec3 normals, float posOnAxis = 0.0f, unsigned int alginment = ON_X_Z_AXIS)
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        float s, t;

        unsigned int vertexIndex = 0;

        // Create separate vertices for each tile (no sharing between tiles)
        for (int row = 0; row < numTilesSecondAxis; ++row)
        {
            for (int col = 0; col < numTilesFirstAxis; ++col)
            {
                // Calculate the offset needed to center it
                float offsetCol = col - numTilesFirstAxis / 2.0f + 0.5f;
                float offsetRow = row - numTilesSecondAxis / 2.0f + 0.5f;

                glm::vec3 v0;
                glm::vec3 v1;
                glm::vec3 v2;
                glm::vec3 v3;

                switch (alginment)
                {
                case ON_X_Y_AXIS:

                    v0 = glm::vec3(offsetRow - 0.5f, offsetCol - 0.5f, posOnAxis);
                    v1 = glm::vec3(offsetRow + 0.5f, offsetCol - 0.5f, posOnAxis);
                    v2 = glm::vec3(offsetRow + 0.5f, offsetCol + 0.5f, posOnAxis);
                    v3 = glm::vec3(offsetRow - 0.5f, offsetCol + 0.5f, posOnAxis);

                    break;

                case ON_X_Z_AXIS:
                    v0 = glm::vec3(offsetRow - 0.5f, posOnAxis, offsetCol - 0.5f);
                    v1 = glm::vec3(offsetRow + 0.5f, posOnAxis, offsetCol - 0.5f);
                    v2 = glm::vec3(offsetRow + 0.5f, posOnAxis, offsetCol + 0.5f);
                    v3 = glm::vec3(offsetRow - 0.5f, posOnAxis, offsetCol + 0.5f);
                    break;

                case ON_Y_Z_AXIS:
                    v0 = glm::vec3(posOnAxis, offsetRow - 0.5f, offsetCol - 0.5f);
                    v1 = glm::vec3(posOnAxis, offsetRow + 0.5f, offsetCol - 0.5f);
                    v2 = glm::vec3(posOnAxis, offsetRow + 0.5f, offsetCol + 0.5f);
                    v3 = glm::vec3(posOnAxis, offsetRow - 0.5f, offsetCol + 0.5f);

                    break;

                default:
                    v0 = glm::vec3(offsetRow - 0.5f, posOnAxis, offsetCol - 0.5f);
                    v1 = glm::vec3(offsetRow + 0.5f, posOnAxis, offsetCol - 0.5f);
                    v2 = glm::vec3(offsetRow + 0.5f, posOnAxis, offsetCol + 0.5f);
                    v3 = glm::vec3(offsetRow - 0.5f, posOnAxis, offsetCol + 0.5f);

                    break;
                }

                // Create 4 vertices for this tile
                // Bottom-left
                s = 0.0f;
                t = 0.0f;

                vertices.insert(vertices.end(), { v0.x, v0.y, v0.z, normals.x, normals.y, normals.z, s, t, color.r, color.g, color.b });
                // Bottom-right
                s = 1.0f;
                t = 0.0f;
                vertices.insert(vertices.end(), { v1.x, v1.y, v1.z, normals.x, normals.y, normals.z, s, t, color.r, color.g, color.b });
                // Top-right
                s = 1.0f;
                t = 1.0f;
                vertices.insert(vertices.end(), { v2.x, v2.y, v2.z, normals.x, normals.y, normals.z, s, t, color.r, color.g, color.b });
                // Top-left
                s = 0.0f;
                t = 1.0f;
                vertices.insert(vertices.end(), { v3.x, v3.y, v3.z, normals.x, normals.y, normals.z, s, t, color.r, color.g, color.b });

                // Create indices for 2 triangles that form this tile
                // First triangle (bottom-left, bottom-right, top-left)
                indices.insert(indices.end(), { vertexIndex, vertexIndex + 1, vertexIndex + 3 });
                // Second triangle (bottom-right, top-right, top-left)
                indices.insert(indices.end(), { vertexIndex + 1, vertexIndex + 2, vertexIndex + 3 });

                vertexIndex += 4; // Move to next set of 4 vertices
            }
        }
        return std::make_pair(vertices, indices);
    }

    inline std::pair< std::pair<std::array<float, 11 * 4>, std::vector<glm::mat4>> , std::array<unsigned int, 6>> GenerateGrid3DForInstanceDrawing(int numTilesFirstAxis, int numTilesSecondAxis, glm::vec3 color, unsigned int alginment = ON_X_Z_AXIS, float posOnAxis = 0.0f, glm::vec3 rotationVec = glm::vec3{1.0f,0.0f,0.0f}, float rotation = 0.0f)
    {
        std::vector<glm::mat4> models;

        std::array<unsigned int, 6> indices = {
            0, 1, 2,
            3, 0, 2,
        };

        std::array<float, 11 * 4> vertices = {
            // pos              // Normals        // T.C.     // Color
            -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, color.r, color.g, color.b, // close left
            -0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, color.r, color.g, color.b, // far left
             0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, color.r, color.g, color.b, // far right
             0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, color.r, color.g, color.b, // close right
        };

        // Create separate vertices for each tile (no sharing between tiles)
        for (int row = 0; row < numTilesSecondAxis; ++row)
        {
            for (int col = 0; col < numTilesFirstAxis; ++col)
            {
                glm::vec3 scale = glm::vec3{ 1.0f };
                glm::vec3 pos = glm::vec3{ 0.0f };

                glm::mat4 model;
                // Calculate the offset needed to center it
                float offsetCol = col - numTilesFirstAxis / 2.0f + 0.5f;
                float offsetRow = row - numTilesSecondAxis / 2.0f + 0.5f;
                

                switch (alginment)
                {
                case ON_X_Z_AXIS:
                    pos = glm::vec3{ offsetRow, posOnAxis, offsetCol };
                    break;

                case ON_X_Y_AXIS:
                    pos = glm::vec3{ offsetRow, offsetCol, posOnAxis };
                    break;

                case ON_Y_Z_AXIS:
                    pos = glm::vec3{ posOnAxis, offsetRow, offsetCol };
                    break;

                }

                model = MatrixOperations::getTransformedMatrix(scale, rotationVec, rotation, pos);
                models.push_back(model);
            }
        }
        auto vertexData = std::make_pair(vertices, models);
        return std::make_pair(vertexData, indices);
    }

    // Generate a UV sphere (default size = 1.0, from -0.5 to 0.5)
    inline std::pair<std::vector<float>, std::vector<unsigned int>> CreateSphere(float size = 1.0f, unsigned int segments = 32, unsigned int rings = 16,
        glm::vec3 color = glm::vec3{ 1.0f, 1.0f, 1.0f })

    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        float radius = size * 0.5f;  // Convert size to radius


        // Generate vertices
        for (unsigned int ring = 0; ring <= rings; ++ring) {
            float phi = glm::pi<float>() * static_cast<float>(ring) / static_cast<float>(rings);
            float y = radius * std::cos(phi);
            float ringRadius = radius * std::sin(phi);

            for (unsigned int seg = 0; seg <= segments; ++seg) {
                float theta = 2.0f * glm::pi<float>() * static_cast<float>(seg) / static_cast<float>(segments);

                // Position
                float x = ringRadius * std::cos(theta);
                float z = ringRadius * std::sin(theta);
                glm::vec3 position(x, y, z);

                // Normal (normalized position for sphere centered at origin)
                glm::vec3 normal = glm::normalize(position);

                // Texture coordinates
                float u = static_cast<float>(seg) / static_cast<float>(segments);
                float v = static_cast<float>(ring) / static_cast<float>(rings);
                glm::vec2 texCoords(u, v);

                vertices.push_back(position.x);
                vertices.push_back(position.y);
                vertices.push_back(position.z);
                vertices.push_back(normal.x);
                vertices.push_back(normal.y);
                vertices.push_back(normal.z);
                vertices.push_back(texCoords[0]);
                vertices.push_back(texCoords[1]);
                vertices.push_back(color.r);
                vertices.push_back(color.g);
                vertices.push_back(color.b);
            }
        }

        // Generate indices
        for (unsigned int ring = 0; ring < rings; ++ring) {
            for (unsigned int seg = 0; seg < segments; ++seg) {
                unsigned int current = ring * (segments + 1) + seg;
                unsigned int next = current + segments + 1;

                // First triangle
                indices.push_back(current);
                indices.push_back(next);
                indices.push_back(current + 1);

                // Second triangle
                indices.push_back(current + 1);
                indices.push_back(next);
                indices.push_back(next + 1);
            }
        }

        return std::make_pair(vertices, indices);
    }

    inline std::pair<std::vector<float>, std::vector<unsigned int>> CreateTetrahedron(float size = 1.0f, glm::vec3 color = glm::vec3{ 1.0f, 1.0f, 1.0f })
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        // Calculate vertices of a regular tetrahedron with top point at +Y
        float h = size * std::sqrt(2.0f / 3.0f);  // Height of tetrahedron
        float r = size * std::sqrt(3.0f) / 3.0f;  // Radius of base circle

        // Four vertices of the tetrahedron (already centered)
        glm::vec3 positions[4] = {
            glm::vec3(0.0f, h * 0.5f, 0.0f),                                  // Top vertex (positive Y)
            glm::vec3(-r, -h * 0.5f, 0.0f),                                   // Base vertex 1
            glm::vec3(r * 0.5f, -h * 0.5f, -r * std::sqrt(3.0f) * 0.5f),      // Base vertex 2
            glm::vec3(r * 0.5f, -h * 0.5f, r * std::sqrt(3.0f) * 0.5f)        // Base vertex 3
        };

        // Define the 4 faces (each face is a triangle)
        // Each row represents a face with 3 vertex indices
        unsigned int faces[4][3] = {
            {0, 1, 2},  // Face 0
            {0, 3, 1},  // Face 1
            {0, 2, 3},  // Face 2
            {1, 3, 2}   // Face 3
        };

        // Generate vertices for each face (with proper normals)
        for (int face = 0; face < 4; ++face) {
            // Get the three vertices of this face
            glm::vec3 v0 = positions[faces[face][0]];
            glm::vec3 v1 = positions[faces[face][1]];
            glm::vec3 v2 = positions[faces[face][2]];

            // Calculate face normal
            glm::vec3 edge1 = v1 - v0;
            glm::vec3 edge2 = v2 - v0;
            glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

            // Add three vertices for this face
            vertices.push_back(v0.x);
            vertices.push_back(v0.y);
            vertices.push_back(v0.z);
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(color.r);
            vertices.push_back(color.g);
            vertices.push_back(color.b);

            vertices.push_back(v1.x);
            vertices.push_back(v1.y);
            vertices.push_back(v1.z);
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);
            vertices.push_back(color.r);
            vertices.push_back(color.g);
            vertices.push_back(color.b);

            vertices.push_back(v2.x);
            vertices.push_back(v2.y);
            vertices.push_back(v2.z);
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
            vertices.push_back(0.5f);
            vertices.push_back(1.0f);
            vertices.push_back(color.r);
            vertices.push_back(color.g);
            vertices.push_back(color.b);

            // Add indices for this face
            unsigned int baseIdx = face * 3;
            indices.push_back(baseIdx);
            indices.push_back(baseIdx + 1);
            indices.push_back(baseIdx + 2);
        }

        return std::make_pair(vertices, indices);
    }
    
}
#endif // !GEOMETRICTOOLS_H_