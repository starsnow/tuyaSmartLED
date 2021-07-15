// snowMode.h
// seesea 2021-07-09
// 飘雪模式

#ifndef _SNOW_MODE_H_
#define _SNOW_MODE_H_

#include "cube2812.h"
#include "modeBaseClass.h"

// FastLED 的 LED 数据
// 内存不够用，只用三个面
extern CRGB leds[MATRIX_BUFFER_NUM][NUM_LEDS_PER_MATRIX];

// 各个 FastLED 控制器
extern CLEDController* FastLEDControllers[MATRIX_BUFFER_NUM];

class SnowMode : public RenderMode
{
private:
    static const uint8_t SNOW_NUM = 30;
    unsigned int renderInterval;
    uint8_t newDotChance;
    uint8_t brightnessDelta;
    Point bubbles[BUBBLE_NUM];
    CRGB *pLedsTop, *pLedsBottom, *pLedsSide;

public:
    BubbleMode()
    {
        renderInterval = 100;
        newDotChance = 50;
        brightnessDelta = 30;
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
        // 对每一个非黑点进行处理，如果达到最亮白色则设置为黑点，否则加上一些亮度
        for (uint8_t i = 0; i < NUM_LEDS_PER_MATRIX; ++i)
        {
            if (! pLedsTop[i])
                continue;
            
            // if (pLedsTop[i].getAverageLight() == 0xFF)
            if (pLedsTop[i] == (CRGB) CRGB::White)
            {
                pLedsTop[i] = CRGB::Black;
                continue;
            }

            pLedsTop[i].addToRGB(brightnessDelta);
        }

        // 根据几率设置一个点为非黑点进行渐亮
        if (random8(100) > (100 - newDotChance))
        {
            return;
        }

        pLedsTop[XY(random8(8), random8(8))] = 0x010101;
    }

    // 气泡效果底面的渲染
    // 底面是渐暗的效果
    void renderBubbleBottom()
    {
        // 对每一个非黑点进行渐暗处理
        for (uint8_t i = 0; i < NUM_LEDS_PER_MATRIX; ++i)
        {
            if (! pLedsBottom[i])
                continue;
            
            pLedsBottom[i].subtractFromRGB(brightnessDelta);
        }

        // 根据几率设置一个点为白色进行渐暗
        if (random8(100) > (100 - newDotChance))
        {
            return;
        }

        pLedsBottom[XY(random8(8), random8(8))] = CRGB::White;
    }

    // 气泡效果侧面的渲染
    // 就是小白点往上升就好了
    void renderRainSide(CRGB pLeds[], uint8_t dir)
    {
        const CRGB BUBBLE_COLOR = CRGB::White;
        CRGB *pLed;
        uint8_t i;

        // 向上移动气泡点
        for (i = 0; i < BUBBLE_NUM; ++i)
        {
            if (bubbles[i].x == -1)
                continue;

            pLeds[XY(bubbles[i].x, bubbles[i].y, dir)] = CRGB::Black;

            bubbles[i].y--;
            if (bubbles[i].y < 0)
            {
                bubbles[i].x = -1;
                continue;
            }
            pLeds[XY(bubbles[i].x, bubbles[i].y, dir)] = BUBBLE_COLOR;
        }

        // 根据几率来生成一个气泡
        if (random8(100) > (100 - newDotChance))
            return;

        for (i = 0; i < BUBBLE_NUM; ++i)
        {
            // 找到空余头部点位置的时候才生成新的头部点
            if (bubbles[i].x != -1)
                continue;

            bubbles[i].x = random8(MATRIX_WIDTH);
            bubbles[i].y = MATRIX_HEIGHT;
            break;
        }
    }

    void render() 
    {
        renderBubbleTop();
        renderBubbleBottom();
        renderRainSide(pLedsSide, CW0);
    }
};

#endif