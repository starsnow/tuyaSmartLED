// snakeGameMode.h
// seesea 2021-07-09
// 贪吃蛇游戏模式

#ifndef _SNOW_MODE_H_
#define _SNOW_MODE_H_

#include "cube2812.h"
#include "modeBaseClass.h"

// FastLED 的 LED 数据
// 内存不够用，只用三个面
extern CRGB leds[MATRIX_BUFFER_NUM][NUM_LEDS_PER_MATRIX];

// 各个 FastLED 控制器
extern CLEDController* FastLEDControllers[MATRIX_BUFFER_NUM];

class SnakeGameMode : public RenderMode
{
private:
    static const uint8_t MAX_SNAKE_LENGTH = 30;
    static const uint8_t MAX_FOOD_NUM = 1;
    
    static const CRGB foodColors[] = {CRGB::Magenta, CRGB::Lime, CRGB::HotPink};
    const CRGB headColor = CRGB::Yellow;
    const CRGB bodyColor = CRGB::SandyBrown;

    unsigned int renderInterval;
    CRGB *pLedsFront;

    Point snake[MAX_SNAKE_LENGTH];
    Point food[MAX_FOOD_NUM];

public:
    SnakeGameMode()
    {
        renderInterval = 500;
        thawInterval = renderInterval * 8;
    }

    String getName() 
    {
        return F("snake game mode");
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

        pLedsFront = leds[UP_SIDE];
    }

    void renderSnake(CRGB *pLeds, uint8_t dir)
    {

    }

    void render() 
    {
    }
};

#endif