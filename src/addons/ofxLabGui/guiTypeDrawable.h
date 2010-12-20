#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"


class drawableStacker : public ofBaseDraws{
    public:

    void addDrawer(ofBaseDraws * drawer){
        drawers.push_back(drawer);
    }

    void setWidth(float w){
        width = w;
    }

    void setHeight(float h){
        height = h;
    }

    float getWidth(){
        return width;
    }

    float getHeight(){
        return height;
    }

    void draw(float x, float y, float w, float h){
        ofSetColor(0xFFFFFF);
        for(int i = 0; i < drawers.size(); i++){
            drawers[i]->draw(x, y, w, h);
        }
    }

    void draw(float x, float y){
        ofSetColor(0xFFFFFF);
        for(int i = 0; i < drawers.size(); i++){
            drawers[i]->draw(x, y, width, height);
        }
    }

    vector <ofBaseDraws *> drawers;
    float width;
    float height;
};

class guiTypeDrawable : public guiBaseObject{

     public:

        guiTypeDrawable(){
            vid = NULL;
            bgColor.setSelectedColor(0, 0, 0, 255);
            bgColor.setColor(0, 0, 0, 255);
        }

        //------------------------------------------------
        void setup(string videoName, ofBaseDraws * vidIn, float videoWidth, float videoHeight){
            vid = vidIn;
            name = videoName;
            updateText();

            setDimensions(videoWidth, videoHeight);

        }

        //-----------------------------------------------.
        void updateGui(float x, float y, bool firstHit, bool isRelative){

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

                    ofDisableAlphaBlending();

                    ofSetColor(0xFFFFFF);
                    vid->draw(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

                glPopMatrix();

            ofPopStyle();
        }

        ofBaseDraws * vid;
};
