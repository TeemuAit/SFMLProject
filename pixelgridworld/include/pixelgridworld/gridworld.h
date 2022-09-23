#pragma once
#include <pixelgridworld/window.h>

namespace pixelgridworld {
    //// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= ////
    /// The GridWorldWindow class.
    //// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= ////
    class Window {
    private:
    public:
        ///
        /// Creates a new GridWorldWindow.
        ///
        /// \param title - Window title text.
        Window(const std::string& title, size_t sizeInPixels)
            : m_window(0, 0, sizeInPixels, sizeInPixels, title, std::vector<RGBA>({
                // Specify RGBA-palette:
                RGBA(0x00,0x00,0x00,0x00), // 0
                RGBA(0x00,0x00,0x00),      // 1
                RGBA(0xFF,0xFF,0xFF),      // 2
                RGBA(50,50,50),      // 3
                RGBA(100,100,100),   // 4
                RGBA(150,150,150),   // 5
                RGBA(200,200,200),   // 6
                RGBA(250,250,250),   // 7
                // RGBCMY
                RGBA(0xFF,0x00,0x00), // 8
                RGBA(0x00,0xFF,0x00), // 9
                RGBA(0x00,0x00,0xFF), // 10
                RGBA(0x00,0xFF,0xFF), // 11
                RGBA(0xFF,0x00,0xFF), // 12
                RGBA(0xFF,0xFF,0x00), // 13
                // Dark colors
                RGBA(127,127,127),    // 14
                RGBA(127,0x00,0x00),  // 15
                RGBA(0x00,127,0x00),  // 16
                RGBA(0x00,0x00,127),  // 17
                RGBA(0x00,127,127),   // 18
                RGBA(127,0x00,127),   // 19
                RGBA(127,127,0x00),   // 20
          }), [&](float deltaTime) -> core::Matrix {
            return m_map;
          }) {
        }

        ~Window() {

        }

        ///
        /// Sets new map and redraws window.
        int setMap(const core::Matrix& map) {
            m_map = map;
            return m_window.update();
        }

        ///
        /// Takes a screenshot and saves it to png file.
        void screenshot(const std::string filename) {
            m_window.screenshot(filename);
        }

        void reset() {
            for(auto& row : m_map) {
                for(auto& scalar : row) {
                    scalar = 0;
                }
            }
        }

        bool shouldClose() {
            return m_window.shouldClose();
        }
    private:
        typedef core::RGBA RGBA;
        window::Window m_window;
        core::Matrix m_map;
    };
}
