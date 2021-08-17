// hackMatrixMode.h
// seesea 2021-07-09
// 黑客帝国效果模式

#ifndef _HACKER_MATRIX_MODE_H_
#define _HACKER_MATRIX_MODE_H_

#include "cube2812.h"
#include "modeBaseClass.h"

// FastLED 的 LED 数据
// 内存不够用，只用三个面
extern CRGB leds[MATRIX_BUFFER_NUM][NUM_LEDS_PER_MATRIX];

// 各个 FastLED 控制器
extern CLEDController* FastLEDControllers[MATRIX_NUM];

class HackerMatrixMode : public RenderMode
{
private:
    const uint8_t DISAPPEAR_RATE = 200;
    static const uint8_t HEAD_NUM = 20;
    const CRGB HEAD_COLOR = CRGB::DarkGray;
    const CRGB BODY_COLOR = CRGB::Green;
    
    unsigned int renderInterval;
    uint8_t newDotChance;
    Point head[HEAD_NUM];
    CRGB *pLedsTop, *pLedsSide;

public:
    HackerMatrixMode()
    {
        renderInterval = 100;
        newDotChance = 90;
    }

    String getName() 
    {
        return F("hacker matrix mode");
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
        FastLEDControllers[FRONT_SIDE]->setLeds(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[BACK_SIDE]->setLeds(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);

        pLedsTop  = leds[UP_SIDE];
        pLedsSide = leds[LEFT_SIDE];
    }

    // 黑客帝国效果，侧面
    void renderHackerMatrixSide(CRGB pLeds[], uint8_t dir)
    {
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
                    continue;
                }

                // y == 0 的时候，根据几率来生成一个头部点
                if (random8(100) > newDotChance)
                    continue;

                for (i = 0; i < HEAD_NUM; ++i)
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
        for (i = 0; i < HEAD_NUM; ++i)
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
    void renderHackerMatrixTop(CRGB pLeds[])
    {
        for (uint8_t i = 0; i < NUM_LEDS_PER_MATRIX; ++i)
        {
            if (pLeds[i] == HEAD_COLOR)
            {
                pLeds[i] = BODY_COLOR;
                continue;
            }
            pLeds[i].nscale8(DISAPPEAR_RATE);
        }

        if (random8(100) > newDotChance)
            return;

        randomDot(pLeds, HEAD_COLOR);
    }

    void render() 
    {
        renderHackerMatrixTop(leds[UP_SIDE]);
        renderHackerMatrixSide(leds[LEFT_SIDE], CW0);
    }


    void input(uint8_t)
    {
        
    }
};

#endif