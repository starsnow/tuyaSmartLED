// energyCubeMode.h
// seesea 2021-07-09
// 能量魔方模式

#ifndef _ENERGY_CUBE_MODE_H_
#define _ENERGY_CUBE_MODE_H_

#include "cube2812.h"
#include "modeBaseClass.h"

// FastLED 的 LED 数据
// 内存不够用，只用三个面
extern CRGB leds[MATRIX_BUFFER_NUM][NUM_LEDS_PER_MATRIX];

// 各个 FastLED 控制器
extern CLEDController* FastLEDControllers[MATRIX_BUFFER_NUM];

class EnergyCubeMode : public RenderMode
{
private:
    static const uint8_t SCALE_RATE = 200;  // 变暗的比率
    static const uint8_t CORE_CHANCE = 90;  // 核心新出能量点的几率
    static const uint8_t EDGE_CHANCE = 50;  // 边缘新出能量点的几率
    unsigned int renderInterval;
    CRGB *pLeds;

public:
    EnergyCubeMode()
    {
        renderInterval = 5;
    }

    String getName() 
    {
        return F("energy cube mode");
    }

    unsigned int getRenderInterval()
    {
        return renderInterval;
    }

    void init()
    {
        FastLEDControllers[UP_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[DOWN_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[LEFT_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[RIGHT_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[FRONT_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
        FastLEDControllers[BACK_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);

        pLeds = leds[UP_SIDE];
    }

    inline bool isCore(uint8_t x, uint8_t y)
    {

    }

    void render() 
    {
        // 对每一个非黑点进行渐暗处理
        for (uint8_t i = 0; i < NUM_LEDS_PER_MATRIX; ++i)
        {
            if (! pLedsBottom[i])
                continue;
            
            pLedsBottom[i].nscale8(SCALE_RATE);
        }

        // // 根据几率设置一个点为白色进行渐暗
        // if (random8(100) > (100 - newDotChance))
        // {
        //     return;
        // }

        // pLedsBottom[XY(random8(8), random8(8))] = CRGB::White;
        // 根据是否核心点来确认产生几率以及颜色
    }
};

#endif
