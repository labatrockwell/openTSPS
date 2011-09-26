#pragma once

class guiCustomImpl{

     public:

        guiCustomImpl(){
            mouseIsRelative = false;
        }

        virtual ~guiCustomImpl(){

        }

        virtual void saveSettings(string filename){}
        virtual void reloadSettings(){}
        virtual void saveSettings(){}
        virtual void loadSettings(string filename){}

        virtual  void update(){}
        virtual  void draw(float x, float y, float w, float h){}

        virtual void mousePressed(float x, float y){}
        virtual void mouseDragged(float x, float y){}
        virtual void mouseReleased(){}

        bool mouseIsRelative;
		
		ofRectangle boundingRect;
};
