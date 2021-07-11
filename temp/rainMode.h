// rainMode.h
// seesea 2021-07-09
// 雨滴模式

#ifndef _RAIN_MODE_H_
#define _RAIN_MODE_H_

#include "cube2812.h"

class RainMode : public renderMode
{
private:
    unsigned int renderInterval;
    Point head[10];

public:
    RainMode()
    {
        renderInterval = 100;    
    }

    String getName() 
    {
        return "rain mode";
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
        FastLEDControllers[FRONT_SIDE]->setLeds(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[BACK_SIDE]->setLeds(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
    }

    // 雨滴效果上下面的渲染
    // 和黑客帝国效果一样
    void renderRainTop()
    {
        nscale8(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);

        if (random8(100) > 10)
            return;

        randomDot(pLeds, CRGB::White);
    }

    // 雨滴效果侧面的渲染
    // 和黑客帝国效果差不多
    void renderRainSide()
    {
        const uint8_t DISAPPEAR_RATE = 100;
        const CRGB RAIN_COLOR = CRGB::Green;
        CRGB *pLed;
        uint8_t i;

        for (uint8_t y = 0; y < 8; ++y)
        {
            for (uint8_t x = 0; x < 8; ++x)
            {
                // 淡化
                pLed = &pLeds[XY(x, y, dir)];
                pLed->nscale8(DISAPPEAR_RATE);

                if (y >= 1)
                {
                    continue;
                }

                // y == 0 的时候，根据几率来生成一个头部点
                if (random8(100) > 10)
                    continue;

                for (i = 0; i < 10; ++i)
                {
                    // 找到空余头部点位置的时候才生成新的头部点
                    if (head[i].x != -1)
                        continue;

                    head[i].x = random8(MATRIX_WIDTH);
                    head[i].y = y;
                    break;
                }
            }
        }

        // 向下移动头部的坐标
        for (i = 0; i < 10; ++i)
        {
            if (head[i].x == -1)
                continue;

            pLeds[XY(head[i].x, head[i].y, dir)] = RAIN_COLOR;

            head[i].y++;
            if (head[i].y >= MATRIX_HEIGHT)
            {
                head[i].x = -1;
                continue;
            }
        }
    }

    void render() 
    {
        renderHackMatrixTop(leds[UP_SIDE]);
        renderHackMatrixSide(leds[LEFT_SIDE], CW0);
    }
};

#endif