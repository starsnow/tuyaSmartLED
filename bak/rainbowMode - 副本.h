// rainbowMode.h
// seesea 2021-07-09
// 雨滴模式

#ifndef _RAINBOW_MODE_H_
#define _RAINBOW_MODE_H_

#include "cube2812.h"

class rainbowMode : public renderMode
{
private:
    unsigned int renderInterval;

public:
    rainbowMode()
    {
        renderInterval = 100;
    }

    String getName() 
    {
        return " mode";
    }

    unsignd long renderInterval()
    {
        return renderInterval;
    }

    void init()
    {

    }

    void render() 
    {
        
    }
};

#endif