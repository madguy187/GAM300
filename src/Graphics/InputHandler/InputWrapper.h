#pragma once
#include <array>
#include "AllInputKeyCodes.h"
#include <iostream>

namespace Eclipse
{

  class InputWrapper
  {
  public:

    struct InputData
    {
      InputState inputState = InputState::Key_NULLSTATE;
      bool SingleFlag = false;
    };

    void init();
    int isKeyPressed = 0;
    int isKeyHold = 0;
    int isKeyReleased = 0;
    bool SingleKeyPressed = false;

   std::map< std::pair<InputKeycode, int>, std::pair<bool,bool> > _pressedKeyRecords;
   using KeyIT = std::map< std::pair<InputKeycode, int>, std::pair<bool, bool> >::iterator;

   std::array<InputData, 256> _keyboardmouseStatus;		//Stores the input state of both keyboard and mouse
    // check keypress
        //<remarks> Choose array since number of keycode is fixed  ... 0 is empty </remarks

    //Front portion stores curr frame pressed, Back portion store last frame pressed
    //trying to avoid std set since it takes up more memory in its abstraction
    //std::array<unsigned char, 256> _pressedKeyRecords;		//Stores the "press state" keys to be converted to "down state"

    unsigned char _currPressedCount = 0;					//Num of keys pressed in curr frame (remain until next frame)
    unsigned char _lastPressedCount = 0;					//Num of keys pressed in last frame (to be converted)

    bool GetKeyPressed(InputKeycode keycode) ;
    bool GetKeyDown(InputKeycode keycode) ;
    bool GetKeyReleased(InputKeycode keycode) ;
    float GetKeyAxis(InputKeycode keycode) ;

    void Update();

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    ////Convert keycode to string
    //std::ostream& operator <<(std::ostream& stream, const Eclipse::InputKeycode& keycode);

  };
}