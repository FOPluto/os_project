#ifndef UI_H
#define UI_H

#include "GLUI/GL2DSlider.h"
#include "GLUI/GL2DButton.h"
#include <GL/glut.h>
#include <vector>
#include <ctime>
#include "semaphore.hpp"

using namespace std;

void changeSize(int w, int h);

void orientMe(float directionx, float directiony);

void moveMeFlat(int direction);

void mouse(int but, int state, int x, int y);

void mouseMotion(int x, int y);

void processSpecialKeys(int key, int x, int y);

void drawSphere(ItemRepository *ir, object *ob, int i);

void drawArrow();

void drawInit(  std::thread** producers, 
                std::thread** consumers,
                std::thread** sub_consumers,
                size_t produce_size,
                size_t consume_size,
                size_t sub_consume_size,
                ItemRepository * buffer1,
                ItemRepository * buffer2,
                std::unordered_map<std::string, int> &speed_map);


#endif // !UI_H
