#include "Matrix.hpp"
#include "glm/gtx/matrix_decompose.inl"
#include "glm/gtx/string_cast.hpp"

namespace glm
{
    vec3 axis_unit_creator_v3(size_t index)
    {
        vec3 res(0.0f);
        res[index] = -1; //toggle - / + to turn
        return res;
    }
    
    mat4& rotate_around_offset(mat4& m, const vec3& r, const vec3& offset)
    {
        m = translate(m, offset);
        for (size_t i = 0; i < 3; ++i)
            if (r[i] != 0)
                m = rotate(m, r[i], axis_unit_creator_v3(i));
        m = translate(m, vec3(-offset[0], -offset[1], -offset[2]));
        return (m);
    }
}