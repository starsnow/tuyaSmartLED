// bubbleMode.h
// seesea 2021-07-09
// 气泡模式

#ifndef _BUBBLE_MODE_H_
#define _BUBBLE_MODE_H_

#include "cube2812.h"
#include "modeBaseClass.h"

// FastLED 的 LED 数据
// 内存不够用，只用三个面
extern CRGB leds[MATRIX_BUFFER_NUM][NUM_LEDS_PER_MATRIX];

// 各个 FastLED 控制器
extern CLEDController* FastLEDControllers[MATRIX_BUFFER_NUM];

class BubbleMode : public RenderMode
{
private:
    unsigned int renderInterval;
    uint8_t newDotChance;
    uint8_t scaleRate;
    Point head[10];
    CRGB *pLedsTop, *pLedsBottom, *pLedsSide;

public:
    RainMode()
    {
        renderInterval = 100;
        newDotChance = 20;
        scaleRate = 100;
    }

    String getName() 
    {
        return F("bubble mode");
    }

    unsigned int getRenderInterval()
    {
        return renderInterval;
    }

    void init()
    {
        FastLEDControllers[UP_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[DOWN_SIDE]->setLeds(leds[DOWN_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[LEFT_SIDE]->setLeds(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[RIGHT_SIDE]->setLeds(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[FRONT_SIDE]->setLeds(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[BACK_SIDE]->setLeds(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);

        pLedsTop    = leds[UP_SIDE];
        pLedsBottom = leds[DOWN_SIDE];
        pLedsSide   = leds[LEFT_SIDE];
    }

    // 气泡效果上面的渲染
    // 上面是渐亮的效果
    void renderBubbleTop()
    {

    }

    // 气泡效果底面的渲染
    // 底面是渐暗的效果
    void renderBubbleBottom()
    {

    }

    // 气泡效果侧面的渲染
    void renderRainSide(CRGB pLeds[], uint8_t dir)
    {
        const uint8_t DISAPPEAR_RATE = 100;
        const CRGB RAIN_COLOR = CRGB::White;
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
                if (random8(100) > (100 - newDotChance))
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
        renderRainTop();
        renderRainSide(pLedsSide, CW0);
    }
};

#endif