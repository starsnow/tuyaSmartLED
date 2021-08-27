// cube2812.h
// Created by seesea 2021-07-07
// 2812 立方体的头文件

#ifndef _CUBE2812_H_
#define _CUBE2812_H_

#include <arduino.h>
#include <fastLED.h>

// 点阵屏的相应参数
#define NUM_LEDS_PER_MATRIX 64  // 点阵 LED 数量
#define MATRIX_NUM          6   // 点阵屏数量
#define MATRIX_BUFFER_NUM   2   // 点阵屏的 LED 缓冲区数量（因内存不足，不同的点阵屏用同一个缓冲区显示同样的内容）
#define MATRIX_WIDTH        8   // 点阵屏的宽（LED 数）
#define MATRIX_HEIGHT       8   // 点阵屏的高（LED 数）

// FastLED 相关参数
#define INIT_BRIGHTNESS     50
#define CHIPSET             WS2812B
#define COLOR_ORDER         GRB

// 每个面的 2812 数据引脚
#ifdef ARDUINO_AVR_PRO
// prop mini 的引脚
enum DATA_PIN
{
    UP_PIN      = 2,
    DOWN_PIN    = 4,
    LEFT_PIN    = 5,
    RIGHT_PIN   = 6,
    FRONT_PIN   = 7,
    BACK_PIN    = 8
};
#else
enum DATA_PIN
{
    UP_PIN      = A0,
    DOWN_PIN    = A1,
    LEFT_PIN    = A2,
    RIGHT_PIN   = A3,
    FRONT_PIN   = A4,
    BACK_PIN    = A5
 };
 #endif
// 立方体每个面
// 如果因内存不足的话，按优先级使用 上 左 前 三个面，使三个相邻的面不同样，在观看的时候也就达到效果了
enum CUBE_SIDE
{
    UP_SIDE = 0,
    LEFT_SIDE,
    FRONT_SIDE,
    DOWN_SIDE,
    RIGHT_SIDE,
    BACK_SIDE
};

// 点阵放置的方向
enum MATRIX_DIR
{
    CW0 = 0,
    CW90,
    CW180,
    CW270
};

// 输入
enum Input 
{
    NONE = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// 点阵显示模式
enum RENDER_MODE 
{
    EMPTY,              // 什么也没有
    RAINBOW,            // 彩虹
    COLOURFUL_DREAM,    // 五彩梦幻
    STAR_SKY,           // 星空
    HACKER_MATRIX,      // 黑客帝国
    RAIN,               // 雨
    BUBBLE,             // 气泡
    SNOW,               // 雪
    ENERGY_CUBE,        // 能量魔方
    SNAKE_GAME          // 贪吃蛇游戏
};

// 坐标点
class Point
{
public:
    int8_t x, y;

    Point(int8_t _x = -1, int8_t _y = -1)
    {
        x = _x;
        y = _y;
    };

    bool operator== (const Point &p)
    {
        return (p.x == x) && (p.y == y);
    }
};

void initCube2812();
void updateCube2812();
void turnOnCube2812();
void turnOffCube2812();
void randomDot(CRGB pLeds[], const CRGB color);
void setRenderMode(enum RENDER_MODE mode);
void inputDir(uint8_t dir);
inline uint16_t XY(uint8_t x, uint8_t y, uint8_t dir = CW0);

#endif
