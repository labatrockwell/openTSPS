#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"
#include "simpleLogger.h"

class guiTypeLogger : public guiBaseObject{

     public:

        guiTypeLogger(){
            log = NULL;
            pct = 0;
        }

        //------------------------------------------------
        void setup(string loggerName, simpleLogger * logger, float loggerWidth, float loggerHeight){
            log = logger;
            name = loggerName;
            updateText();

            setDimensions(loggerWidth, loggerHeight);
            hitArea.width   = 20;
            outlineColor.selected = outlineColor.color;
        }

        //-----------------------------------------------.
        void updateGui(float x, float y, bool firstHit, bool isRelative){
            if( state == SG_STATE_SELECTED){
                pct = ( y - ( hitArea.y ) ) / hitArea.height;
                pct = ofClamp(pct, 0, 1);
            }

        }


        //-----------------------------------------------.
        void drawRecords(float x, float y, float width, float height){
            if( log == NULL)return;
            if( log->logs.size() == 0)return;

            ofPushStyle();
                float yPos = 13;
                int startPos = (float)(log->logs.size()-1) * (1.0-pct);
                for(int i = startPos; i >= 0; i--){
                    string str = log->logs[i].logStr;
                    if( str.length() * 8> width ){
                        int newLen = (float)width / 8;
                        //newLen = ofClamp(newLen, 1, 999999);
                        str = str.substr(0, newLen);
                    }

                    displayText.renderString(str, x, y + yPos);

                    yPos += 13.6;
                    if(yPos +14 >= height)break;
                }
            ofPopStyle();
        }

        //-----------------------------------------------.
        void render(){
            if (!enabled) return;
            ofPushStyle();{
                ofPushMatrix();{
                    //draw the background
                    ofFill();
                    ofSetColor(bgColor.getColor().r, bgColor.getColor().g, bgColor.getColor().b, bgColor.getColor().a);
                    ofRect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height);

                    ofSetColor(textColor.getColor().r, textColor.getColor().g, textColor.getColor().b, textColor.getColor().a);
                    guiBaseObject::renderText();

                    ofFill();
                    ofSetColor(fgColor.getColor().r, fgColor.getColor().g, fgColor.getColor().b, fgColor.getColor().a);
                    ofRect(hitArea.x, hitArea.y + (hitArea.height - 5) * pct, hitArea.width, 5);

                    ofNoFill();
                    ofSetColor(outlineColor.getColor().r, fgColor.getColor().g, fgColor.getColor().b, fgColor.getColor().a);
                    ofRect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height);
                    ofRect(hitArea.x , hitArea.y, hitArea.width, hitArea.height);

                    ofSetColor(textColor.getColor().r, textColor.getColor().g, textColor.getColor().b, textColor.getColor().a);
                    if(log != NULL)drawRecords(hitArea.x+hitArea.width + 5, hitArea.y, boundingBox.width-(hitArea.width + 5), boundingBox.height);
                } ofPopMatrix();
            }ofPopStyle();
        }

        float pct;
        simpleLogger * log;
};
