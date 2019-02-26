#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include <memory>
#include <string>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/constants.hpp>

#include <CBCore/Defines.h>
#include <CBCore/Path.h>
#include <CBSDL/Fwd.h>
#include <CBGL/Fwd.h>

using namespace std::literals;
