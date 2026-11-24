#include "graphics.h"
#include "GlobalState.h"
#include "config.h"

void update(float dt) { GlobalState::getInstance()->update(dt); }
void draw() { GlobalState::getInstance()->draw(); }

int main() {
    graphics::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "A* Pathfinder");
    GlobalState::getInstance()->init();
    graphics::setUpdateFunction(update);
    graphics::setDrawFunction(draw);
    graphics::setCanvasSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    graphics::startMessageLoop(); 
    return 0;
}