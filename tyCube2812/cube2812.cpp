// cube2812.cpp
// Created by seesea 2021-07-07
// 2812 立方体的实现文件

#include <arduino.h>
#include "cube2812.h"
#include "rainbowMode.h"
#include "colourfulDreamMode.h"
#include "starSkyMode.h"
#include "rainMode.h"
#include "hackerMatrixMode.h"
#include "bubbleMode.h"
#include "energyCubeMode.h"
#include "snowMode.h"
#include "snakeGameMode.h"

// FastLED 的 LED 数据
// 内存不够用，只用三个面
CRGB leds[MATRIX_BUFFER_NUM][NUM_LEDS_PER_MATRIX];

// 各个 FastLED 控制器
CLEDController* FastLEDControllers[MATRIX_NUM];

/*
// 使用先创建一堆对象，再用指针数组的方式比较简单也不容易内存泄露，但是费内存
RainbowMode rainbowMode;
ColourfulDreamMode colourfulDreamMode;
StarSkyMode starSkyMode;

RenderMode *renderModes[] = { &rainbowMode, &colourfulDreamMode, &starSkyMode };
*/

RenderMode *renderMode = 0;
enum RENDER_MODE lastModeName = EMPTY;

// 初始化
void initCube2812()
{
    FastLED.setBrightness(INIT_BRIGHTNESS);

    // 内存不足，所以所有的侧面都用同样的显示
    FastLEDControllers[UP_SIDE]    = &FastLED.addLeds<CHIPSET, UP_PIN,    COLOR_ORDER>(leds[UP_SIDE],   NUM_LEDS_PER_MATRIX);
    FastLEDControllers[DOWN_SIDE]  = &FastLED.addLeds<CHIPSET, DOWN_PIN,  COLOR_ORDER>(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
    FastLEDControllers[LEFT_SIDE]  = &FastLED.addLeds<CHIPSET, LEFT_PIN,  COLOR_ORDER>(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
    FastLEDControllers[RIGHT_SIDE] = &FastLED.addLeds<CHIPSET, RIGHT_PIN, COLOR_ORDER>(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
    FastLEDControllers[FRONT_SIDE] = &FastLED.addLeds<CHIPSET, FRONT_PIN, COLOR_ORDER>(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);
    FastLEDControllers[BACK_SIDE]  = &FastLED.addLeds<CHIPSET, BACK_PIN,  COLOR_ORDER>(leds[LEFT_SIDE], NUM_LEDS_PER_MATRIX);

    // setRenderMode(RAINBOW);
    // setRenderMode(COLOURFUL_DREAM);
    // setRenderMode(STAR_SKY);
    // setRenderMode(HACKER_MATRIX);
    // setRenderMode(BUBBLE);
    // setRenderMode(ENERGY_CUBE);
    // setRenderMode(SNOW);
    setRenderMode(SNAKE_GAME);
    // setRenderMode(EMPTY);
}

// 渲染刷新函数
void updateCube2812()
{
    static unsigned long last_time = 0;
    static unsigned int  renderIntervalMs = 0;
    if (millis() - last_time > renderIntervalMs)
    {
        last_time = millis();

        if (renderMode == 0)
            return;

Serial.println("xxx");
        renderMode->input(random8(4) +1);
        // inputDir(random8(4) +1)
        renderIntervalMs = renderMode->getRenderInterval();
        renderMode->render();
        FastLED.show();
    }
}

void inputDir(uint8_t dir)
{
    renderMode->input(dir);
}

// 设置渲染模式
void setRenderMode(enum RENDER_MODE mode)
{
    // 使用先创建一堆对象，再用指针数组的方式比较简单也不容易内存泄露，但是费内存
    // renderMode = renderModes[mode];
    // renderMode->init();

    RenderMode *newMode = 0;

    switch (mode)
    {
        case RAINBOW:            // 彩虹
            newMode = new RainbowMode();
            break;

        case COLOURFUL_DREAM:   // 五彩梦幻
            newMode = new ColourfulDreamMode();
            break;
        
        case STAR_SKY:           // 星空
            newMode = new StarSkyMode();
            break;

        case HACKER_MATRIX:      // 黑客帝国
            newMode = new HackerMatrixMode();
            break;

        case RAIN:               // 雨
            newMode = new RainMode();
            break;

        case BUBBLE:             // 气泡
            newMode = new BubbleMode();
            break;

        case SNOW:               // 雪
            newMode = new SnowMode();
            break;

        case ENERGY_CUBE:        // 能量魔方
            newMode = new EnergyCubeMode();
            break;

        case SNAKE_GAME:        // 贪吃蛇游戏
            newMode = new SnakeGameMode();
            break;

        case EMPTY:
            if (renderMode != 0)
            {
                delete renderMode;
            }
            FastLED.clear();
            FastLED.showColor(CRGB::Black);

            return;

        default: 
            return;
    }

    if (newMode == 0)
        return;

    if (renderMode != 0)
        delete renderMode;

    FastLED.clear();
    lastModeName = mode;
    renderMode = newMode;
    renderMode->init();
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
void randomDot(CRGB pLeds[], const CRGB color)
{
    uint8_t x = random8(8);
    uint8_t y = random8(8);
    pLeds[XY(x, y)] = color;
}

// FastLED 里的 XY 坐标转换行数的个人定制版
// 矩阵旋转公式，由新坐标换算为原坐标：
//   0: x = x', y = y'
//  90: x = y', y = height - x'
// 180: x = width - x', y = height - y'
// 270: x = width - y', y = x
inline uint16_t XY(uint8_t x, uint8_t y, uint8_t dir = CW0)
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

void turnOnCube2812()
{
    setRenderMode(lastModeName);
}

void turnOffCube2812()
{
    setRenderMode(EMPTY);
}