// rainbowMode.h
// seesea 2021-07-09
// 彩虹模式

#ifndef _RAINBOW_MODE_H_
#define _RAINBOW_MODE_H_

#include "cube2812.h"
#include "modeBaseClass.h"

// FastLED 的 LED 数据
// 内存不够用，只用三个面
extern CRGB leds[MATRIX_BUFFER_NUM][NUM_LEDS_PER_MATRIX];

// 各个 FastLED 控制器
extern CLEDController* FastLEDControllers[MATRIX_BUFFER_NUM];

class RainbowMode : public RenderMode
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
        return F("rainbow mode");
    }

    unsigned int getRenderInterval()
    {
        return renderInterval;
    }

    void init()
    {
        for (uint8_t i = 0; i < MATRIX_NUM; ++i)
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
