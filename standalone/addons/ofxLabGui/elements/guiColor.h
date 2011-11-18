#pragma once

#include "simpleColor.h"

class guiColor{

    public:

        //------------------------------------------------
        guiColor(){
            bUseSelected  = false;
            bUseDisabled    = false;
            colorMode       = 0;
        }

        //------------------------------------------------
        void setColor(int r, int g, int b, int a){
            color.setColor(r, g, b, a);
        }

        //------------------------------------------------
        void setColor(int hexValue){
            color.setColor(hexValue);
        }

        //------------------------------------------------
        void setSelectedColor(int r, int g, int b, int a){
            selected.setColor(r, g, b, a);
            bUseSelected = true;
        }

        //------------------------------------------------
        void setSelectedColor(int hexValue){
            selected.setColor(hexValue);
            bUseSelected = true;
        }

        //------------------------------------------------
        void setDisabledColor(int r, int g, int b, int a){
            disabled.setColor(r, g, b, a);
            bUseDisabled = true;
        }

        //------------------------------------------------
        void setDisabledColor(int hexValue){
            disabled.setColor(hexValue);
            bUseDisabled = true;
        }

        //------------------------------------------------
        void setGuiColorMode(int whichColor){
            colorMode = whichColor;
        }

        //----------------------------------------------------------
        simpleColor& getColor(){
            if( colorMode == 1 && bUseSelected) return selected.getColor();
            else if( colorMode == 2 && bUseDisabled ) return disabled.getColor();
            else return color.getColor();
        }

        //----------------------------------------------------------
        float* getColorF(){
            if( colorMode == 1 && bUseSelected) return selected.getColorF();
            else if( colorMode == 2 && bUseDisabled) return disabled.getColorF();
            else return color.getColorF();
        }

        //----------------------------------------------------------
        float* getSelectedColorF(){
            return selected.getColorF();
        }

        //----------------------------------------------------------
        float* getNormalColorF(){
           return color.getColorF();
        }

        //---------------
        //---------------
        simpleColor color;
        simpleColor selected;
        simpleColor disabled;

        int colorMode;
        bool bUseDisabled;
        bool bUseSelected;

};

