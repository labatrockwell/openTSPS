#pragma once

#include "guiBaseObject.h"
#include "guiTypeText.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

class guiTypeText : public guiBaseObject{

    public:
		
	
		//------------------------------------------------
		void setup(string textName, string defaultVal){
			value.addValueS( (string)defaultVal);
			name = textName;
			setDimensions(200, 20);
			valueText.setText( value.getValueS() );
		}
		
		void setFont(ofTrueTypeFont * fontPtr){
			displayText.setFont(fontPtr);
			valueText.setFont(fontPtr);
			valueText.setText( value.getValueS() );
		}
	
		//-----------------------------------------------.
		void updateGui(float x, float y, bool firstHit, bool isRelative){
			if(!firstHit)return;
			
			if( state == SG_STATE_SELECTED){
				
			}
			updateText();
			valueText.setText( value.getValueS() );
		}
	
		void keyPressed(int key){
			string newValue = "";
			newValue += value.getValueS();
			if ( bTextEnterMode ){
				if (key == OF_KEY_BACKSPACE){
					if (newValue.size() > 0) {
						newValue.erase(newValue.size()-1);
					}
				} else if ( key == OF_KEY_RETURN ){
					bTextEnterMode = false;
					setNormal();					
				} else {
					newValue += key;
				}
			}
			bool set = value.setValueS( newValue );
		}
		
		//-----------------------------------------------.
        void render(){
            if (!enabled) return;
			string newValue = value.getValueS();
			valueText.setText( value.getValueS() );
			ofPushStyle();
			glPushMatrix();
			guiBaseObject::renderText();
			
			//draw the background
			ofFill();
			ofSetColor(bgColor.getColor().r,bgColor.getColor().g, bgColor.getColor().b, bgColor.getColor().a);
			ofDrawRectangle(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
			
			//draw the outline
			ofNoFill();
			ofSetColor(outlineColor.getColor().r,outlineColor.getColor().g, outlineColor.getColor().b, outlineColor.getColor().a);
			ofDrawRectangle(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
			
			ofFill();
			ofSetColor(outlineColor.getColor().r, outlineColor.getColor().g, outlineColor.getColor().b, outlineColor.getColor().a);
			valueText.renderText(hitArea.x + 2, hitArea.y+hitArea.height*3/4);
			//ofDrawBitmapString(newValue, hitArea.x, hitArea.y+hitArea.height*3/4);
			ofNoFill();
			
			ofSetColor(fgColor.getColor().r, fgColor.getColor().g, fgColor.getColor().b, fgColor.getColor().a);
			ofDrawRectangle(hitArea.x+3, hitArea.y+3, -6 + hitArea.width, -6 + hitArea.height);
			
			glPopMatrix();
			ofPopStyle();
		}
		
		virtual void release(){
			if (!bTextEnterMode){
				state = SG_STATE_NORMAL;
				setNormal();
			}
		}
	
	private:
	
		guiTextBase valueText;	
		

};
