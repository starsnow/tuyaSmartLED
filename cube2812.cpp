// cube2812.cpp
// Created by seesea 2021-07-07
// 2812 立方体的实现文件

#include "cube2812.h"

// 渲染函数指针变量/数组
static unsigned int (*renderFunction)() = 0;
static unsigned int (*lastRenderFunction)() = 0;
static unsigned int (*renderFunctions)() [] = {
    renderRainbow,          // RAINBOW             彩虹
    renderHackMatrix,       // HACKER_MATRIX       黑客帝国
    renderStarSky,          // STAR_SKY            星空
    renderRain,             // RAIN                雨
    renderColourfulDream,   // COLOURFUL_DREAM     五彩梦幻
    renderBubble,           // BUBBLE              气泡
    renderSnow,             // SNOW                雪
    renderEnergyCube,       // ENERGY_CUBE         能量魔方
};

static CLEDController& FastLEDControllers[MATRIX_NUM];

// 初始化
void initCube2812()
{
    FastLED.setBrightness(INIT_BRIGHTNESS);

    // 内存不足，所以所有的侧面都用同样的显示
    FastLEDControllers[UP_SIDE]    = FastLED.addLeds<CHIPSET, UP_PIN,    COLOR_ORDER>(leds[UP_SIDE],   NUM_LEDS_PER_MATRIX);
    FastLEDControllers[DOWN_SIDE]  = FastLED.addLeds<CHIPSET, DOWN_PIN,  COLOR_ORDER>(leds[DOWN_SIDE], NUM_LEDS_PER_MATRIX);
    FastLEDControllers[LEFT_SIDE]  = FastLED.addLeds<CHIPSET, LEFT_PIN,  COLOR_ORDER>(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
    FastLEDControllers[RIGHT_SIDE] = FastLED.addLeds<CHIPSET, RIGHT_PIN, COLOR_ORDER>(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
    FastLEDControllers[FRONT_SIDE] = FastLED.addLeds<CHIPSET, FRONT_PIN, COLOR_ORDER>(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
    FastLEDControllers[BACK_SIDE]  = FastLED.addLeds<CHIPSET, BACK_PIN,  COLOR_ORDER>(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
}

// 渲染刷新函数
void updateCube2812()
{
    static unsigned long last_time = 0;
    static unsigned int renderIntervalMs = 0;
    if (millis() - last_time > renderIntervalMs)
    {
        last_time = millis();
        renderIntervalMs = renderLED();
        FastLED.show();
    }
}

// 根据不同的渲染函数来渲染点阵，并返回下一次渲染的时间间隔
unsigned int renderLED() inline
{
    if (renderFunction == 0)
        return -1;

    return renderFunction();
}

void blinkLED(int times)
{
    for (int i = 0; i < times; ++i)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
    }
}

// 8 x 8 点阵随机一个位置设置为指定颜色
void randomDot(CRGB pLeds[], CRGB &color)
{
    uint8_t x = random8(8);
    uint8_t y = random8(8);
    pLeds[XY(x, y)] = color;
}

// 设置渲染模式
void setRenderMode(enum RENDER_MODE mode) inline
{
    switch (mode)
    {
        case RAINBOW:            // 彩虹
            initRainbowMode();
            break;

        case HACKER_MATRIX:      // 黑客帝国
        case STAR_SKY:           // 星空
        case RAIN:               // 雨
        case COLOURFUL_DREAM:    // 五彩梦幻
        case BUBBLE:             // 气泡
        case SNOW:               // 雪
    }

    renderFunction = renderFunctions[mode];
}

// FastLED 里的 XY 坐标转换行数的个人定制版
// 矩阵旋转公式，由新坐标换算为原坐标：
//   0: x = x', y = y'
//  90: x = y', y = height - x'
// 180: x = width - x', y = height - y'
// 270: x = width - y', y = x
uint16_t XY(uint8_t x, uint8_t y, uint8_t dir = CW0) inline
{
    switch (dir)
    {
    case CW0:
        return y * MATRIX_WIDTH + x;
    case CW90:
        return (MATRIX_HEIGHT - x - 1) * MATRIX_WIDTH + y;
    case CW180:
        return (MATRIX_WIDTH - y - 1) * MATRIX_WIDTH + MATRIX_HEIGHT - x - 1;
    case CW270:
        return x * MATRIX_WIDTH + MATRIX_WIDTH - y - 1;
    }
    return 0;
}

// ------------------------------------------------------------
// 彩虹模式
// ------------------------------------------------------------

// 彩虹模式，所有面都一样
void initRainbowMode()
{

}

unsigned int renderRainbow()
{

}

// ------------------------------------------------------------
// 黑客帝国模式
// ------------------------------------------------------------

//  黑客帝国效果模式初始化
// 所有侧面一样，上下面一样
void initHackMatrixMode()
{

}



}

// 黑客帝国效果，顶和底面
void renderHackMatrixTop(CRGB pLeds[])
{
}

// 渲染黑客帝国
unsigned int renderHackMatrix()
{

    return 100;
}

// ------------------------------------------------------------
// 星空模式
// ------------------------------------------------------------

//  星空效果初始化
// 每相邻的三个面不一样就可以了，因为一次只能同时看到三面
void initStarSkyMode()
{

}




// 渲染某一个面的星星
void renderStar(CRGB *pLeds)
{

}

// 星空效果
unsigned int renderStarSky()
{
}

// ------------------------------------------------------------
// 雨滴模式
// ------------------------------------------------------------

// 雨滴效果初始化
// 和黑客帝国效果一样
void initRainMode()
{

}

// 雨滴效果上下面的渲染
// 和黑客帝国效果一样
void renderRainTop()
{
}

// 雨滴效果侧面的渲染
// 和黑客帝国效果差不多
void renderRainSide()
{

}

// 雨滴效果渲染
// 和黑客帝国效果差不多
unsigned int renderRain()
{

    return 100;
}

// ------------------------------------------------------------
// 五彩梦幻模式
// ------------------------------------------------------------

// 五彩梦幻效果初始化
// 和星空效果一样
void initColourfulDreamMode()
{
    FastLEDControllers[UP_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
    FastLEDControllers[DOWN_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
    FastLEDControllers[LEFT_SIDE]->setLeds(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
    FastLEDControllers[RIGHT_SIDE]->setLeds(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
    FastLEDControllers[FRONT_SIDE]->setLeds(leds[FRONT_SIDE], NUM_LEDS_PER_MATRIX);
    FastLEDControllers[BACK_SIDE]->setLeds(leds[FRONT_SIDE], NUM_LEDS_PER_MATRIX);
}

// 根据几率随机显示一个彩色点
void randPutAColorDot(CRGB *pLeds)
{
    if (random8(100) > 70)
        return;

    randomDot(pLeds, CRGB::White);
}

// 五彩梦幻效果渲染
unsigned int renderColourfulDream()
{
    const uint8_t DISAPPEAR_RATE = 50;

    nscale8(leds[UP_SIDE], NUM_LEDS_PER_MATRIX, DISAPPEAR_RATE);
    nscale8(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX, DISAPPEAR_RATE);
    nscale8(leds[FRONT_SIDE], NUM_LEDS_PER_MATRIX, DISAPPEAR_RATE);

    randPutAColorDot(leds[UP_SIDE]);
    randPutAColorDot(leds[LEFT_SIDE]);
    randPutAColorDot(leds[FRONT_SIDE]);
    
    return random8(10);
}

// ------------------------------------------------------------
// 气泡模式
// ------------------------------------------------------------

unsigned int renderBubble()
{
    
}

// ------------------------------------------------------------
// 雪花模式
// ------------------------------------------------------------

unsigned int renderSnow()
{
    
}
