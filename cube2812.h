// cube2812.h
// Created by seesea 2021-07-07
// 2812 立方体的头文件

#ifndef _CUBE2812_H_
#define _CUBE2812_H_

// 点阵屏的相应参数
#define NUM_LEDS_PER_MATRIX 64
#define MATRIX_NUM          6
#define MATRIX_WIDTH        8
#define MATRIX_HEIGHT       8

// FastLED 相关参数
#define INIT_BRIGHTNESS     5
#define CHIPSET             WS2812B
#define COLOR_ORDER         GRB

// 每个面的 2812 数据引脚
enum DATA_PIN
{
    UP_PIN      = 2,
    DOWN_PIN    = 4,
    LEFT_PIN    = 5,
    RIGHT_PIN   = 6,
    FRONT_PIN   = 7,
    BACK_PIN    = 8
};

// 立方体每个面
enum CUBE_SIDE
{
    UP_SIDE = 0,
    DOWN_SIDE,
    LEFT_SIDE,
    RIGHT_SIDE,
    FRONT_SIDE,
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

// 点阵显示模式
enum RENDER_MODE 
{
    RAINBOW,            // 彩虹
    HACKER_MATRIX,      // 黑客帝国
    STAR_SKY,           // 星空
    RAIN,               // 雨
    COLOURFUL_DREAM,    // 五彩梦幻
    BUBBLE,             // 气泡
    SNOW                // 雪
};

// 坐标点
class Point
{
public:
    Point(int8_t _x = -1, int8_t _y = -1)
    {
        x = _x;
        y = _y;
    }

    int8_t x, y;
};

// FastLED 的 LED 数据
CRGB leds[3][NUM_LEDS_PER_MATRIX];

void initCube2812();
void updateCube2812();

#endif