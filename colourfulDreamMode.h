// colourfulDreamMode.h
// seesea 2021-07-09
// 雨滴模式

#ifndef _COLOURFUL_DREAM_MODE_H_
#define _COLOURFUL_DREAM_MODE_H_

#include "cube2812.h"

class ColourfulDreamMode : public renderMode
{
private:
    unsigned int renderInterval;

public:
    ColourfulDreamMode()
    {
        renderInterval = 100;
    }

    String getName() 
    {
        return "colourful dream mode";
    }

    unsignd long renderInterval()
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
        if (random8(100) > 70)
            return;

        randomDot(pLeds, CRGB::White);
    }

    void render() 
    {
        const uint8_t DISAPPEAR_RATE = 50;

        nscale8(leds[UP_SIDE], NUM_LEDS_PER_MATRIX, DISAPPEAR_RATE);
        nscale8(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX, DISAPPEAR_RATE);
        nscale8(leds[FRONT_SIDE], NUM_LEDS_PER_MATRIX, DISAPPEAR_RATE);

        randPutAColorDot(leds[UP_SIDE]);
        randPutAColorDot(leds[LEFT_SIDE]);
        randPutAColorDot(leds[FRONT_SIDE]);
    }
};

#endif