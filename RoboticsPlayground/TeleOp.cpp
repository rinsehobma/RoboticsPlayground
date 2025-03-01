#include <Windows.h>
#include <iostream>
#include <conio.h>

void DoTeleOp() {
    if (GetAsyncKeyState(VK_UP)) {
        std::cout << "Going forward" << std::endl;
    }

    if (GetAsyncKeyState(VK_DOWN)) {
        std::cout << "Going backward" << std::endl;
    }

    if (GetAsyncKeyState(VK_LEFT)) {
        std::cout << "Rotating left" << std::endl;
    }

    if (GetAsyncKeyState(VK_RIGHT)) {
        std::cout << "Rotating right" << std::endl;
    }


}