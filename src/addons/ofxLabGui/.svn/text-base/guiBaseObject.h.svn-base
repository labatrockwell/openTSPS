#pragma once

#include "simpleColor.h"
#include "guiColor.h"
#include "guiValue.h"
#include "guiTextBase.h"

#define SG_STATE_NORMAL 0
#define SG_STATE_SELECTED 1

#define SG_TYPE_FLOAT 0
#define SG_TYPE_INT 1
#define SG_TYPE_BOOL 2
#define SG_TYPE_STRING 3

//layout vars
#ifndef borderWidth
#define borderWidth  10.f
#endif

static bool isInsideRect(float x, float y, ofRectangle rect){
    return ( x >= rect.x && x <= rect.x + rect.width && y >= rect.y && y <= rect.y + rect.height );
}

class guiBaseObject{

    public:
        guiBaseObject(){
            readOnly    = false;
            bShowText   = true;
            locked      = false;
            dataType    = SG_TYPE_FLOAT;
			bTextEnterMode = false;

            numDecimalPlaces    = 2;
            storedTextWidth     = 0;

            fgColor.setColor(180, 180, 180, 255);
            fgColor.setSelectedColor(100, 140, 220, 255);

            outlineColor.setColor(255, 255, 255, 180);
            outlineColor.setSelectedColor(255, 255, 255, 255);

            bgColor.setSelectedColor(12, 12, 12, 200);
            bgColor.setColor(12, 12, 12, 200);

            //these need to be setable at some point
            fontSize     = 11;
            titleSpacing = 10;
			
			//group stuff
			numGroups = currentGroup = 0;

        }

        //------------------------------------------
        virtual void setXmlName(string _xmlName){
            xmlName = _xmlName;
        }

        //--------------------------------------------
        virtual void setFont(ofTrueTypeFont * fontPtr){
            displayText.setFont(fontPtr);
        }

        //should be called on mousedown
        //-------------------------------------------
        virtual bool checkHit(float x, float y, bool isRelative){
            if(readOnly)return false;
            if( isInsideRect(x, y, hitArea) ){
                state = SG_STATE_SELECTED;
				if (dataType == SG_TYPE_STRING) bTextEnterMode = true;
                setSelected();
                updateGui(x, y, true, isRelative);

                float offsetX = x - hitArea.x;
                float offsetY = y - hitArea.y;

                for(int i = 0; i < children.size(); i++){
                    children[i]->checkHit(offsetX, offsetY, isRelative);
                }
                return true;
            }
			
			//if (dataType == SG_TYPE_STRING) bTextEnterMode = true;
            return false;
        }
	
		
		//should be called on keypressed
		//-------------------------------------------
		virtual void keyPressed(int key){
			//do stuff
		}

        //this is the equivilant of mouse moved if the gui element has been selected
        //this is empty as it really should be specified by
        //the extending class (gui element).
        //------------------------------------------------
        virtual void updateGui(float x, float y, bool firstHit, bool isRelative){

        }

        //should  be called on mouse up
        //-------------------------------------------
        virtual void release(){
            state = SG_STATE_NORMAL;
            setNormal();
            for(int i = 0; i < children.size(); i++){
                children[i]->release();
            }
         }
		
		//called to force override children with persistent editing state
		// e.g. text fields
		//-------------------------------------------
		virtual void resetChildren(){
			for(int i = 0; i < children.size(); i++){
				children[i]->bTextEnterMode = false;
				children[i]->setNormal();
				children[i]->resetChildren();
			}
		}
	
        //these are here for the custom control types
        //we notify all elements about these actions
        virtual void saveSettings(string filename){}
        virtual void reloadSettings(){}
        virtual void saveSettings(){}
        virtual void loadSettings(string filename){}

        virtual void lock(){
            locked = true;
        }

        virtual void unlock(){
            locked = false;
        }

        bool isLocked(){
            return locked;
        }

        //------------------------------------------------
        virtual void setShowText(bool showText){
            bShowText = showText;
			updateBoundingBox();
        }

        //-----------------------------------------------
        virtual void setTypeInt(){
            dataType = SG_TYPE_INT;
        }

        //-----------------------------------------------
        virtual void setTypeFloat(){
            dataType = SG_TYPE_FLOAT;
        }

        //-----------------------------------------------
        virtual void setTypeBool(){
            dataType = SG_TYPE_BOOL;
        }

		//-----------------------------------------------
		virtual void setTypeString(){
			dataType = SG_TYPE_STRING;
		}
	
	
        //-----------------------------------------------
        virtual void setPosition(float x, float y){
            boundingBox.x = x;
            boundingBox.y = y;
            hitArea.x     = x;
            hitArea.y     = y;
        }

        //------------------------------------------------
        virtual void setDimensions(float width, float height){
            hitArea.width       = width;
            hitArea.height      = height;
            boundingBox.width   = width;
            boundingBox.height  = height;
        }

        //-----------------------------------------------
        virtual float getPosX(){
            return boundingBox.x;
        }

        //------------------------------------------------
        virtual float getPosY(){
            return boundingBox.y;
        }

        //------------------------------------------------
        virtual float getWidth(){
            return boundingBox.width;

        }

        //-----------------------------------------------
        virtual float getHeight(){
             return boundingBox.height;
        }

        //-----------------------------------------------
        virtual void update(){
            updateText();
        }

        //-----------------------------------------------
        virtual void checkPrescison(){
            //here we check the range of floating point values and change number of decimal places based on range
            float range =fabs( value.getMax() - value.getMin() );
            if( range < 0.001 ){
                numDecimalPlaces  = 6;
            }else if( range < 0.01){
                numDecimalPlaces = 5;
            }else if( range < 0.1 ){
                numDecimalPlaces = 4;
            }else if( range < 1){
                numDecimalPlaces = 3;
            }else if( range < 10){
                numDecimalPlaces = 2;
            }else{
                numDecimalPlaces = 1;
            }
        }

//        //-----------------------------------------------
//        virtual void setIsRelative( bool bIsRelative ){
//            isRelative = bIsRelative;
//        }

        //every time we update the value of our text
        //-----------------------------------------------
        virtual void updateText(){

            drawStr = name;
            for(int i = 0; i < value.getNumValues(); i++){
                if( dataType == SG_TYPE_FLOAT ){
                    checkPrescison();
                    drawStr += " "+ofToString(value.getValueF(i), numDecimalPlaces);
                }else if( dataType == SG_TYPE_INT ) drawStr += " "+ofToString(value.getValueI(i), 0);
                else if( dataType == SG_TYPE_BOOL ){
					/*if (value.getValueB(i) == true)	drawStr += " true";
					else drawStr += " false";*/
                } else if( dataType == SG_TYPE_STRING ) drawStr += " "+value.getValueS(i);
            }

            displayText.setText(drawStr);

            //now update our bounding box
            updateBoundingBox();
        }

        //---------------------------------------------
        virtual void updateBoundingBox(){
            if(bShowText){
                //we need to update out hitArea because the text will have moved the gui down
                hitArea.y = (int) (boundingBox.y + displayText.getTextSingleLineHeight() + titleSpacing);
                boundingBox.height = hitArea.height + displayText.getTextSingleLineHeight() + titleSpacing;
            }else{
                 //we need to update out hitArea because the text will have moved the gui down
                hitArea.y = boundingBox.y;
				boundingBox.height = hitArea.height;
            }
        }

        //---------------------------------------------
        virtual void renderText(){
            if(!bShowText) return;

            glColor4fv(textColor.getColorF());
            displayText.renderText(boundingBox.x, boundingBox.y + displayText.getTextSingleLineHeight());
        }
		
		//---------------------------------------------
		virtual void renderText(int x, int y){
			if(!bShowText) return;
			
			glColor4fv(textColor.getColorF());
			displayText.renderText(x, y + displayText.getTextSingleLineHeight());
		}

        //this also needs to be specified by the extending class
        //--------------------------------------------
        virtual void render(){

        }

         //-------------------------------------------
         virtual void setSelected(){
            fgColor.setGuiColorMode(1);
            bgColor.setGuiColorMode(1);
            outlineColor.setGuiColorMode(1);
            textColor.setGuiColorMode(1);
         }

         //-------------------------------------------
         virtual void setNormal(){
            fgColor.setGuiColorMode(0);
            bgColor.setGuiColorMode(0);
            outlineColor.setGuiColorMode(0);
            textColor.setGuiColorMode(0);
         }

		//-------------------------------------------
		//override to just set all the main fills to the same color
		virtual void seBaseColor( int norR, int norG, int norB, int norA = 255){
			fgColor.setColor(norR, norG, norB, norA);
            fgColor.setSelectedColor(norR, norG, norB, norA);
			outlineColor.setColor(norR, norG, norB, norA);
            outlineColor.setSelectedColor(norR, norG, norB, norA);
		}
	
         //-------------------------------------------
         virtual void setForegroundColor( int norR, int norG, int norB, int norA = 255){
            fgColor.setColor(norR, norG, norB, norA);
         }
	
		//-------------------------------------------
		virtual void setForegroundColor( int hexValue){
			fgColor.setColor(hexValue);
		}

         //-------------------------------------------
         virtual void setForegroundSelectColor(int selR, int selG, int selB, int selA = 255){
            fgColor.setSelectedColor(selR, selG, selB, selA);
         }
		
		//-------------------------------------------
		virtual void setForegroundSelectColor( int hexValue ){
			fgColor.setSelectedColor(hexValue);
		}

         //-------------------------------------------
         virtual void setBackgroundColor( int norR, int norG, int norB, int norA = 255){
            bgColor.setColor(norR, norG, norB, norA);
         }
		
		//-------------------------------------------
		virtual void setBackgroundColor( int hexValue ){
			bgColor.setColor(hexValue);
		}

         //-------------------------------------------
         virtual void setBackgroundSelectColor(int selR, int selG, int selB, int selA = 255){
            bgColor.setSelectedColor(selR, selG, selB, selA);
         }
		
		//-------------------------------------------
		virtual void setBackgroundSelectColor( int hexValue ){
			bgColor.setSelectedColor(hexValue);
		}

         //-------------------------------------------
        virtual void setOutlineColor( int norR, int norG, int norB, int norA = 255){
			outlineColor.setColor(norR, norG, norB, norA);
        }
		
		//-------------------------------------------
		virtual void setOutlineColor( int hexValue ){
			outlineColor.setColor(hexValue);
		}

         //-------------------------------------------
        virtual void setOutlineSelectColor(int selR, int selG, int selB, int selA = 255){
            outlineColor.setSelectedColor(selR, selG, selB, selA);
		}

		//-------------------------------------------
		virtual void setOutlineSelectColor( int hexValue ){
			outlineColor.setSelectedColor(hexValue);
		}

		 //-------------------------------------------
		 virtual void setTextColor( int norR, int norG, int norB, int norA = 255){
			textColor.setColor(norR, norG, norB, norA);
		 }
		
		//-------------------------------------------
		virtual void setTextColor( int hexValue){
			textColor.setColor(hexValue);
		}

         //-------------------------------------------
         virtual void setTextSelectColor(int selR, int selG, int selB, int selA = 255){
            textColor.setSelectedColor(selR, selG, selB, selA);
         }
		
		//-------------------------------------------
		virtual void setTextSelectColor(int hexValue){
			textColor.setSelectedColor(hexValue);
		}

        //list of properties
        //------------------
        string name;
        string drawStr;
        string xmlName;

        //guiValue value;
        guiValue value;

        guiColor fgColor;
        guiColor bgColor;
        guiColor outlineColor;
        guiColor textColor;

        ofRectangle boundingBox;
        ofRectangle hitArea;

        int numDecimalPlaces;

        //------------------
        guiTextBase displayText;

        //-------------------
        vector <guiBaseObject *>children;

        //bool isRelative;
        bool locked;

        float storedTextWidth;
        float titleSpacing;
        float fontSize;
        bool bShowText;
        bool readOnly;
        int  state;
		int  dataType;
		bool bTextEnterMode;
	
		int numGroups;
		int currentGroup;
};

