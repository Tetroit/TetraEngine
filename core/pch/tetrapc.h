#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <corecrt_io.h>
#include <memory>
#include <ctime>
#include <fcntl.h>
#include <thread>
#include <queue>
#include <ranges>

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

constexpr void log_assert(const bool expr, const std::string& success, const std::string& error) {
    if (expr) LOG(success);
    else LOG(error);
}

constexpr void log_assert_from(const bool expr, const std::string& success, const std::string& error, const std::string& sender) {
    if (expr) LOG_FROM(sender, success);
    else LOG_FROM(sender, error);
}

constexpr void log_test(const bool expr, const std::string& testName) {
    if (expr) LOG("\033[32m[o]\033[0m " + testName);
    else LOG("\033[33m[(x)]\033[0m " + testName);
}

using uint = unsigned int;