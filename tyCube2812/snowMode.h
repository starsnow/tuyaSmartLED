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
extern CLEDController* FastLEDControllers[MATRIX_NUM];

class SnowMode : public RenderMode
{
private:
    static const uint8_t SNOW_NUM = 30;
    const uint8_t DISAPPEAR_RATE_TOP = 200;     
    const uint8_t DISAPPEAR_RATE_BOTTOM = 240;     
    const uint8_t THAW_RATE = 200;     
    const CRGB SNOW_COLOR = CRGB::White;

    unsigned int renderInterval;
    unsigned int thawInterval;      // 积雪融化的间隔时间
    uint8_t newDotChance;
    CRGB *pLedsTop, *pLedsBottom, *pLedsSide;
    Point snows[SNOW_NUM];
    CRGB emptyDot, snowDot;

public:
    SnowMode()
    {
        renderInterval = 150;
        thawInterval = renderInterval * 15;
        newDotChance = 50;

        // 由于要使用到奇偶校验位，需要特殊处理一下雪花点，保证奇偶校验位是 0
        emptyDot = CRGB::Black;
        snowDot  = SNOW_COLOR;
        emptyDot.setParity(0);
        snowDot.setParity(0);
    }

    String getName() 
    {
        return F("snow mode");
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

        pLedsTop    = leds[UP_SIDE];
        pLedsBottom = leds[UP_SIDE];
        pLedsSide   = leds[LEFT_SIDE];
    }

    // 使用接口设置奇偶校验值为0，用来为侧面做为是否有积雪的标识，省去一个另做一个数组的空间和麻烦
    void clearPaity(CRGB *pLeds, uint8_t ledNum)
    {
        for (uint8_t i = 0; i < ledNum; ++i)
        {
            pLeds[i].setParity(0);
        }
    }

    // 飘雪效果上面的渲染
    // 上面是随机点渐暗的效果
    void renderSnowTop()
    {
        for (uint8_t i = 0; i < NUM_LEDS_PER_MATRIX; ++i)
        {
            pLedsTop[i].nscale8(DISAPPEAR_RATE_TOP);
        }

        if (random8(100) > (newDotChance))
        {
            return;
        }

        pLedsTop[XY(random8(8), random8(8))] = snowDot;
    }

    // 飘雪效果底面的渲染
    // 底面也是渐暗的效果，不过让白点停留的时间久一些
    void renderSnowBottom()
    {
        for (uint8_t i = 0; i < NUM_LEDS_PER_MATRIX; ++i)
        {
            pLedsBottom[i].nscale8(DISAPPEAR_RATE_BOTTOM);
        }

        if (random8(100) > (newDotChance))
        {
            return;
        }

        pLedsBottom[XY(random8(8), random8(8))] = snowDot;
    }

    // 融化一层积雪
    // 融化最下一层雪，和融化最上一层，视觉效果一样
    // 融化函数的调用和生成函数调用是同步的，内部再做延迟判断
    // 在同步调用的过程，我们将最下一行进行淡化，直到要清除的时候，
    // 把最下一行有积雪的再设置为最亮，把最上一行清掉，这样看上去就像是最下一行积雪融化一样
    void thaw(CRGB pLeds[], uint8_t dir)
    {
        static unsigned long lastTick = millis();
        uint8_t x, y;
        CRGB *pLed;

        for (x = 0; x < MATRIX_WIDTH; ++x)
        {
            pLed = &pLeds[XY(x, MATRIX_HEIGHT - 1, dir)];
            if (pLed->getParity() == 1)
            {
                pLed->nscale8(THAW_RATE);
                pLed->setParity(1);
            }
        }

        if (millis() - lastTick < thawInterval)
            return;

        lastTick = millis();

        // 找每一列的第一个积雪点，清除
        for (x = 0; x < MATRIX_WIDTH; ++x)
        {
            for (y = 0; y < MATRIX_HEIGHT; ++y)
            {
                pLed = &pLeds[XY(x, y, dir)];
                if (pLed->getParity() == 0)
                    continue;

                *pLed = emptyDot;
                pLed->setParity(0);
                break;
            }
        }

        for (x = 0; x < MATRIX_WIDTH; ++x)
        {
            pLed = &pLeds[XY(x, MATRIX_HEIGHT - 1, dir)];
            if (pLed->getParity() == 1)
            {
                *pLed = snowDot;
            }
        }
    }
    
    // 飘雪效果侧面的渲染
    // 小白点下落，并且会积累在最下面，每过一定时间清除掉一行
    void renderSnowSide(CRGB pLeds[], uint8_t dir)
    {
        Point underPoint;
        int8_t i;

        // 先做一下融化积雪，再处理雪花下落
        thaw(pLeds, dir);

        // 从后往前进行，向下移动雪花
        for (i = SNOW_NUM - 1; i >= 0; --i)
        {
            if (snows[i].x == -1)
                continue;

            // 如果下一个位置有雪花或是已经在最下一层，则停止下落，成为积雪
            underPoint.x = snows[i].x;
            underPoint.y = snows[i].y + 1;
            if (underPoint.y >= MATRIX_HEIGHT || pLeds[XY(underPoint.x, underPoint.y, dir)].getParity() == 1)
            {
                pLeds[XY(snows[i].x, snows[i].y, dir)].setParity(1);
                snows[i].x = -1;

                continue;
            }

            // 下移雪花
            pLeds[XY(snows[i].x, snows[i].y, dir)] = emptyDot;
            snows[i].y++;
            pLeds[XY(snows[i].x, snows[i].y, dir)] = snowDot;
        }

        // 根据几率来生成一个雪花
        if (random8(100) > newDotChance)
            return;

        for (i = 0; i < SNOW_NUM; ++i)
        {
            // 找到空余头部点位置的时候才生成新的头部点
            if (snows[i].x != -1)
                continue;

            // 随机生成一个点，不过如果这个位置积雪太高的话就弃用，再选过。这里选择四层就不再落雪了
            // 可以用个计数来做尝试次数，不过省一个变量是一个，就直接循环到数组结束就行，效果差不到哪去
            snows[i].x = random8(MATRIX_WIDTH);
            if (pLeds[XY(snows[i].x, 3, dir)].getParity() == 1)
            {
                snows[i].x = -1;
                continue;
            }

            snows[i].y = 0;
            pLeds[XY(snows[i].x, snows[i].y, dir)] = snowDot;
            break;
        }
    }

    void render() 
    {
        renderSnowTop();
        // renderSnowBottom();
        renderSnowSide(pLedsSide, CW0);
    }
    
    void input(uint8_t)
    {
        
    }
};

#endif