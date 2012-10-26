#include "main.hpp"

int main(int argc, char **argv) {
  Display::create(1280, 720);
  System::initialize();

  while (!Keyboard::isKeyDown(KEY_ESCAPE)) {
    System::update();
    System::render();
    Display::update();
  }

  return 0;
}
