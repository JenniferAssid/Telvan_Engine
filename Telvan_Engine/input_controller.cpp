#include "input_controller.h"
#include "transform.h"
#include "error_logging.h"
#include "entity.h"

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

    Transform* transform = ic.parent_->Get_Component<Transform>(Component_Type::ct_Transform);

    if (transform == nullptr)
    {
        Error_Logging::Get_Instance()->Record_Message(
            Error_Logging::Get_Instance()->Format_Output("Transform not found for entity \"%s\"",
                ic.parent_->Get_Name().c_str()),
            Error_Logging::Message_Level::ot_Information,
            "Input_Controller",
            "move_up");
        return;
    }

    glm::vec2 pos = transform->Get_Translation();
    pos.y -= 5.0f;
    transform->Set_Translation(pos);
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

    Transform* transform = ic.parent_->Get_Component<Transform>(Component_Type::ct_Transform);

    if (transform == nullptr)
    {
        Error_Logging::Get_Instance()->Record_Message(
            Error_Logging::Get_Instance()->Format_Output("Transform not found for entity \"%s\"",
                ic.parent_->Get_Name().c_str()),
            Error_Logging::Message_Level::ot_Information,
            "Input_Controller",
            "move_down");
        return;
    }

    glm::vec2 pos = transform->Get_Translation();
    pos.y += 5.0f;
    transform->Set_Translation(pos);
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

    Transform* transform = ic.parent_->Get_Component<Transform>(Component_Type::ct_Transform);

    if (transform == nullptr)
    {
        Error_Logging::Get_Instance()->Record_Message(
            Error_Logging::Get_Instance()->Format_Output("Transform not found for entity \"%s\"",
                ic.parent_->Get_Name().c_str()),
            Error_Logging::Message_Level::ot_Information,
            "Input_Controller",
            "move_left");
        return;
    }

    glm::vec2 pos = transform->Get_Translation();
    pos.x -= 5.0f;
    transform->Set_Translation(pos);
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

    Transform* transform = ic.parent_->Get_Component<Transform>(Component_Type::ct_Transform);

    if (transform == nullptr)
    {
        Error_Logging::Get_Instance()->Record_Message(
            Error_Logging::Get_Instance()->Format_Output("Transform not found for entity \"%s\"",
                ic.parent_->Get_Name().c_str()),
            Error_Logging::Message_Level::ot_Information,
            "Input_Controller",
            "move_right");
        return;
    }

    glm::vec2 pos = transform->Get_Translation();
    pos.x += 5.0f;
    transform->Set_Translation(pos);
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
        if (info.next == true && info.current == false && info.on_press != nullptr)
            info.on_press(dT, *this);
        // Key is being held down
        else if (info.next == true && info.current == true && info.on_down != nullptr)
            info.on_down(dT, *this);
        // Kye has been released
        else if (info.next == false && info.current == true && info.on_release != nullptr)
            info.on_release(dT, *this);

        instance->Set_Current_State_To_Next_Of_Key(key);
    }
}

void Input_Controller::Start()
{
    Input::Get_Instance()->Add_Binding(action_bindings_[(int)Input_Actions::ia_Up], move_up, Input::Callback_Type::cb_Down);
    Input::Get_Instance()->Add_Binding(action_bindings_[(int)Input_Actions::ia_Down], move_down, Input::Callback_Type::cb_Down);
    Input::Get_Instance()->Add_Binding(action_bindings_[(int)Input_Actions::ia_Left], move_left, Input::Callback_Type::cb_Down);
    Input::Get_Instance()->Add_Binding(action_bindings_[(int)Input_Actions::ia_Right], move_right, Input::Callback_Type::cb_Down);
}

void Input_Controller::Stop()
{
    Input::Get_Instance()->Remove_Binding(action_bindings_[(int)Input_Actions::ia_Up], Input::Callback_Type::cb_Down);
    Input::Get_Instance()->Remove_Binding(action_bindings_[(int)Input_Actions::ia_Down], Input::Callback_Type::cb_Down);
    Input::Get_Instance()->Remove_Binding(action_bindings_[(int)Input_Actions::ia_Left], Input::Callback_Type::cb_Down);
    Input::Get_Instance()->Remove_Binding(action_bindings_[(int)Input_Actions::ia_Right], Input::Callback_Type::cb_Down);
}

void Input_Controller::Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
    writer.Key("input_controller");
    writer.StartObject();
    writer.EndObject();
}

void Input_Controller::Read_From(rapidjson::GenericObject<false, rapidjson::Value>& writer)
{
    if (writer.HasMember("input_controller") == false) return;
}