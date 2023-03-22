#pragma once

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

    bool print_full_transform_;

private:
    void write_to(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

public:
    Transform(glm::vec3 translation = glm::vec3(0.0f),
        glm::vec3 scale = glm::vec3(1.0f),
        float rotation = 0.0f) : Component(Component_Type::ct_Transform),
        translation_(translation),
        scale_(scale),
        rotation_(rotation),
        print_full_transform_(false)
    {}
    // TODO: Make copy constructor
    ~Transform() {}

    Component* Clone() override;

    void Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;
    void Read_From(rapidjson::GenericObject<false, rapidjson::Value>& writer) override;

    inline glm::vec2 Get_Translation() const { return glm::vec2(translation_.x, translation_.y); }
    inline float Get_Z_Sorting_Value() const { return translation_.z; }
    inline glm::vec2 Get_Scale() const { return scale_; }
    inline float Get_Rotation() const { return rotation_; }
    inline bool Get_Print_Full_Transform() const { return print_full_transform_; }

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
    inline void Set_Print_Full_Transform(bool print_full_transform) { print_full_transform_ = print_full_transform; }
};

#endif // !TRANSFORM_CLASS_H
