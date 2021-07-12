// starSkyMode.h
// seesea 2021-07-09
// 星空模式

#ifndef _STAR_SKY_MODE_H_
#define _STAR_SKY_MODE_H_

#include "cube2812.h"
#include "modeBaseClass.h"

// FastLED 的 LED 数据
// 内存不够用，只用三个面
extern CRGB leds[MATRIX_BUFFER_NUM][NUM_LEDS_PER_MATRIX];

// 各个 FastLED 控制器
extern CLEDController* FastLEDControllers[MATRIX_BUFFER_NUM];

const uint8_t MAX_STAR_NUM = 10;

class Star 
{
    // Point pos;
    CRGB *pLed;
    uint8_t life;
    uint8_t curLife;
    uint8_t brightnessRate;

public:
    start()
    {
        life = 0;
    }

    star(CRGB *pLeds, uint8_t _x, uint8_t _y, uint8_t _life, uint8_t _brightnessRate)
    {
        life = _life;
        curLife = 0;
        brightnessRate = _brightnessRate;
        pLed = &pLeds[XY(_x, _y)];
        *pLed = CRGB::Black;
    }

    void init(CRGB *pLeds, uint8_t _x, uint8_t _y, uint8_t _life, uint8_t _brightnessRate)
    {
        star(pLeds, _x, _y, _life, _brightnessRate);
    }

    void twinkle()
    {
        // 生命结束
        if (curLife >= life)
        {
            life = 0;
            *pLed = CRGB::Black;
            return;
        }

        // 前半段生命是渐亮，后半段生命渐暗
        if (curLife < (life >> 1))
        {
            pLed->addToRGB(brightnessRate);
        }
        else
        {
            pLed->subtractFromRGB(brightnessRate);
        }

        curLife++;
    }

    bool isAlive()
    {
        return life > 0;
    }
};

class StarSkyMode : public RenderMode
{
private:
    unsigned int renderInterval;
    Star stars[MAX_STAR_NUM];
    CRGB *pLeds[MATRIX_BUFFER_NUM];

public:
    StarSkyMode()
    {
        renderInterval = 5;
    }

    String getName() 
    {
        return F("star sky mode");
    }

    unsigned long getRenderInterval()
    {
        return renderInterval;
    }

    void init()
    {
        // FastLED.clearData();
//        FastLED.clear();
        FastLEDControllers[UP_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[DOWN_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[LEFT_SIDE]->setLeds(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[RIGHT_SIDE]->setLeds(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[FRONT_SIDE]->setLeds(leds[FRONT_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[BACK_SIDE]->setLeds(leds[FRONT_SIDE], NUM_LEDS_PER_MATRIX);

        pLeds[0] = leds[UP_SIDE];
        pLeds[1] = leds[LEFT_SIDE];
        pLeds[2] = leds[FRONT_SIDE];
    }

    void render() 
    {
        bool addAStar = false;
        if (random8(100) < 5)
        {
            addAStar = true;
        }

        for (uint8_t i = 0; i < MAX_STAR_NUM; ++i)
        {
            if (! stars[i].isAlive())
            {
                if (addAStar)
                {
                    stars[i].init(pLeds[random8(MATRIX_BUFFER_NUM)], random8(8), random8(8), random8(100, 255), random8(1, 10));
                    addAStar = false;
                }

                continue;
            }

            stars[i].twinkle();
        }
    }
};

#endif
