/*************************************************************************************************************************************
* Title: input_controller.h
* Author: Jennifer Assid
* Date of Creation: 21 March 2023
*
**************************************************************************************************************************************/
#pragma once
#ifndef INPUT_CONTROLLER_CLASS_H
#define INPUT_CONTROLLER_CLASS_H

#include "components.h"
#include "input.h"

class Input_Controller : Component
{
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:
    enum class Input_Actions
    {
        ia_Up = 0,
        ia_Down,
        ia_Left,
        ia_Right,

        ia_Toggle_Debug,

        TOTAL_NUMBER_OF_ACTIONS
    };

/************************************************************ VARIABLES **************************************************************/
private:
protected:
    int* action_bindings_;

public:
/************************************************************ FUNCTIONS **************************************************************/
private:
protected:
    friend void move_up(float dT, Input_Controller& ic);
    friend void move_down(float dT, Input_Controller& ic);
    friend void move_left(float dT, Input_Controller& ic);
    friend void move_right(float dT, Input_Controller& ic);

    friend void save_to_scene(float dT, Input_Controller& ic);

    friend void toggle_debug_draw(float dT, Input_Controller& ic);

public:
    Input_Controller();
    ~Input_Controller();

    Component* Clone() override;

    void Pre_Update(float dT) override;

    void Start() override;
    void Stop() override;

    void Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, bool preserve_values = true)  override;
    void Read_From(rapidjson::GenericObject<false, rapidjson::Value>& writer) override;
};

#endif // !INPUT_CONTROLLER_CLASS_H

