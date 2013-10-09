#pragma once


#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

class guiTypeSlider : public guiBaseObject{

    public:
		
        //------------------------------------------------
        void setup(string sliderName, float defaultVal, float min, float max){
            value.addValue(defaultVal, min, max);
            name = sliderName;
        }

        //-----------------------------------------------.
        void updateGui(float x, float y, bool firstHit, bool isRelative = false){
            if( state == SG_STATE_SELECTED){

                if( !isRelative ){
                    float pct = ( x - ( hitArea.x ) ) / hitArea.width;
                    value.setValueAsPct( pct );
                }else if( !firstHit ){
                    float pct = value.getPct();
                    pct += (x * 0.02) / hitArea.width;
                    value.setValueAsPct( pct );
                }
            }
        }

        //-----------------------------------------------.
        void render(){
            if (!enabled) return;
            ofPushStyle();
                glPushMatrix();
                    guiBaseObject::renderText();

                    //draw the background
                    ofFill();
                    ofSetColor(bgColor.getColor().r, bgColor.getColor().g, bgColor.getColor().b, bgColor.getColor().a);
                    ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

                    //draw the foreground
                    ofSetColor(fgColor.getColor().r, fgColor.getColor().g, fgColor.getColor().b, fgColor.getColor().a);
                    ofRect(hitArea.x, hitArea.y, hitArea.width * value.getPct(), hitArea.height);

                    //draw the outline
                    ofNoFill();
                    ofSetColor(outlineColor.getColor().r, outlineColor.getColor().g, outlineColor.getColor().b, outlineColor.getColor().a);
                    ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
                glPopMatrix();
            ofPopStyle();
        }


};
