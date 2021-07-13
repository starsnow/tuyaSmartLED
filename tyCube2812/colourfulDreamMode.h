// colourfulDreamMode.h
// seesea 2021-07-09
// 五彩梦幻模式

#ifndef _COLOURFUL_DREAM_MODE_H_
#define _COLOURFUL_DREAM_MODE_H_

#include "cube2812.h"

// FastLED 的 LED 数据
// 内存不够用，只用三个面
extern CRGB leds[MATRIX_BUFFER_NUM][NUM_LEDS_PER_MATRIX];

// 各个 FastLED 控制器
extern CLEDController* FastLEDControllers[MATRIX_BUFFER_NUM];

class ColourfulDreamMode : public RenderMode
{
private:
    unsigned int renderInterval;
    const uint8_t DISAPPEAR_RATE = 250;

public:
    ColourfulDreamMode()
    {
        renderInterval = 100;
    }

    String getName() 
    {
        return F("colourful dream mode");
    }

    unsigned int getRenderInterval()
    {
        return renderInterval;
    }

    void init()
    {
        FastLEDControllers[UP_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[DOWN_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[LEFT_SIDE]->setLeds(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[RIGHT_SIDE]->setLeds(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[FRONT_SIDE]->setLeds(leds[FRONT_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[BACK_SIDE]->setLeds(leds[FRONT_SIDE], NUM_LEDS_PER_MATRIX);
    }

    void randPutAColorDot(CRGB *pLeds)
    {
        if (random8(100) > 90)
            return;

        randomDot(pLeds, CRGB(random8(), random8(), random8()));
    }

    void render() 
    {
        nscale8(leds[UP_SIDE], NUM_LEDS_PER_MATRIX, DISAPPEAR_RATE);
        nscale8(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX, DISAPPEAR_RATE);
        nscale8(leds[FRONT_SIDE], NUM_LEDS_PER_MATRIX, DISAPPEAR_RATE);

        randPutAColorDot(leds[UP_SIDE]);
        randPutAColorDot(leds[LEFT_SIDE]);
        randPutAColorDot(leds[FRONT_SIDE]);
    }
};

#endif
