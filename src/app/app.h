#pragma once

#include <config.h>
extern float offsetX;
namespace App {
    void init();
    void update(float deltaTime, float currentTime);
    void render();
    void close();
}