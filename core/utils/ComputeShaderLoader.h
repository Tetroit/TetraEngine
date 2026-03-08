#pragma once
#include <fstream>
#include <sstream>

namespace TetraEngine {
    class ComputeShaderLoader {
    public:
        static GLuint loadCompute(const std::string& path)
        {
            std::ifstream file(path);
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string source = buffer.str();
            const char* src = source.c_str();

            GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
            glShaderSource(shader, 1, &src, nullptr);
            glCompileShader(shader);

            GLint success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char log[1024];
                glGetShaderInfoLog(shader, 1024, nullptr, log);
                std::cerr << "Compute shader compile error:\n" << log << std::endl;
            }

            GLuint program = glCreateProgram();
            glAttachShader(program, shader);
            glLinkProgram(program);

            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if (!success)
            {
                char log[1024];
                glGetProgramInfoLog(program, 1024, nullptr, log);
                std::cerr << "Program link error:\n" << log << std::endl;
            }

            glDeleteShader(shader);
            return program;
        }
    };
} // TetraEngine
