#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

class guiType2DSlider : public guiBaseObject{

    public:

        guiType2DSlider(){
            knobSize = 6.0;
        }

        //------------------------------------------------
        void setup(string sliderName, float defaultVal1, float min1, float max1, float defaultVal2, float min2, float max2){
            value.addValue(defaultVal1, min1, max1);
            value.addValue(defaultVal2, min2, max2);
            name = sliderName;
            updateText();
        }

        //-----------------------------------------------.
        void updateGui(float x, float y, bool firstHit, bool isRelative){
            if( state == SG_STATE_SELECTED){
                float pct1 = ( x - ( hitArea.x ) ) / hitArea.width;
                value.setValueAsPct( pct1, 0);
                float pct2 = ( y - ( hitArea.y ) ) / hitArea.height;
                value.setValueAsPct( pct2, 1);
                updateText();
            }
        }

        //-----------------------------------------------
        void setKnobSize(float _knobSize){
            knobSize = _knobSize;
        }

        //-----------------------------------------------.
        void render(){
            ofPushStyle();

                glPushMatrix();
                //glTranslatef(boundingBox.x, boundingBox.y, 0);
                    guiBaseObject::renderText();

                    //draw the background
                    ofFill();
                    glColor4fv(bgColor.getColorF());
                    ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

                    //draw the foreground

                    float boxWidth  = knobSize;
                    float boxHeight = knobSize;

                    float bx = hitArea.x + (boxWidth * 0.5) + ( value.getPct(0) * (hitArea.width - (boxWidth)) );
                    float by = hitArea.y + (boxHeight * 0.5) + ( value.getPct(1) * (hitArea.height - (boxHeight)) );

                    glColor4fv(fgColor.getColorF());

                    ofSetRectMode(OF_RECTMODE_CENTER);
                    ofRect(bx, by, boxWidth, boxHeight);
                    ofSetRectMode(OF_RECTMODE_CORNER);

                    //draw the outline
                    ofNoFill();
                    glColor4fv(outlineColor.getColorF());
                    ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

                glPopMatrix();

            ofPopStyle();
        }

        float knobSize;

};
