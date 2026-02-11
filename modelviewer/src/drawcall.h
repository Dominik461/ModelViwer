#ifndef DRAWCALL_H_
#define DRAWCALL_H_

#include <glad/glad.h>

struct DrawCall {
    int materialID;
    GLuint indexOffset; // in elements
    GLuint indexCount;
};

#endif // !DRAWCALL_H_
