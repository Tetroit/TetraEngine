#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <memory>
#include <ctime>
#include <thread>
#include <queue>

#include <Windows.h>

#include <stb_image.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../config.h"
#include "../utils/Types.h"

#define LOG(x) std::cout << "SYSTEM::" << x << '\n'
#define LOG_ERR(x) std::cout << "ERROR::" << x << '\n'
#define LOG_FROM(x,y) std::cout << x << "::" << y << '\n'

using uint = unsigned int;