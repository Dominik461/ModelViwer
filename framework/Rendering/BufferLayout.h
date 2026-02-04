#ifndef BUFFERLAYOUT_H_
#define BUFFERLAYOUT_H_

#include "ShadersDataTypes.h"
#include <string>
#include <vector>

struct BufferAttribute {
    // Constructor with data for the buffer
    BufferAttribute(ShaderDataType type, const std::string& name, GLboolean normalized = false)
        : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0),
        Normalized(normalized) 
    {
        // As Mat4 is saved using 4 * Vec4 it needs its own offset
        if (type == ShaderDataType::Mat4)
        {
            Offset = ShaderDataTypeComponentCount(type);
        }

    }
    std::string Name;
    ShaderDataType Type;
    GLuint Size;
    GLuint Offset;
    GLboolean Normalized;
};

class BufferLayout {
public:
    BufferLayout() {}
    BufferLayout(const std::initializer_list<BufferAttribute>& attributes)
        : Attributes(attributes) {
        this->CalculateOffsetAndStride();
    }

    inline const std::vector<BufferAttribute>& GetAttributes() const { return this->Attributes; }
    inline GLsizei GetStride() const { return this->Stride; }

    std::vector<BufferAttribute>::iterator begin() { return this->Attributes.begin(); }
    std::vector<BufferAttribute>::iterator end() { return this->Attributes.end(); }
    std::vector<BufferAttribute>::const_iterator begin() const { return this->Attributes.begin(); }
    std::vector<BufferAttribute>::const_iterator end() const { return this->Attributes.end(); }

private:
    void CalculateOffsetAndStride() {
        GLsizei offset = 0;
        this->Stride = 0;
        for (auto& attribute : Attributes) {
            
            // Mat4 needs special treatment
            if (attribute.Type == ShaderDataType::Mat4)
            {
                // need to add offset, as it already has a self offset
                attribute.Offset += offset;

                // using atrribute offset as the Mat4 is saved as 4 * Vec4
                offset += attribute.Offset;
            }
            else
            {
                attribute.Offset = offset;
                offset += attribute.Size;
            }
            this->Stride += attribute.Size;
        }
    }

private:
    std::vector<BufferAttribute> Attributes;
    GLsizei Stride;
};
#endif