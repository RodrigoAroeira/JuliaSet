#pragma once

namespace Globals {

extern short WIDTH;
extern short HEIGHT;
extern bool PAUSED;
extern bool RELOAD;
extern float ZOOM;
extern float PAN_X;
extern float PAN_Y;

namespace Constants {

const int MAX_ITERATIONS = 100;
const double X_LIM = 1.5;
const double Y_LIM = 1.5;
const float ZOOM_DEFAULT = 2.0f;
} // namespace Constants
} // namespace Globals
