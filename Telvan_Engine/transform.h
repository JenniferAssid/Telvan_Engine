#ifndef TRANSFORM_CLASS_H
#define TRANSFORM_CLASS_H

#include <glm/glm.hpp>
#include <rapidjson/prettywriter.h>

#include "components.h"

class Transform : Component
{
private:
    glm::vec3 translation_;
    glm::vec2 scale_;
    float rotation_;

public:
    Transform(glm::vec3 translation = glm::vec3(0.0f),
        glm::vec3 scale = glm::vec3(1.0f),
        float rotation = 0.0f) : translation_(translation),
                                    scale_(scale),
                                    rotation_(rotation)
    {}
    // TODO: Make copy constructor
    ~Transform() {}

    void Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;
    void Read_From(rapidjson::Document& document) override;

    inline glm::vec2 Get_Translation() const { return glm::vec2(translation_.x, translation_.y); }
    inline float Get_Z_Sorting_Value() const { return translation_.z; }
    inline glm::vec2 Get_Scale() const { return scale_; }
    inline float Get_Rotation() const { return rotation_; }

    inline void Set_Translation(glm::vec3 translation) { translation_ = glm::vec3(translation.x,
                                                                                    translation.y,
                                                                                    translation.z); }
    inline void Set_Translation(glm::vec2 translation) { translation_ = glm::vec3(translation.x, 
                                                                                    translation.y, 
                                                                                    translation_.z); }
    inline void Set_Z_Sorting_Value(float z) { translation_.z = z; }
    inline void Set_Scale(glm::vec2 scale) { scale_ = scale; }
    inline void Set_Scale_X(float x) { scale_.x = x; }
    inline void Set_Scale_Y(float y) { scale_.y = y; }
    inline void Set_Rotation(float deg) { rotation_ = deg; }
};

#endif // !TRANSFORM_CLASS_H
