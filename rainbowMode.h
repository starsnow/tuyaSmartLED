// rainbowMode.h
// seesea 2021-07-09
// 彩虹模式

#ifndef _RAINBOW_MODE_H_
#define _RAINBOW_MODE_H_

#include "cube2812.h"

class RainbowMode : public renderMode
{
private:
    unsigned int renderInterval;

public:
    RainbowMode()
    {
        renderInterval = 100;    
    }

    String getName() 
    {
        return "rainbow mode";
    }

    unsignd long renderInterval()
    {
        return renderInterval;
    }

    void init()
    {
        for (int i = 0; i < MATRIX_NUM; ++i)
        {
            FastLEDControllers[i]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
        }
    }

    void render() 
    {
        static uint8_t hue = -1;
        fill_rainbow(leds[UP_SIDE], NUM_LEDS_PER_MATRIX, hue++, 10);
    }
};

#endif