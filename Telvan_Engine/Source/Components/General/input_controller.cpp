/*************************************************************************************************************************************
* Title: input_controller.cpp
* Author: Jennifer Assid
* Date of Creation: 21 March 2023
*
**************************************************************************************************************************************/
#include "input_controller.h"
#include "rigid_body.h"
#include "error_logging.h"
#include "entity.h"
#include "engine.h"

void move_up(float dT, Input_Controller& ic)
{
    if (ic.parent_ == nullptr)
    {
        Error_Logging::Get_Instance()->Record_Message("Parent not found",
            Error_Logging::Message_Level::ot_Information,
            "Input_Controller",
            "move_up");
        return;
    }

    Rigid_Body* rigid_body = ic.parent_->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);

    if (rigid_body == nullptr)
    {
        Error_Logging::Get_Instance()->Record_Message(
            Error_Logging::Get_Instance()->Format_Output("Transform not found for entity \"%s\"",
                ic.parent_->Get_Name().c_str()),
            Error_Logging::Message_Level::ot_Information,
            "Input_Controller",
            "move_up");
        return;
    }

    rigid_body->Add_Force(glm::vec2(0.0f, 1.0f) * 5.0f);
}

void move_down(float dT, Input_Controller& ic)
{
    if (ic.parent_ == nullptr)
    {
        Error_Logging::Get_Instance()->Record_Message("Parent not found",
            Error_Logging::Message_Level::ot_Information,
            "Input_Controller",
            "move_down");
        return;
    }

    Rigid_Body* rigid_body = ic.parent_->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);


    if (rigid_body == nullptr)
    {
        Error_Logging::Get_Instance()->Record_Message(
            Error_Logging::Get_Instance()->Format_Output("Transform not found for entity \"%s\"",
                ic.parent_->Get_Name().c_str()),
            Error_Logging::Message_Level::ot_Information,
            "Input_Controller",
            "move_down");
        return;
    }

    rigid_body->Add_Force(glm::vec2(0.0f, -1.0f) * 5.0f);
}

void move_left(float dT, Input_Controller& ic)
{
    if (ic.parent_ == nullptr)
    {
        Error_Logging::Get_Instance()->Record_Message("Parent not found",
            Error_Logging::Message_Level::ot_Information,
            "Input_Controller",
            "move_left");
        return;
    }

    Rigid_Body* rigid_body = ic.parent_->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);

    if (rigid_body == nullptr)
    {
        Error_Logging::Get_Instance()->Record_Message(
            Error_Logging::Get_Instance()->Format_Output("Transform not found for entity \"%s\"",
                ic.parent_->Get_Name().c_str()),
            Error_Logging::Message_Level::ot_Information,
            "Input_Controller",
            "move_left");
        return;
    }

    rigid_body->Add_Force(glm::vec2(-1.0f, 0.0f) * 5.0f);
}

void move_right(float dT, Input_Controller& ic)
{
    if (ic.parent_ == nullptr)
    {
        Error_Logging::Get_Instance()->Record_Message("Parent not found",
            Error_Logging::Message_Level::ot_Information,
            "Input_Controller",
            "move_right");
        return;
    }

    Rigid_Body* rigid_body = ic.parent_->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);

    if (rigid_body == nullptr)
    {
        Error_Logging::Get_Instance()->Record_Message(
            Error_Logging::Get_Instance()->Format_Output("Transform not found for entity \"%s\"",
                ic.parent_->Get_Name().c_str()),
            Error_Logging::Message_Level::ot_Information,
            "Input_Controller",
            "move_right");
        return;
    }

    rigid_body->Add_Force(glm::vec2(1.0f, 0.0f) * 5.0f);
}

void toggle_debug_draw(float dT, Input_Controller& ic)
{
    Engine::Get_Instance()->Set_Debug_Draw(!Engine::Get_Instance()->Get_Debug_Draw());
}

Input_Controller::Input_Controller() : Component(Component_Type::ct_Input_Controller) 
{
    action_bindings_ = new int[(unsigned int)Input_Actions::TOTAL_NUMBER_OF_ACTIONS];

    if (Input::Get_Instance()->Is_Binded(GLFW_KEY_W, Input::Callback_Type::cb_Down) == false)
    {
        action_bindings_[(int)Input_Actions::ia_Up] = GLFW_KEY_W;
        action_bindings_[(int)Input_Actions::ia_Down] = GLFW_KEY_S;
        action_bindings_[(int)Input_Actions::ia_Left] = GLFW_KEY_A;
        action_bindings_[(int)Input_Actions::ia_Right] = GLFW_KEY_D;
        action_bindings_[(int)Input_Actions::ia_Toggle_Debug] = GLFW_KEY_TAB;
    }
}

Input_Controller::~Input_Controller() {}

Component* Input_Controller::Clone()
{
    Input_Controller* input_controller = new Input_Controller();

    return (Component*)input_controller;
}

void Input_Controller::Pre_Update(float dT)
{
    int key;
    Input* instance = Input::Get_Instance();
    std::vector<int> events_ = instance->Get_Events();
    for (unsigned i = 0; i < events_.size(); i++)
    {
        key = events_[i];

        Input::Input_Information info = instance->Get_Info_From_Key(key);

        // Key has just been pressed
        if (info.next == true && info.current == false)
        {
            if (info.on_press != nullptr) info.on_press(dT, *this);
            
            events_.erase(events_.begin() + i);
            i--;
        }
        // Key is being held down
        else if (info.next == true && info.current == true && info.on_down != nullptr)
            info.on_down(dT, *this);
        // Key has been released
        else if (info.next == false && info.current == true)
        {
            if (info.on_release != nullptr)  info.on_release(dT, *this);

            events_.erase(events_.begin() + i);
            i--;
        }
        else if (info.next == false && info.current == false)
        {
            events_.erase(events_.begin() + i);
            i--;
        }

        instance->Set_Current_State_To_Next_Of_Key(key);
    }
}

void Input_Controller::Start()
{
    Input::Get_Instance()->Add_Binding(action_bindings_[(int)Input_Actions::ia_Up], move_up, Input::Callback_Type::cb_Down);
    Input::Get_Instance()->Add_Binding(action_bindings_[(int)Input_Actions::ia_Down], move_down, Input::Callback_Type::cb_Down);
    Input::Get_Instance()->Add_Binding(action_bindings_[(int)Input_Actions::ia_Left], move_left, Input::Callback_Type::cb_Down);
    Input::Get_Instance()->Add_Binding(action_bindings_[(int)Input_Actions::ia_Right], move_right, Input::Callback_Type::cb_Down);
    Input::Get_Instance()->Add_Binding(action_bindings_[(int)Input_Actions::ia_Toggle_Debug], toggle_debug_draw, Input::Callback_Type::cb_Press);
}

void Input_Controller::Stop()
{
    Input::Get_Instance()->Remove_Binding(action_bindings_[(int)Input_Actions::ia_Up], Input::Callback_Type::cb_Down);
    Input::Get_Instance()->Remove_Binding(action_bindings_[(int)Input_Actions::ia_Down], Input::Callback_Type::cb_Down);
    Input::Get_Instance()->Remove_Binding(action_bindings_[(int)Input_Actions::ia_Left], Input::Callback_Type::cb_Down);
    Input::Get_Instance()->Remove_Binding(action_bindings_[(int)Input_Actions::ia_Right], Input::Callback_Type::cb_Down);
}

void Input_Controller::Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, bool preserve_values)
{
    writer.Key("input_controller");
    writer.StartObject();
    writer.EndObject();
}

void Input_Controller::Read_From(rapidjson::GenericObject<false, rapidjson::Value>& writer)
{
    if (writer.HasMember("input_controller") == false) return;
}