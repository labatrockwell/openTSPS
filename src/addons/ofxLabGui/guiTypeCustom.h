#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"
#include "guiCustomImpl.h"

class guiTypeCustom : public guiBaseObject{

     public:

        guiTypeCustom(){
            custom = NULL;
            bgColor.setSelectedColor(0, 0, 0, 255);
            bgColor.setColor(0, 0, 0, 255);
        }

        //------------------------------------------------
        void setup(string customName, guiCustomImpl * customIn, float panelWidth, float panelHeight){
            custom = customIn;
            name = customName;
            updateText();

            setDimensions(panelWidth, panelHeight);
            if( custom != NULL )custom->boundingRect = hitArea;
        }

        //-----------------------------------------------
        virtual void saveSettings(){
            if( custom != NULL)custom->saveSettings();
        }

        //-----------------------------------------------
        virtual void saveSettings(string filename){
            if( custom != NULL)custom->saveSettings(filename);
        }

        //-----------------------------------------------
        virtual void loadSettings(string filename){
            if( custom != NULL)custom->loadSettings(filename);
        }

        //-----------------------------------------------
        virtual void reloadSettings(){
            if( custom != NULL)custom->reloadSettings();
        }

        //-----------------------------------------------
        virtual void update(){
            updateText();
            if( custom != NULL)custom->boundingRect = hitArea;
            if( custom != NULL)custom->update();
        }

        //-----------------------------------------------.
        void updateGui(float x, float y, bool firstHit, bool isRelative){
            if( custom == NULL || state != SG_STATE_SELECTED)return;

			float realX = x - hitArea.x;
			float realY = y - hitArea.y;

            custom->mouseIsRelative = isRelative;

            if(firstHit){
                custom->mousePressed(realX, realY);
            }else{
                custom->mouseDragged(realX, realY);
            }
        }

        //should  be called on mouse up
        //-------------------------------------------
        virtual void release(){
            if( custom == NULL)return;

            state = SG_STATE_NORMAL;
            setNormal();
            custom->mouseReleased();
         }

        //-----------------------------------------------.
        void render(){

            ofPushStyle();

                glPushMatrix();
                    guiBaseObject::renderText();

                    //draw the background
                    ofFill();
                    glColor4fv(bgColor.getColorF());
                    ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

                    ofDisableAlphaBlending();

                    ofSetColor(0xFFFFFF);
                    if( custom != NULL)custom->draw(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

                glPopMatrix();

            ofPopStyle();
        }

        guiCustomImpl * custom;
};
