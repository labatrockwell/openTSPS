#pragma once

#include "guiBaseObject.h"
#include "guiTypeGroup.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

#define LOCK_WIDTH 10
#define LOCK_HEIGHT 10
#define LOCK_BORDER 3

#define panelSpacing   10
#define tabWidth	25.f
#define tabHeight	10.f

#define tabPadding 2.0f

class guiTypePanel : public guiBaseObject{

    public:

		//------------------------------------------------
		//override because of tab
		void updateBoundingBox(){
			hitArea.y = (int)boundingBox.y + tabHeight;
			boundingBox.height = hitArea.height;
		}
	
        //------------------------------------------------
        guiTypePanel(){
            currentXPos     = panelSpacing;
            currentYPos     = panelSpacing;
            spacingAmntX    = panelSpacing;
            spacingAmntY    = panelSpacing;

            columns.clear();
            columns.push_back(ofRectangle(20, 20, 30, 20));
            col = 0;
			bDrawLock = false;
			
			tabRect.x      = 0;
			tabRect.y      = 0;
			tabRect.width  = tabWidth;
			tabRect.height = tabHeight;
			bSelected = false;
			
			setGroupBgColor(255,255,255,255);
		}

        //------------------------------------------------
        void setup(string panelName, float defaultX = 20, float defaultY = 10){
            name = panelName;

            columns[0] = ofRectangle(defaultX, defaultY, 50, 20);

            //we don't want our panel flashing when we click :)
            bgColor.selected = bgColor.color;
            outlineColor.selected = outlineColor.color;
			updateText();
			
			tabRect.width = displayText.getTextWidth() + tabPadding*2;
			tabRect.height = displayText.getTextSingleLineHeight()*1.5f;
			groupBg.x = panelSpacing;
			groupBg.width = getWidth() - panelSpacing*2; 
			groupBg.y = panelSpacing;
        }

        //-----------------------------------------------
        void addColumn(float minWidth){
            float colX = columns.back().x + columns.back().width + spacingAmntX;
            columns.push_back(ofRectangle(colX, 20, minWidth, 20));
        }

        //-----------------------------------------------
        bool selectColumn(int which){
            col = ofClamp(which, 0, columns.size()-1);
            return true;
        }

        //-----------------------------------------------
        void setElementSpacing(float spacingX, float spacingY){
            spacingAmntX = spacingX;
            spacingAmntY = spacingY;
        }
	
		//-----------------------------------------------
		void setTabPosition(float tabX, float tabY){
			tabRect.x = tabX;
			tabRect.y = tabY;
		}
	
		//-----------------------------------------------
		ofRectangle getTabRect(){
			return tabRect;
		}

        //-----------------------------------------------.
        virtual bool checkHit(float x, float y, bool isRelative){
            if(readOnly)return false;
            if( x >= hitArea.x && x <= hitArea.x + hitArea.width && y >= hitArea.y && y <= hitArea.y + hitArea.height){
                state = SG_STATE_SELECTED;
                
				float xx = x - boundingBox.x;
                float yy = y - boundingBox.y;
				
				if( xx  > lockRect.x && xx < lockRect.x + lockRect.width && yy > lockRect.y && yy < lockRect.y + lockRect.height ){
                    locked = !locked;
                }
					
                setSelected();
                updateGui(x, y, true, isRelative);
				
                if( !locked ){

                    float offsetX = x - hitArea.x;
                    float offsetY = y - hitArea.y;
										
                    for(int i = 0; i < children.size(); i++){
						children[i]->bTextEnterMode = false;
						children[i]->release();
					}
					
					bTextEnterMode = false;
					
                    for(int i = 0; i < children.size(); i++){
						children[i]->bTextEnterMode = bTextEnterMode;
                        children[i]->checkHit(offsetX, offsetY, isRelative);
						if (children[i]->bTextEnterMode){
							bTextEnterMode = true;
						}
                    }
                } else {
					resetChildren();
				}
                return true;
            }
            return false;
        }
	
	
		//-----------------------------------------------.
		virtual void keyPressed(int key){
			if( !locked ){
				for(int i = 0; i < children.size(); i++){
					//if (children[i]->state == SG_STATE_SELECTED) children[i]->keyPressed(key);
					children[i]->keyPressed(key);
				}
			}
		}

        //-----------------------------------------------.
        void updateGui(float x, float y, bool firstHit, bool isRelative){
            if( state == SG_STATE_SELECTED){

                float offsetX = 0;
                float offsetY = 0;

                if( isRelative ){
                    offsetX = x;
                    offsetY = y;
                }else{
                    offsetX = x - hitArea.x;
                    offsetY = y - hitArea.y;
                }

                if( !locked ){

                    for(int i = 0; i < children.size(); i++){
                        children[i]->updateGui(offsetX, offsetY, firstHit, isRelative);
                    }

                }
            }
        }

        //we should actually be checking our child heights
        //every frame to see if the panel needs to adjust layout
        //for now we only check heights when elements are first added
        //----------------------------------------------
        virtual void update(){
            updateText();

            lockRect.x          = boundingBox.width - (LOCK_WIDTH + spacingAmntX + LOCK_BORDER);
            lockRect.y          = spacingAmntY - LOCK_BORDER;
            lockRect.width      = LOCK_WIDTH + LOCK_BORDER * 2;
            lockRect.height     = LOCK_HEIGHT + LOCK_BORDER * 2;

            for(int i = 0; i < children.size(); i++){
                children[i]->update();
            }

            for(int i = 0; i < whichColumn.size(); i++){
                if( children[i]->boundingBox.x != columns[whichColumn[i]].x ){
                    float amntToShiftX = columns[whichColumn[i]].x - children[i]->boundingBox.x;

                    children[i]->hitArea.x += amntToShiftX;
                    children[i]->boundingBox.x += amntToShiftX;
                }
            }
			
			//reset y positions (if elements are added / dimensions changed)
			
			currentYPos = panelSpacing;
			for(int i = 0; i < children.size(); i++){
				children[i]->boundingBox.y = (int) currentYPos;
				currentYPos += children[i]->getHeight() + spacingAmntY;
			}
			
			//update group bg
			groupBg.height = 0;
			groupBg.width = getWidth() - panelSpacing*2; 
			if (groups.size() > 0) groupBg.height = panelSpacing + groups[groups.size()-1]->boundingBox.y + groups[groups.size()-1]->getHeight();
        }

        //-----------------------------------------------
        void addElement( guiBaseObject * element ){
            element->updateText();
			
			//currentYPos = 0;
			currentYPos = panelSpacing;
            for(int i = 0; i < children.size(); i++){
				children[i]->setPosition((int)children[i]->getPosX(), (int)currentYPos);
				currentYPos += children[i]->getHeight() + spacingAmntY;
			}
			
            element->setPosition((int)columns[col].x, (int)currentYPos);
            whichColumn.push_back(col);
			
            //add the element to the panel list
            children.push_back( element );
			
            //update the current position for the next element
            columns[col].y += element->getHeight() + spacingAmntY;
			
            float checkWidth = element->getWidth();
            if(checkWidth >= columns[col].width ){
                float amnt = checkWidth - columns[col].width;
                columns[col].width += amnt;

                for(int i = col+1; i < columns.size(); i++){
                    columns[i].x += amnt;
                }
            }

            //see if we need to resize!
            //checkResize(element);
			
			
        }

        //-----------------------------------------------.
        void drawLocked(){
            ofPushMatrix();
                ofFill();
                ofTranslate(lockRect.x, lockRect.y, 0);

                ofSetColor(200, 0, 0);
                ofRect(0, 0, lockRect.width, lockRect.height);

                ofTranslate(LOCK_BORDER, LOCK_BORDER, 0);

                ofSetColor(255, 255, 255);
                ofEllipse(LOCK_WIDTH/2, LOCK_HEIGHT/2, LOCK_WIDTH * 0.8, LOCK_HEIGHT * 0.9);

                ofSetColor(200, 0, 0);
                ofEllipse(LOCK_WIDTH/2, LOCK_HEIGHT/2, LOCK_WIDTH * 0.8 * 0.6, LOCK_HEIGHT * 0.9 * 0.6);

                ofSetColor(255, 255, 255);
                ofRect(0, LOCK_HEIGHT/2, LOCK_WIDTH, LOCK_HEIGHT/2);
            ofPopMatrix();
        }

        //-----------------------------------------------.
        void drawUnlocked(){
            ofPushMatrix();
                ofFill();
                ofTranslate(lockRect.x, lockRect.y, 0);

                ofSetColor(0, 0, 0);
                ofRect(0, 0, lockRect.width, lockRect.height);

                ofTranslate(LOCK_BORDER, LOCK_BORDER, 0);
                ofSetColor(255, 255, 255);
                ofEllipse(LOCK_WIDTH/2, LOCK_HEIGHT * 0.4, LOCK_WIDTH * 0.8, LOCK_HEIGHT * 0.9);

                ofSetColor(0, 0, 0);
                ofEllipse(LOCK_WIDTH/2, LOCK_HEIGHT * 0.44, LOCK_WIDTH * 0.8 * 0.6, LOCK_HEIGHT * 0.9 * 0.6);

                ofSetColor(255, 255, 255);
                ofRect(0, LOCK_HEIGHT/2, LOCK_WIDTH, LOCK_HEIGHT/2);

                ofSetColor(0, 0, 0);
                ofRect(0, LOCK_HEIGHT * 0.5 - LOCK_HEIGHT * 0.25 , LOCK_WIDTH * 0.35, LOCK_HEIGHT * 0.25);

            ofPopMatrix();
        }

		//-----------------------------------------------.
		void setDrawLock( bool _bDrawLock ){
			bDrawLock = _bDrawLock;
		}
		
        //-----------------------------------------------.
        void render(){
            ofPushStyle();

                glPushMatrix();
                glTranslatef(boundingBox.x, boundingBox.y, 0);
                    //draw the background
                    ofFill();
                    glColor4fv(bgColor.getSelectedColorF());
                    ofRect(0, 0, boundingBox.width, boundingBox.height);

                    //draw the outline
                    ofNoFill();
                    glColor4fv(outlineColor.getColorF());			
					ofBeginShape();
					ofVertex(tabRect.x-boundingBox.x, 0);
					ofVertex(0, 0);
					ofVertex(0, boundingBox.height);
					ofVertex(boundingBox.width, boundingBox.height);
					ofVertex(boundingBox.width, 0);
					ofVertex(tabRect.x-boundingBox.x+tabRect.width, 0);					
					ofEndShape(false);
			
					if (groups.size() > 0){						
						ofFill();
						glColor4fv(groupBgColor.getSelectedColorF());
						ofRect(groupBg.x, groupBg.y, groupBg.width, groupBg.height);
					}
			
					if (bDrawLock){
						if( locked ){
							drawLocked();
						} else {
							drawUnlocked();
						}
					}

                glPopMatrix();
			
                glPushMatrix();
                    glTranslatef(hitArea.x, hitArea.y, 0);
						for(int i = 0; i < children.size(); i++){
                            children[i]->render();
                        }
                glPopMatrix();
			
            ofPopStyle();
			
			
			renderTab();
        }
	
		//-----------------------------------------------.
		void renderTab(){
			ofPushStyle();	
				//draw tab
				ofPushMatrix();
				ofTranslate(tabRect.x, tabRect.y);
				ofFill();
			
				if (bSelected){
					glColor4fv(bgColor.getSelectedColorF());
					ofBeginShape();
					ofVertex(0, tabRect.height);
					ofVertex(0, 0);
					ofVertex(tabRect.width, 0);
					ofVertex(tabRect.width, tabRect.height);
					ofEndShape(false);
				} else {
					glColor4fv(bgColor.getColorF());
					ofRect(0,0,tabRect.width, tabRect.height-1);
				}
				
				//ofRect(0,0,tabRect.width, tabRect.height);
				ofNoFill();
				glColor4fv(outlineColor.getColorF());
				ofBeginShape();
				ofVertex(0, tabRect.height);
				ofVertex(0, 0);
				ofVertex(tabRect.width, 0);
				ofVertex(tabRect.width, tabRect.height);
				ofEndShape(false);
				ofPopMatrix();
				renderText(tabRect.x + tabPadding, tabRect.y);
				
			ofPopStyle();
		};
	
		//-----------------------------------------------.
		void setGroupBgColor( int r, int g, int b, int a ){
			groupBgColor.setColor(r,g,b,a);
		}
	
		bool bSelected;
	
		bool bDrawLock;
        ofRectangle lockRect;
		ofRectangle tabRect;

        float currentXPos;
        float currentYPos;
        float spacingAmntX;
        float spacingAmntY;
		
		vector <ofRectangle> columns;
		vector <guiTypeGroup *> groups;
		ofRectangle groupBg;
		guiColor	groupBgColor;
        vector <int> whichColumn;
        int col;

};
