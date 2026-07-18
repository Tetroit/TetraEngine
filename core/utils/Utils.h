#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace TetraEngine {
    class Utils {
    private:
        Utils() = default;
    public:

        static glm::vec3 Lerp(glm::vec3 a, glm::vec3 b, float fac)
        {
            if (fac < 0)
                fac = 0;
            if (fac > 1)
                fac = 1;
            return a * (1.f - fac) + b * fac;
        }
        static float frand()
        {
            return (float)rand() / (float)RAND_MAX;
        }
        static void Words(std::string line, std::vector<std::string>& res, char separator, bool ignoreEmpty = true) {

            res.clear();
            int wordStart = 0;
            for (int i = 0; i < line.size(); i++)
            {
                if (line[i] == separator)
                {
                    if (wordStart != i || !ignoreEmpty)
                    {
                        std::string word = line.substr(wordStart, i - wordStart);
                        res.push_back(word);
                        wordStart = i + 1;
                    }
                }
            }
            if (wordStart < line.size())
            {
                std::string word = line.substr(wordStart, line.size() - wordStart);
                res.push_back(word);
            }

        }
        static std::vector<std::string> Words(std::string line, char separator, bool ignoreEmpty = true) {

            std::vector<std::string> res;
            int wordStart = 0;
            for (int i = 0; i < line.size(); i++)
            {
                if (line[i] == separator)
                {
                    if (wordStart != i || !ignoreEmpty)
                    {
                        std::string word = line.substr(wordStart, i - wordStart);
                        res.push_back(word);
                        wordStart = i + 1;
                    }
                }
            }
            if (wordStart < line.size())
            {
                std::string word = line.substr(wordStart, line.size() - wordStart);
                res.push_back(word);
            }
            return res;
        }
    };

    struct Flags {
    private:
        int value;
    public:
        bool getFlag(const short id) {
            return (1 << id) & value;
        }
        void setFlag(const short id, const bool state)
        {
            value = (state) ? ((1 << id) | value) : (~(1 << id) & value);
        }
    };
}