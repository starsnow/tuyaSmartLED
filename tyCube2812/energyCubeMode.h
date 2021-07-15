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

DEFINE_GRADIENT_PALETTE(energyCube_gp) {
    0,   0x00, 0xFF, 0xFF,      // 亮蓝
    50,  0x00, 0xC1, 0xD1,      // 暗蓝
    255, 0x00, 0x00, 0x00       // 黑
};

class EnergyCubeMode : public RenderMode
{
private:
    static const uint8_t SCALE_RATE = 200;  // 变暗的比率
    static const CRGBPalette16 energyCubeColorPallette = energyCube_gp;

    unsigned int renderInterval;
    CRGB *pLeds;
    
    uint8_t orignalBrigheness;            // 旧的亮度信息保存下来，以便在退出此模式的时候恢复

public:
    EnergyCubeMode()
    {
        renderInterval = 5;
        orignalBrigheness = FastLED.getBrightness();
    }

    ~EnergyCubeMode()
    {
        // 析构的时候恢复原来的亮度信息
        FastLED.setBrightness(orignalBrigheness);
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

    // 避免小数运算又避免计算结果超256，将坐标扩大两倍来计算
    // x,y : 0-7，中心点 3.5，扩大两倍后就是 0-14，中心点 7。最大值 (14 - 7)^2 * 2 = 98
    // 减少计算量并且就是要指数效果，使用距离的平方即可
    inline uint8_t calcDistanceToCore(uint8_t x, uint8_t y)
    {
        uint8_t dx = (x > 7 ? x - 7 : 7 - x);
        uint8_t dy = (x > 7 ? x - 7 : 7 - x);
        return dx * dx + dy * dy;
    }

    // 计算几率，越中心几率越大
    // x 0 1 2 3 4 5 6 7
    // 0 0 0 0 0 0 0 0 0
    // 1 0 0 0 0 0 0 0 0
    // 2 0 0 0 9 9 0 0 0
    // 3 0 0 9 9 9 9 0 0
    // 4 0 0 9 9 9 9 0 0
    // 5 0 0 0 9 9 0 0 0
    // 6 0 0 0 0 0 0 0 0
    // 7 0 0 0 0 0 0 0 0
    // 使用上述扩大两倍后的距离计算公式，最小距离平方是 1，最大距离平方是 98
    inline uint8_t calcNewDotChance(uint8_t distanceToCore)
    {
        return 100 - distanceToCore;
    }

    // 根据离中心的距离获取新点的颜色
    inline CRGB getNewDotColor(uint8_t distanceToCore)
    {
        uint8_t index = map(distanceToCore, 0, 100, 0, 240);
        return ColorFromPalette(energyCubeColorPallette, index);
    }

    void render() 
    {
        uint8_t distanceToCore;

        for (uint8_t x = 0; x < MATRIX_WIDTH; ++x)
        {
            for (uint8_t y = 0; y < MATRIX_HEIGHT; ++y)
            {
                // 每个点每次降低一些亮度
                pLeds[i].nscale8(SCALE_RATE);

                // 根据距离中心的距离来选择几率及颜色设置新的亮点
                distanceToCore = calcDistanceToCore(x, y);
                if (random8(100) > (100 - calcNewDotChance(distanceToCore)))
                {
                    continue;
                }

                pLeds[XY(x, y)] = getNewDotColor(distanceToCore);                                
            }
        }

        // 设置一些亮度波动
        FastLED.setBrightness(nscale8(orignalBrigheness, beatsin8(10, 100, 255)));
    }
};

#endif
