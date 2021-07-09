// hackMatrixMode.h
// seesea 2021-07-09
// 黑客帝国效果模式

#ifndef _HACK_MATRIX_MODE_H_
#define _HACK_MATRIX_MODE_H_

#include "cube2812.h"

class HackMatrixMode : public renderMode
{
private:
    unsigned int renderInterval;

public:
    HackMatrixMode()
    {
        renderInterval = 100;
    }

    String getName() 
    {
        return "hack matrix mode";
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

    // 黑客帝国效果，侧面
    void renderHackMatrixSide(CRGB pLeds[], uint8_t dir)
    {
        static Point head[10];
        const CRGB HEAD_COLOR = CRGB::White;
        const CRGB BODY_COLOR = CRGB::Green;
        const uint8_t DISAPPEAR_RATE = 100;
        CRGB *pLed;    

        uint8_t i;

        for (uint8_t y = 0; y < 8; ++y)
        {
            for (uint8_t x = 0; x < 8; ++x)
            {
                pLed = &pLeds[XY(x, y, dir)];
                if (*pLed == HEAD_COLOR)
                {
                    *pLed = BODY_COLOR;
                    continue;
                }

                // 淡化
                pLed->nscale8(DISAPPEAR_RATE);

                if (y >= 1)
                {
                    // pLeds[XY(x, y, dir)] = pLeds[XY(x, y - 1, dir)];
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

            pLeds[XY(head[i].x, head[i].y, dir)] = HEAD_COLOR;

            head[i].y++;
            if (head[i].y >= MATRIX_HEIGHT)
            {
                head[i].x = -1;
                continue;
            }
        }
    }

    // 黑客帝国效果，顶和底面
    void renderHackMatrixTop(CRGB pLeds[])
    {

    }

    void render() 
    {
        renderHackMatrixTop(leds[UP_SIDE]);
        renderHackMatrixSide(leds[LEFT_SIDE], CW0);
    }
};

#endif