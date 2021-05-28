#include "pch.h"
#include "InputWrapper.h"
#include "GLHelper.h"

using namespace Eclipse;

static std::ostream& operator << (std::ostream& stream, const Eclipse::InputKeycode& keycode) {

  switch (keycode)
  {
  case InputKeycode::Key_Null:
    stream << "Key_NULL";
    break;
  case InputKeycode::Key_BACKSPACE:
    stream << "Key_BACKSPACE";
    break;
  case InputKeycode::Key_TAB:
    stream << "Key_TAB";
    break;
  case InputKeycode::Key_CLEAR:
    stream << "Key_CLEAR";
    break;
  case InputKeycode::Key_RETURN:
    stream << "Key_RETURN";
    break;
  case InputKeycode::Key_SHIFT:
    stream << "Key_SHIFT";
    break;
  case InputKeycode::Key_CTRL:
    stream << "Key_CTRL";
    break;
  case InputKeycode::Key_ALT:
    stream << "Key_ALT";
    break;
  case InputKeycode::Key_PAUSE:
    stream << "Key_PAUSE";
    break;
  case InputKeycode::Key_CAPSLOCK:
    stream << "Key_CAPSLOCK";
    break;
  case InputKeycode::Key_ESC:
    stream << "Key_ESC";
    break;
  case InputKeycode::Key_SPACE:
    stream << "Key_SPACE";
    break;
  case InputKeycode::Key_PGUP:
    stream << "Key_PGUP";
    break;
  case InputKeycode::Key_PGDN:
    stream << "Key_PGDN";
    break;
  case InputKeycode::Key_END:
    stream << "Key_END ";
    break;
  case InputKeycode::Key_HOME:
    stream << "Key_HOME";
    break;
  case InputKeycode::Key_LEFT:
    stream << "Key_LEFT";
    break;
  case InputKeycode::Key_UP:
    stream << "Key_UP";
    break;
  case InputKeycode::Key_RIGHT:
    stream << "Key_RIGHT";
    break;
  case InputKeycode::Key_DOWN:
    stream << "Key_DOWN";
    break;
  case InputKeycode::Key_SELECT:
    stream << "Key_SELECT";
    break;
  case InputKeycode::Key_PRINT:
    stream << "Key_PRINT";
    break;
  case InputKeycode::Key_EXECUTE:
    stream << "Key_EXECUTE";
    break;
  case InputKeycode::Key_PRTSCREEN:
    stream << "Key_PRTSC";
    break;
  case InputKeycode::Key_INSERT:
    stream << "Key_INSERT";
    break;
  case InputKeycode::Key_DELETE:
    stream << "Key_DELETE";
    break;
  case InputKeycode::Key_HELP:
    stream << "Key_HELP";
    break;
  case InputKeycode::Key_0:
    stream << "Key_0";
    break;
  case InputKeycode::Key_1:
    stream << "Key_1";
    break;
  case InputKeycode::Key_2:
    stream << "Key_2";
    break;
  case InputKeycode::Key_3:
    stream << "Key_3";
    break;
  case InputKeycode::Key_4:
    stream << "Key_4";
    break;
  case InputKeycode::Key_5:
    stream << "Key_5";
    break;
  case InputKeycode::Key_6:
    stream << "Key_6";
    break;
  case InputKeycode::Key_7:
    stream << "Key_7";
    break;
  case InputKeycode::Key_8:
    stream << "Key_8";
    break;
  case InputKeycode::Key_9:
    stream << "Key_9";
    break;
  case InputKeycode::Key_A:
    stream << "Key_A";
    break;
  case InputKeycode::Key_B:
    stream << "Key_B";
    break;
  case InputKeycode::Key_C:
    stream << "Key_C";
    break;
  case InputKeycode::Key_D:
    stream << "Key_D";
    break;
  case InputKeycode::Key_E:
    stream << "Key_E";
    break;
  case InputKeycode::Key_F:
    stream << "Key_F";
    break;
  case InputKeycode::Key_G:
    stream << "Key_G";
    break;
  case InputKeycode::Key_H:
    stream << "Key_H";
    break;
  case InputKeycode::Key_I:
    stream << "Key_I";
    break;
  case InputKeycode::Key_J:
    stream << "Key_J";
    break;
  case InputKeycode::Key_K:
    stream << "Key_K";
    break;
  case InputKeycode::Key_L:
    stream << "Key_L";
    break;
  case InputKeycode::Key_M:
    stream << "Key_M";
    break;
  case InputKeycode::Key_N:
    stream << "Key_N";
    break;
  case InputKeycode::Key_O:
    stream << "Key_O";
    break;
  case InputKeycode::Key_P:
    stream << "Key_P";
    break;
  case InputKeycode::Key_Q:
    stream << "Key_Q";
    break;
  case InputKeycode::Key_R:
    stream << "Key_R";
    break;
  case InputKeycode::Key_S:
    stream << "Key_S";
    break;
  case InputKeycode::Key_T:
    stream << "Key_T";
    break;
  case InputKeycode::Key_U:
    stream << "Key_U";
    break;
  case InputKeycode::Key_V:
    stream << "Key_V";
    break;
  case InputKeycode::Key_W:
    stream << "Key_W";
    break;
  case InputKeycode::Key_X:
    stream << "Key_X";
    break;
  case InputKeycode::Key_Y:
    stream << "Key_Y";
    break;
  case InputKeycode::Key_Z:
    stream << "Key_Z";
    break;
  //case InputKeycode::Key_WINLEFT:
  //  stream << "Key_WINLEFT";
  //  break;
  //case InputKeycode::Key_WINRIGHT:
  //  stream << "Key_WINRIGHT";
  //  break;
  case InputKeycode::Key_APPS:
    stream << "Key_APPS";
    break;
  case InputKeycode::Key_SLEEP:
    stream << "Key_SLEEP";
    break;
  //case InputKeycode::Key_NUM0:
  //  stream << "Key_NUM0";
  //  break;
  //case InputKeycode::Key_NUM1:
  //  stream << "Key_NUM1";
  //  break;
  //case InputKeycode::Key_NUM2:
  //  stream << "Key_NUM2";
  //  break;
  //case InputKeycode::Key_NUM3:
  //  stream << "Key_NUM3";
  //  break;
  //case InputKeycode::Key_NUM4:
  //  stream << "Key_NUM4";
  //  break;
  //case InputKeycode::Key_NUM5:
  //  stream << "Key_NUM5";
  //  break;
  //case InputKeycode::Key_NUM6:
  //  stream << "Key_NUM6";
  //  break;
  //case InputKeycode::Key_NUM7:
  //  stream << "Key_NUM7";
  //  break;
  //case InputKeycode::Key_NUM8:
  //  stream << "Key_NUM8";
  //  break;
  //case InputKeycode::Key_NUM9:
  //  stream << "Key_NUM9";
  //  break;
  case InputKeycode::Key_MULTIPLY:
    stream << "Key_MULTIPLY";
    break;
  case InputKeycode::Key_ADD:
    stream << "Key_ADD";
    break;
  case InputKeycode::Key_SEPERATOR:
    stream << "Key_SEPERATOR";
    break;
  case InputKeycode::Key_SUBTRACT:
    stream << "Key_SUBTRACT";
    break;
  case InputKeycode::Key_DECIMAL:
    stream << "Key_DECIMAL";
    break;
  case InputKeycode::Key_DIVIDE:
    stream << "Key_DIVIDE";
    break;
  case InputKeycode::Key_F1:
    stream << "Key_F1";
    break;
  case InputKeycode::Key_F2:
    stream << "Key_F2";
    break;
  case InputKeycode::Key_F3:
    stream << "Key_F3";
    break;
  case InputKeycode::Key_F4:
    stream << "Key_F4";
    break;
  case InputKeycode::Key_F5:
    stream << "Key_F5";
    break;
  case InputKeycode::Key_F6:
    stream << "Key_F6";
    break;
  case InputKeycode::Key_F7:
    stream << "Key_F7";
    break;
  case InputKeycode::Key_F8:
    stream << "Key_F8";
    break;
  case InputKeycode::Key_F9:
    stream << "Key_F9";
    break;
  case InputKeycode::Key_F10:
    stream << "Key_F10";
    break;
  case InputKeycode::Key_F11:
    stream << "Key_F11";
    break;
  case InputKeycode::Key_F12:
    stream << "Key_F12";
    break;
  case InputKeycode::Key_NUMLOCK:
    stream << "Key_NUMLOCK";
    break;
  case InputKeycode::Key_SCROLLLOCK:
    stream << "Key_SCROLLLOCK";
    break;
  case InputKeycode::Key_SHIFTLEFT:
    stream << "Key_SHIFTLEFT";
    break;
  case InputKeycode::Key_SHIFTRIGHT:
    stream << "Key_SHIFTRIGHT";
    break;
  case InputKeycode::Key_CTRLLEFT:
    stream << "Key_CTRLLEFT";
    break;
  case InputKeycode::Key_CTRLRIGHT:
    stream << "Key_CTRLRIGHT";
    break;
  case InputKeycode::Key_ALTLEFT:
    stream << "Key_ALTLEFT";
    break;
  case InputKeycode::Key_ALTRIGHT:
    stream << "Key_ALTRIGHT";
    break;
  case InputKeycode::Key_SEMICOLON:
    stream << "Key_SEMICOLON";
    break;
  case InputKeycode::Key_PLUS:
    stream << "Key_PLUS";
    break;
  case InputKeycode::Key_COMMA:
    stream << "Key_COMMA";
    break;
  case InputKeycode::Key_MINUS:
    stream << "Key_MINUS";
    break;
  case InputKeycode::Key_FORWARDSLASH:
    stream << "Key_FORWARDSLASH";
    break;    stream << "Key_TILDE";
    break;
  case InputKeycode::Key_SQBRACKETLEFT:
    stream << "Key_SQBRACKETLEFT";
    break;
  case InputKeycode::Key_BACKSLASH:
    stream << "Key_BACKSLASH ";
    break;
  case InputKeycode::Key_SQBRACKETRIGHT:
    stream << "Key_SQBRACKETRIGHT";
    break;
  case InputKeycode::Key_QUOTE:
    stream << "Key_QUOTE";
    break;
  case InputKeycode::Key_MISCELLANEOUS:
    stream << "Key_MISCELLANEOUS";
    break;
  default:
    break;
  }
  return stream;
}

void Eclipse::InputWrapper::init()
{
  KeyContainer.clear();
}

bool Eclipse::InputWrapper::GetIsPrint()
{
  return EnablePrint;
}

void Eclipse::InputWrapper::SetIsPrint(bool input)
{
  EnablePrint = input;
}

bool InputWrapper::GetKeyTriggered(InputKeycode keycode)
{
#ifndef CURRENT_CODE

  int isKeyPressed = glfwGetKey(GLHelper::ptr_window, static_cast<int>(keycode));

  if (isKeyPressed == GLFW_PRESS)
  {
    bool single = false;
    KeyContainer.insert({ std::pair<InputKeycode,int>(keycode,isKeyPressed),std::pair<bool,bool>(single,single) });

    for (auto& pair2 : KeyContainer)
    {
      auto& Message = (pair2.first.first);
      auto& SingleFlag = (pair2.second.first);

      if (SingleFlag)
        continue;

      SingleFlag = true;
      std::cout << "Container Size :" << KeyContainer.size() << std::endl;

      if (EnablePrint)
      {
        std::cout << "Pressed " << static_cast<int>(Message) << " " << std::endl;
      }
      return true;
    }
  }
  else if (isKeyPressed == GLFW_RELEASE)
  {
    GetKeyReleased(keycode);
    return false;
  }
  return false;

#else
  int isKeyPressed = glfwGetKey(GLHelper::ptr_window, static_cast<int>(keycode));

  if (isKeyPressed == GLFW_PRESS && !single)
  {
    if (single)
      return false;

    single = true;
    //std::cout << " Create Apple " << std::endl;
    return true;
  }
  else if (isKeyPressed == GLFW_RELEASE)
  {
    single = false;
    return false;
  }
  return false;
#endif
}

bool Eclipse::InputWrapper::GetKeyCurrent(InputKeycode keycode)
{
#ifndef CURRENT_CODE
  int isKeyPressed = glfwGetKey(GLHelper::ptr_window, static_cast<int>(keycode));

  if (isKeyPressed == GLFW_PRESS)
  {
    return true;
  }
  return false;

#else
  return false;
#endif
}

bool Eclipse::InputWrapper::GetKeyReleased(InputKeycode keycode)
{
  int isKeyReleased = glfwGetKey(GLHelper::ptr_window, static_cast<int>(keycode));

  if (isKeyReleased == GLFW_RELEASE)
  {
    for (auto KeyIT = KeyContainer.begin(); KeyIT != KeyContainer.end(); ++KeyIT)
    {
      auto& Message = ((*KeyIT).first.first);

      if (Message == keycode)
      {
        auto& Press = ((*KeyIT).first.second);
        auto& SingleFlag = ((*KeyIT).second.first);

        SingleFlag = false;
        KeyContainer.erase(KeyIT);

        if (EnablePrint)
        {
          std::cout << "Released " << static_cast<int>(Message) << " " << std::endl;
        }
        return true;
      }
    }
  }
  return false;
}

void Eclipse::InputWrapper::Update()
{


}

void Eclipse::InputWrapper::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}