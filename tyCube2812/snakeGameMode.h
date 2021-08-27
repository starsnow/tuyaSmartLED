// snakeGameMode.h
// seesea 2021-07-09
// 贪吃蛇游戏模式

#ifndef _SNAKE_GAME_MODE_H_
#define _SNAKE_GAME_MODE_H_

#include "cube2812.h"
#include "modeBaseClass.h"
#include <arduino.h>

// FastLED 的 LED 数据
// 内存不够用，只用三个面
extern CRGB leds[MATRIX_BUFFER_NUM][NUM_LEDS_PER_MATRIX];

// 各个 FastLED 控制器
extern CLEDController* FastLEDControllers[MATRIX_NUM];

class Food
{
public:    
    Point pos;
    uint8_t type;
};

class SnakeGameMode : public RenderMode
{
private:
    static const uint8_t MAX_SNAKE_LENGTH = 30;
    static const uint8_t MAX_FOOD_NUM = 2;
    
    const CRGB foodColors[3] = {CRGB::MediumVioletRed, CRGB::DarkViolet, CRGB::HotPink};
    const CRGB HEAD_COLOR = CRGB::Green;
    const CRGB BODY_COLOR = CRGB::SeaGreen;

    unsigned int renderInterval;
    uint8_t foodColorNum;
    CRGB *pLedsFront;

    Point snake[MAX_SNAKE_LENGTH];
    uint8_t snakeHeadIndex;
    uint8_t snakeLength;
    uint8_t moveDir;
    bool    dead;

    Food foods[MAX_FOOD_NUM];


public:
    SnakeGameMode()
    {
        renderInterval = 300;
        foodColorNum = sizeof(foodColors) / sizeof(foodColors[0]);
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
        random16_set_seed(millis() + analogRead(A0));
        
        FastLEDControllers[UP_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
        // FastLEDControllers[DOWN_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
        // FastLEDControllers[LEFT_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
        // FastLEDControllers[RIGHT_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
        // FastLEDControllers[FRONT_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);
        // FastLEDControllers[BACK_SIDE]->setLeds(leds[UP_SIDE], NUM_LEDS_PER_MATRIX);

        pLedsFront = leds[UP_SIDE];
        
        dead = false;
        moveDir = NONE;
        snakeHeadIndex = 0;
        snakeLength = 3;
        snake[0].x = snake[1].x = snake[2].x= 3;
        snake[0].y = 3;
        snake[1].y = 4;
        snake[2].y = 5;

        genNewFoods(pLedsFront, CW0, MAX_FOOD_NUM);
    }

    void genNewFoods(CRGB *pLeds, uint8_t dir, uint8_t newFoodNum = 1)
    {
        uint8_t i, j;
        bool ok = false;
        uint8_t tryTimes;

        for (i = 0; i < MAX_FOOD_NUM; ++i)
        {
            if (foods[i].pos.x != -1)
            {
                continue;
            }

            ok = false;
            tryTimes = 30;
            while (tryTimes-- != 0)
            {
                foods[i].pos.x = random8(8);
                foods[i].pos.y = random8(8);
                for (j = 0; j < snakeLength; ++j)
                {
                    if (snake[j].x == -1)
                        continue;
                        
                    if (foods[i].pos == snake[j])
                    {
                        break;
                    }
                }

                if (j == snakeLength)
                {
                    ok = true;
                    break;
                }
            }
            
            if (! ok)
            {
                foods[i].pos.x = -1;
                continue;
            }

            foods[i].type = random8(foodColorNum);
            pLeds[XY(foods[i].pos.x, foods[i].pos.y, dir)] = foodColors[foods[i].type];

            --newFoodNum;
            if (newFoodNum == 0)
            {
                break;
            }
        }
    }

    void renderSnake(CRGB *pLeds, uint8_t dir)
    {
        uint8_t i;

        // for (i = snakeHeadIndex; i != (snakeHeadIndex + MAX_SNAKE_LENGTH - 1) % MAX_SNAKE_LENGTH; i = (i + 1 + MAX_SNAKE_LENGTH) % MAX_SNAKE_LENGTH)
        for (i = 0; i < snakeLength; ++i)
        {
            if (snake[i].x == -1)
            {
                continue;
            }

            pLeds[XY(snake[i].x, snake[i].y, dir)] = (i == snakeHeadIndex ? HEAD_COLOR : BODY_COLOR);
        }
    }

    void renderFoods(CRGB *pLeds, uint8_t dir)
    {
        uint8_t i;

        for (i = 0; i < MAX_FOOD_NUM; ++i)
        {
            if (foods[i].pos.x == -1)
                continue;

            pLeds[XY(foods[i].pos.x, foods[i].pos.y, dir)] = foodColors[foods[i].type];
        }
    }

    void renderSnakeGameMap(CRGB *pLeds, uint8_t dir)
    {
        FastLED.clear();
        renderSnake(pLeds, dir);
        renderFoods(pLeds, dir);
    }

    void moveSnake()
    {
        if (moveDir == NONE)
            return;

        uint8_t newHeadIndex = (snakeHeadIndex + snakeLength - 1) % snakeLength;
        snake[newHeadIndex] = snake[snakeHeadIndex];
        snakeHeadIndex = newHeadIndex;

        switch (moveDir)
        {
            case NONE:  return;
            case UP:    snake[snakeHeadIndex].y = (snake[snakeHeadIndex].y + MATRIX_WIDTH - 1) % MATRIX_WIDTH; break;
            case DOWN:  snake[snakeHeadIndex].y = (snake[snakeHeadIndex].y + MATRIX_WIDTH + 1) % MATRIX_WIDTH; break;
            case LEFT:  snake[snakeHeadIndex].x = (snake[snakeHeadIndex].x + MATRIX_HEIGHT - 1) % MATRIX_HEIGHT; break;
            case RIGHT: snake[snakeHeadIndex].x = (snake[snakeHeadIndex].x + MATRIX_HEIGHT + 1) % MATRIX_HEIGHT; break;
        }
    }

    void growSnake()
    {
        if (snakeLength < MAX_SNAKE_LENGTH)
        {
            ++snakeLength;
        }    
    }

    bool eatFood()
    {
        for (uint8_t i = 0; i < MAX_FOOD_NUM; ++i)
        {
            if (snake[snakeHeadIndex] == foods[i].pos)
            {
                foods[i].pos.x = -1;
                return true;
            }           
        }

        return false;
    }

    bool checkHit()
    {
        for (uint8_t i = 0; i < snakeLength; ++i)
        {
            if (i == snakeHeadIndex)
                continue;

            if (snake[i] == snake[snakeHeadIndex])
                return true;
        }

        return false;
    }

    void die()
    {
        dead = true;
    }

    bool isDead()
    {
        return dead;
    }

    void input(uint8_t d)
    {
        if (d == UP    && moveDir == DOWN ||
            d == DOWN  && moveDir == UP ||
            d == LEFT  && moveDir == RIGHT ||
            d == RIGHT && moveDir == LEFT)
        {
            return;
        }
        moveDir = d;
    }

    void render() 
    {
        static bool blinkDeadSnake = false;

        if (isDead())
        {
            blinkDeadSnake = !blinkDeadSnake;
            FastLED.clear();
            renderFoods(pLedsFront, CW0);
            if (blinkDeadSnake)
            {
                renderSnake(pLedsFront, CW0);
            }

            return;
        }

        moveSnake();
        if (checkHit())
        {
            die();
            return;
        }

        if (eatFood())
        {
            growSnake();
            genNewFoods(pLedsFront, CW0);
        }

        renderSnakeGameMap(pLedsFront, CW0);
    }
};

#endif