#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace TetraEngine {
    class Utils {
    private:
        Utils() = default;
    public:

        static std::string GuidToString(const GUID& g) {
            return std::format(
                "{:08X}-{:04X}-{:04X}-{:02X}{:02X}-{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}",
                g.Data1, g.Data2, g.Data3,
                g.Data4[0], g.Data4[1],
                g.Data4[2], g.Data4[3],
                g.Data4[4], g.Data4[5],
                g.Data4[6], g.Data4[7]
            );
        }
        static GUID GuidFromString(const std::string& guid) {
            GUID g{};
            std::istringstream iss(guid);
            std::string part;
            std::getline(iss, part, '-');
            g.Data1 = std::stoul(part, nullptr, 16);
            std::getline(iss, part, '-');
            g.Data2 = std::stoul(part, nullptr, 16);
            std::getline(iss, part, '-');
            g.Data3 = std::stoul(part, nullptr, 16);
            std::getline(iss, part, '-');
            g.Data4[0] = (uint8_t) std::stoul(part.substr(0,2), nullptr, 16);
            g.Data4[1] = (uint8_t) std::stoul(part.substr(2,2), nullptr, 16);
            std::getline(iss, part);
            for (int i=0;i<6;i++) {
                g.Data4[i+2] = (uint8_t) std::stoul(part.substr(i*2,2), nullptr, 16);
            }
            return g;
        }
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