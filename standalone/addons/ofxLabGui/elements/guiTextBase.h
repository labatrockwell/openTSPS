#pragma once



#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

class guiTextBase{

    public:
		
        //-------------------------------------------
        guiTextBase(){
            textString = "";
            bRealFont  = false;
            fontSize   = 13;
            numNewLines = 0;

            ourFont = NULL;
        }

        //we should also have an option to load a font once
        //then just pass a reference to the ofTTF font object
        //--------------------------------------------
        virtual void setFont(ofTrueTypeFont * fontPtr){
            if( fontPtr != NULL && fontPtr->isLoaded() ){
                ourFont = fontPtr;
                bRealFont = true;
            }else bRealFont = false;
        }

        //--------------------------------------------
        virtual float getTextWidth(){
            float textWidth = 0;

            if(bRealFont)textWidth = ourFont->stringWidth(textString);
            else textWidth = 0.615 * (float)fontSize * textString.size() ;

            return  textWidth;
        }

        //--------------------------------------------
        virtual float getTextHeight(){
            float textHeight = 0;

            if(bRealFont)textHeight = ourFont->stringHeight(textString);
            else  textHeight = fontSize + fontSize * numNewLines;

            return textHeight;
        }

        //--------------------------------------------
        virtual float getTextSingleLineHeight(){
            float textHeight = 0;

            if(bRealFont)textHeight = ourFont->stringHeight("A");
            else  textHeight = fontSize;

            return textHeight;
        }

        //-------------------------------------------
        virtual void setText(string textStr){
            textString = textStr;

            numNewLines = 0;
            for(int i = 0; i < textString.size(); i++){
                if( textString[i] == '\n' )numNewLines++;
            }

        }

        //--------------------------------------------
        virtual void renderText(float x, float y){

            if(bRealFont){
                ourFont->drawString(textString, x, y);
            }
            else ofDrawBitmapString(textString, x, y);
        }

        //--------------------------------------------
        virtual void renderString(string textToRender, float x, float y){

            if(bRealFont){
                ourFont->drawString(textToRender, x, y);
            }
            else ofDrawBitmapString(textToRender, x, y);
        }

        //--------------------------------------------
        virtual float stringHeight(string textToRender){
            float textHeight = 0;

            if(bRealFont)textHeight = ourFont->stringHeight(textToRender);
            else{
                int numLines = 0;
                for(int i = 0; i < textToRender.size(); i++){
                    if( textToRender[i] == '\n' )numLines++;
                }
                textHeight = fontSize + fontSize * numLines;
            }
            return textHeight;
        }

        //--------------------------------------------
        virtual float stringWidth(string textToRender){

            float textWidth = 0;

            if(bRealFont)textWidth = ourFont->stringWidth(textToRender);
            else textWidth = 0.615 * (float)fontSize * textToRender.size() ;

            return  textWidth;
        }


        //--------------------------------------------
        ofTrueTypeFont * ourFont;

        string textString;

        int numNewLines;
        int fontSize;
        bool bRealFont;
        bool bShowText;
};
