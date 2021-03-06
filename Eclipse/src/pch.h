#pragma once

// printing for console
#include <iostream>
#include <Tracy/Tracy.hpp>
// for defines
#include <stdint.h>

// memory
#include <memory>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <fstream>
#include <string>
#include <cstdlib>
#include <utility>
#include <istream>
#include <cmath>
#include <fstream>
#include <array>
#include <memory>
#include <filesystem>
#include <thread>
#include <chrono>
#include <random>
#include <utility>
// functional
#include <algorithm>
#include <functional>

// error
#include <assert.h>

// containers
#include <bitset>
#include <array>
#include <queue>
#include <stack>
#include <unordered_map>
#include <set>
#include <limits>

#include "GLM/glm/gtx/quaternion.hpp"

// Events
#include "Events/Events.h"
#include "Events/EventManager.h"

// engine extern pointer
#include "EntryPoint/EntryPoint.h"

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

// Custom Libraries
#include "Library/Math/Vector.h"
#include "Library/Math/Matrix.h"
#include "Editor/Utilities/UndoAndRedo/ICommand.h"
#include "Library/Strings/Lexical.h"
#include "Library/Strings/StringsConcatenate.h"

// Reflection
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

// GlCall Error
#include "Graphics/RendererAPI/Renderer.h"

// Random Number Generator
#include "Library/Random/Random.h"

//UUID
#include "Library/UUID/UUIDGenerator.h"