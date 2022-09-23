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
#include <pixelgridworld/gridworld.h>

int main() {
    pixelgridworld::Window window("Pixelgridworld demo", 512);

    // Specify a map and some functions for it:
    core::Matrix map;
    auto clear = [&map]() {
        map = core::matN(32,1);
    };
    auto setPixel = [&map](const core::Vector& pos, int color) {
        auto x = pos[0];
        auto y = pos[1];
        map[y][x] = color;
    };
    auto getPixel = [&map](const core::Vector& pos) -> int {
        auto x = pos[0];
        auto y = pos[1];
        return map[y][x];
    };

    clear();
    while(window.shouldClose() == false){
        // TODO: update map, now just put some pixels

        setPixel({5, 1}, 15);
        setPixel({5, 2}, 10);

        for(int i=0; i<=20; ++i) {
            setPixel({i, i}, i);
        }

        printf("Pixel 5,1 is %d\n", getPixel({5,1}));

        window.setMap(map);
        window.screenshot("screenshot.png");
    }

    return 0;
}
