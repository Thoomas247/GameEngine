#ifndef SETTINGS
#define SETTINGS

#include "glm/glm.hpp"

// World settings:
const glm::vec3 S_WorldUp = { 0.0f, 1.0f, 0.0f };

// User settings: (TODO: make not const and global)
const unsigned int S_ScreenWidth = 1600;
const unsigned int S_ScreenHeight = 1200;
const float S_MouseSensitivity = 0.1f;

#endif // !SETTINGS
