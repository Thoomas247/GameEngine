#pragma once

/* standard includes */
#include <map>
#include <unordered_map>
#include <set>
#include <vector>

#include <optional>

#include <string>
#include <regex>

#include <memory>

#include <fstream>
#include <sstream>
#include <iostream>

#include <filesystem>

/* external includes */
#define GLFW_INCLUDE_VULKAN
#include "glfw/glfw3.h"

#include "vulkan/vulkan.h"

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "stb/stb_image.h"

#include "tinygltf/tiny_gltf.h"

#include "json/json.hpp"
using json = nlohmann::json;

//#include "shaderc/shaderc.hpp"
//#include "spirv-cross/spirv_reflect.hpp"

/* game engine common includes */
#include "core/UUID.h"
#include "core/Log.h"
#include "core/Input.h"
#include "core/Window.h"
#include "core/MatrixUtil.h"
#include "core/Util.h"

#include "importer/Importer.h"

#include "graphics/Renderer.h"

#include "scene/SceneManager.h"

#include "project/ProjectManager.h"