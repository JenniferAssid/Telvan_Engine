#pragma once
#ifndef INPUT_CONTROLLER_CLASS_H
#define INPUT_CONTROLLER_CLASS_H

#include "components.h"
#include "input.h"

class Input_Controller : Component
{
protected:
    friend void move_up(float dT, Input_Controller& ic);
    friend void move_down(float dT, Input_Controller& ic);
    friend void move_left(float dT, Input_Controller& ic);
    friend void move_right(float dT, Input_Controller& ic);

    int* action_bindings_;
public:
    enum class Input_Actions
    {
        ia_Up = 0,
        ia_Down,
        ia_Left,
        ia_Right,

        TOTAL_NUMBER_OF_ACTIONS
    };

    Input_Controller();
    ~Input_Controller();

    Component* Clone() override;

    void Pre_Update(float dT) override;

    void Start() override;
    void Stop() override;

    void Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)  override;
    void Read_From(rapidjson::GenericObject<false, rapidjson::Value>& writer) override;
};

#endif // !INPUT_CONTROLLER_CLASS_H

