// cube2812.cpp
// Created by seesea 2021-07-07
// 2812 立方体的实现文件

#include "cube2812.h"

// 当前和前一次的渲染函数指针
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

// 彩虹模式，所有面都一样
void initRainbowMode()
{
    for (int i = 0; i < MATRIX_NUM; ++i)
    {
        FastLEDControllers[i]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
    }
}

unsigned int renderRainbow()
{
    static uint8_t hue = -1;
    fill_rainbow(leds[UP_SIDE], NUM_LEDS_PER_MATRIX, hue++, 10);
    return 10;
}

//  黑客帝国效果模式，所有侧面一样，上下面一样
void initRainbowMode()
{
    for (int i = 0; i < MATRIX_NUM; ++i)
    {
        FastLEDControllers[i]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
    }
}

// 黑客帝国效果
void hackMatrix(CRGB pLeds[], uint8_t dir)
{
    static Point head[10];
    CRGB *pLed;

    uint8_t i;

    for (uint8_t y = 0; y < 8; ++y)
    {
        for (uint8_t x = 0; x < 8; ++x)
        {
            // 淡化
            pLed = &pLeds[XY(x, y, dir)];
            pLed->nscale8(100);

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

        pLeds[XY(head[i].x, head[i].y, dir)] = CRGB::Green;

        head[i].y++;
        if (head[i].y >= MATRIX_HEIGHT)
        {
            head[i].x = -1;
            continue;
        }
    }
}

unsigned int renderHackMatrix()
{
    hackMatrix(leds[0], CW0);
    return 100;
}
