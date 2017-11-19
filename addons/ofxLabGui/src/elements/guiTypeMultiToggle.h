#pragma once


#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

#define boxSpacing		2
#define boxSize			14

class guiTypeMultiToggle : public guiBaseObject{

    public:

        //------------------------------------------------
        void setup(string multiToggleName, int defaultBox, vector <string> boxNames){
            bNames = boxNames;
            value.addValue( (int)defaultBox, 0, bNames.size()-1);
            name = multiToggleName;

            float lineHeight = displayText.getTextSingleLineHeight() + boxSize+boxSpacing;
            for(int i = 0; i < bNames.size(); i++){
                float lineWidth = boxSpacing + boxSize + displayText.stringWidth(bNames[i]);

                if( lineWidth > hitArea.width ){
                    hitArea.width       += lineWidth-hitArea.width;
                    boundingBox.width   += lineWidth-hitArea.width;
                }

                lineHeight += displayText.getTextSingleLineHeight();
            }

            if(lineHeight > boundingBox.height){
                hitArea.height      += lineHeight-boundingBox.height  - (displayText.getTextSingleLineHeight() + boxSize+boxSpacing);
                boundingBox.height  += lineHeight-boundingBox.height;
            }

        }
    
        //-----------------------------------------------
        void updateBoundingBox(){
            if(bShowText){
                //we need to update out hitArea because the text will have moved the gui down
                hitArea.y = (int) (boundingBox.y + displayText.getTextSingleLineHeight() + titleSpacing);
                
                float lineHeight = displayText.getTextSingleLineHeight() + boxSize+boxSpacing;
                for(int i = 0; i < bNames.size(); i++){
                    float lineWidth = boxSpacing + boxSize + displayText.stringWidth(bNames[i]);
                    
                    if( lineWidth > hitArea.width ){
                        hitArea.width       += lineWidth-hitArea.width;
                        boundingBox.width   += lineWidth-hitArea.width;
                    }
                    
                    lineHeight += displayText.getTextSingleLineHeight();
                }
                if(lineHeight > boundingBox.height){
                    hitArea.height      += lineHeight-boundingBox.height - (displayText.getTextSingleLineHeight() + boxSize+boxSpacing);
                    boundingBox.height  += lineHeight-boundingBox.height;
                }
            }else{
                //we need to update out hitArea because the text will have moved the gui down
                hitArea.y = boundingBox.y;
                boundingBox.height = hitArea.height;
                float lineHeight = 0;
                for(int i = 0; i < bNames.size(); i++){
                    float lineWidth = boxSpacing + boxSize + displayText.stringWidth(bNames[i]);
                    
                    if( lineWidth > hitArea.width ){
                        hitArea.width       += lineWidth-hitArea.width;
                        boundingBox.width   += lineWidth-hitArea.width;
                    }
                    
                    lineHeight += displayText.getTextSingleLineHeight();
                }
                if(lineHeight > boundingBox.height){
                    hitArea.height      += lineHeight-boundingBox.height;
                    boundingBox.height  += lineHeight-boundingBox.height;
                }
            }
        }

        //-----------------------------------------------
        void updateGui(float x, float y, bool firstHit, bool isRelative){
            if(!firstHit)return;

            if( state == SG_STATE_SELECTED){
                float relX = x - hitArea.x;
                float relY = y - hitArea.y;

                for(int i = 0; i < bNames.size(); i++){
                    ofRectangle tmpRect(0, i * (boxSize+boxSpacing), boxSize, boxSize);
                    if( isInsideRect(relX, relY, tmpRect) ){
                        value.setValue(i, 0);
                        break;
                    }
                }
            }


        }

        //-----------------------------------------------.
        void render(){
            if (!enabled) return;
            ofPushStyle(); {
            guiBaseObject::renderText();

                //draw the background
                ofFill();
                ofSetColor(bgColor.getColor().r, bgColor.getColor().g, bgColor.getColor().b, bgColor.getColor().a);
                ofDrawRectangle(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

                for(int i = 0; i < bNames.size(); i++){
                    float bx = hitArea.x + 0;
                    float by = hitArea.y + i * (boxSize+boxSpacing);

                    if(value.getValueI() == i){
                        ofFill();
                    }else{
                        ofNoFill();
                    }

                    ofSetColor(fgColor.getColor().r, fgColor.getColor().g, fgColor.getColor().b, fgColor.getColor().a);
                    ofDrawRectangle(bx, by, boxSize, boxSize);

                    ofNoFill();
                    ofSetColor(outlineColor.getColor().r, outlineColor.getColor().g, outlineColor.getColor().b, outlineColor.getColor().a);
                    ofDrawRectangle(bx, by, boxSize, boxSize);

                    ofSetColor(textColor.getColor().r, textColor.getColor().g, textColor.getColor().b, textColor.getColor().a);
                    displayText.renderString(bNames[i], bx + boxSize + boxSpacing, by + boxSize -2);
                }

            } ofPopStyle();
        }

    vector <string> bNames;
};
