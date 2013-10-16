#pragma once

#include "ofConstants.h"

// ?
#ifndef MIN
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#endif

class simpleColor{

    public:

    //----------------------------------------------------------
    simpleColor(){
        r = 255.0f;
        g = 255.0f;
        b = 255.0f;
        a = 255.0f;
    }

    //----------------------------------------------------------
    void setColor(int _r, int _g, int _b, int _a){
        r = (float)_r; r = MAX(0,MIN(r,255.0f));
        g = (float)_g; g = MAX(0,MIN(g,255.0f));
        b = (float)_b; b = MAX(0,MIN(b,255.0f));
        a = (float)_a; a = MAX(0,MIN(a,255.0f));
    }

    //----------------------------------------------------------
    void setColor(int hexColor){
        int r = (hexColor >> 24)& 0xff;
        int g = (hexColor >> 16) & 0xff;
        int b = (hexColor >> 8) & 0xff;
        int a = (hexColor >> 0) & 0xff;

        setColor(r, g, b, a);
    }

    //----------------------------------------------------------
    simpleColor& getColor(){
        return *this;
    }

    //----------------------------------------------------------
    float * getColorF(){
        return color;
    }

    //------------------
    //------------------
    union  {
        struct {
            float r;
            float g;
            float b;
            float a;
        };
        float color[4];
    };

};


