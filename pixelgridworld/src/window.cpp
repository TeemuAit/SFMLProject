//// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= ////
//// Pixelwridworld
////
//// MIT Licanse
////
//// Copyright (c) 2022 Mikko Romppainen.
////
//// Permission is hereby granted, free of charge, to any person obtaining
//// a copy of this software and associated documentation files (the
//// "Software"), to deal in the Software without restriction, including
//// without limitation the rights to use, copy, modify, merge, publish,
//// distribute, sublicense, and/or sell copies of the Software, and to
//// permit persons to whom the Software is furnished to do so, subject to
//// the following conditions:
////
//// The above copyright notice and this permission notice shall be included
//// in all copies or substantial portions of the Software.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= ////
#include <pixelgridworld/window.h>
#include <glad/gl.h>		// Include glad
#include <GLFW/glfw3.h>		// Include glfw
#include <string>
#include <vector>
#include <stdexcept>
#include <stb_image_write.h>
#include <assert.h>

// If ytou want to take screenshots, you must speciy following:
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>


namespace window {
    // Class for static initialization of glfw
    class StaticInit {
    public:
        StaticInit() {
            // Set c++-lambda as error call back function for glfw.
            glfwSetErrorCallback([](int error, const char* description) {
                fprintf(stderr, "Error %d: %s\n", error, description);
            });
            // Try to initialize glfw
            if (!glfwInit()) {
                throw std::runtime_error("Failed to initialize OpenGL!");
                return;
            }
        }
        ~StaticInit() {
            // Terminate glfw
            glfwTerminate();
        }
    };

    static window::StaticInit init;



    Window::Window(int argc, char **argv, int sizeX, int sizeY, const std::string& title, const std::vector<core::RGBA>& palette, RunFunc run)
        : runFunc(run), m_width(sizeX), m_height(sizeY), m_palette(palette), m_window(0), m_outputTexture(0) {
        // Create window and check that creation was succesful.
        m_window = glfwCreateWindow(m_width+2, m_height+2, title.c_str(), NULL, NULL);
        if (!m_window) {
            throw std::runtime_error("Failed to create window!");
            return;
        }

        // Set current context
        glfwMakeContextCurrent(m_window);
        // Load GL functions using glad
        gladLoadGL(glfwGetProcAddress);

        // Specify the key callback as c++-lambda to glfw
        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            // Close window if escape is pressed by the user.
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
        });

        glfwSetWindowSizeLimits(m_window,m_width+2,m_height+2,GLFW_DONT_CARE,GLFW_DONT_CARE);

        // Make outputTexture
        glGenTextures(1, &m_outputTexture);
        glBindTexture(GL_TEXTURE_2D, m_outputTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
        // Get time using glfwGetTime-function, for delta time calculation.
        prevTime = (float)glfwGetTime();
    }

    Window::~Window() {
        glDeleteTextures(1, &m_outputTexture);
        // Destroy window
        glfwDestroyWindow(m_window);
    }

    int Window::update() {
        if (shouldClose()) {
            return -1;
        }
        // Set current context
        glfwMakeContextCurrent(m_window);
        // Compute application frame time (delta time) and update application
        float curTime = (float)glfwGetTime();
        float deltaTime = curTime - prevTime;
        prevTime = curTime;
        renderMap(runFunc(deltaTime));
        // Render the game frame and swap OpenGL back buffer to be as front buffer.
        glfwSwapBuffers(m_window);
        // Poll other window events.
        glfwPollEvents();
        if (glfwWindowShouldClose(m_window)) {
            return -1;
        }
        return 0;
    }

    int Window::run() {
        if (shouldClose()) {
            return -1;
        }
        while (!shouldClose()) {
            // Set current context
            glfwMakeContextCurrent(m_window);
            // Query the size of the framebuffer (window content) from glfw.
            int width, height;
            glfwGetFramebufferSize(m_window, &width, &height);

            // Setup the opengl wiewport (i.e specify area to draw)
            glViewport(0, 0, width, height);

            // Compute application frame time (delta time) and update application
            float curTime = (float)glfwGetTime();
            float deltaTime = curTime - prevTime;
            prevTime = curTime;
            renderMap(runFunc(deltaTime));
            // Render the game frame and swap OpenGL back buffer to be as front buffer.
            glfwSwapBuffers(m_window);
            // Poll other window events.
            //screenshot("screenshot.png");
        }
        return 0;
    }

    void Window::screenshot(const std::string filename) {
        // Set current context
        glfwMakeContextCurrent(m_window);
        int width, height;
        int channels = 4;
        glfwGetFramebufferSize(m_window, &width, &height);
        uint8_t* data = (unsigned char*)malloc(channels*width*height);
        glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE, data);
        stbi_flip_vertically_on_write(true);
        stbi_write_png(filename.c_str(), width, height, channels, data, width*channels);
        stbi_flip_vertically_on_write(false);
        free(data);
    }

    bool Window::shouldClose() {
        glfwPollEvents();
        return m_window == 0 || glfwWindowShouldClose(m_window);
    }


    void Window::renderMap(const core::Matrix& map) {
        std::vector<uint8_t> mapData;
        int mapWidth = 0;
        int mapHeight = 0;
        for(auto row : map) {
            ++mapHeight;
            for(auto index : row) {
                assert(index >= 0 && index <m_palette.size());
                auto& color = m_palette[index];
                mapData.push_back(color.r);
                mapData.push_back(color.g);
                mapData.push_back(color.b);
                mapData.push_back(color.a);
                ++mapWidth;
            }
        }
        mapWidth /= mapHeight;
        // Copy outputData to outputTexture
        glBindTexture(GL_TEXTURE_2D, m_outputTexture);
        // This needs to be called because on some cases the texture data has some different row alignment.
        // For more info see: https://www.khronos.org/opengl/wiki/Pixel_Transfer#Pixel_layout
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // set the texture data as RGB
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mapWidth, mapHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &mapData[0]);
        // set the texture wrapping options to repeat
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        // set the texture filtering to nearest (disabled filtering)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, 0);
        glClearColor(0.5f,0.0f,0.5,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Query the size of the framebuffer (window content) from glfw.
        int screenWidth, screenHeight;
        glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);

        // Setup the opengl wiewport (i.e specify area to draw)
        glViewport(0, 0, screenWidth, screenHeight);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Set opengl MVP matrices
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, screenWidth, screenHeight, 0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPushMatrix();
        glLoadIdentity();
        // Translate plane to center of screen.
        glTranslatef(0.5f*(screenWidth-m_width), 0.5f*(screenHeight-m_height), 0);

        // Draw output texture to the screen
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_outputTexture);
        glBegin(GL_QUADS);
        glTexCoord2d(0, 0); glVertex2d(1.0f, 1.0f);
        glTexCoord2d(0, 1); glVertex2d(1.0f, m_height);
        glTexCoord2d(1, 1); glVertex2d(m_width, m_height);
        glTexCoord2d(1, 0); glVertex2d(m_width, 1.0f);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        // Pop opengl MVP matrices
        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }


}
