#pragma once
#include <pixelgridworld/core.h>
#include <functional>
#include <string>

struct GLFWwindow;

namespace window {
    class Window {
    public:
        typedef std::function<core::Matrix(float deltaTime)> RunFunc;
        Window(int argc, char **argv, int sizeX, int sizeY, const std::string& title, const std::vector<core::RGBA>& palette, RunFunc run);
        ~Window();
        int update();
        int run();
        void screenshot(const std::string filename);
        bool shouldClose();
    private:
        void renderMap(const core::Matrix& map);

        const RunFunc       runFunc;
        const int           m_width;
        const int           m_height;
        const std::vector<core::RGBA>   m_palette;
        GLFWwindow*         m_window;
        unsigned int        m_outputTexture;
        float               prevTime;

    };
}
