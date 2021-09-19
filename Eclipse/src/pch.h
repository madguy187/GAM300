#pragma once

// printing for console
#include <iostream>

// for defines
#include <stdint.h>

// memory
#include <memory>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <string>
#include <cstdlib>
#include <string>
#include <vector>
#include <utility>
#include <string>
#include <istream>
#include <cmath>
#include <fstream>
#include <array>
#include <algorithm>
#include <memory>
#include <filesystem>

// functional
#include <algorithm>
#include <functional>

// error
#include <assert.h>

// containers
#include <bitset>
#include <array>
#include <queue>
#include <unordered_map>
#include <set>

// Events
#include "Events/Events.h"
#include "Events/EventManager.h"

// engine extern pointer
#include "EntryPoint/EntryPoint.h"

// Imgui
//#include "imgui.h"
//#include "imgui_impl_opengl3.h"
//#include "imgui_impl_glfw.h"

// Logger
#include "Logger/Logger.h"

#include "matrix_transform.hpp"
#include "type_ptr.hpp"
#include "matrix_transform_2d.hpp"

#include "stb_image.h"

#include "GLEW/include/GL/glew.h"
#include "GLFW/include/GLFW/glfw3.h"
#include "GLM/glm/glm.hpp"

#include "Graphics.h"

// Profiler
#include "Editor/Windows/Profiler/Profiler.h"

// Custom Libraries
#include "Library/Math/Vector.h"
#include "Library/Math/Matrix.h"
#include "Editor/Utilities/UndoAndRedo/ICommand.h"
#include "Library/Strings/Lexical.h"
#include "Library/Strings/StringsConcatenate.h"

// Reflection
//#include "Reflection/registration.h"
//#include "Reflection/primitive_support.h"
//#include "Reflection/complex_support.h"
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"
#include "Reflection/Variant/RefVariant.h"
#include "Reflection/Variant/Variant.h"

// Serialization
#include "Serialization/SerializationManager.h"

//SceneManager
#include "SceneManagement/SceneManager.h"

// Editor API
#include "Editor/ECGuiAPI/ECGui.h"
#include "ImGuizmo/ImGuizmo.h" 
#include "Editor/Windows/Log/Log.h"

// ---- Input Handler Api ----
// Create an object for example : InputWrapper test;
// Call test.GetKeyCurrent(InputKeycode::Key_W)
#include "Graphics/InputHandler/InputWrapper.h"

// GlCall Error
#include "Graphics/RendererAPI/Renderer.h"