/*
 *  TUIOSender.cpp
 *  openTSPS
 */

#include "ofxTSPS/communication/TUIOSender.h"

namespace ofxTSPS {
    void TUIOSender::cursorPressed(float x, float y, int cursorId) {
        myCursor[cursorId].x      = x;
        myCursor[cursorId].y      = y;
        myCursor[cursorId].isAlive = true;
    }
    
    void TUIOSender::cursorDragged(float x, float y, int cursorId) {
        myCursor[cursorId].x      = x;
        myCursor[cursorId].y      = y;
        myCursor[cursorId].moved   = true;
        myCursor[cursorId].isAlive = true;
    }
    
    void TUIOSender::cursorReleased(float x, float y, int cursorId) {
        myCursor[cursorId].x      = x;
        myCursor[cursorId].y      = y;
        myCursor[cursorId].isAlive   = false;
    }
    
    void TUIOSender::setup(std::string host, int port) {
        if(this->host.compare(host) == 0 && this->port == port) return;
        
        if(verbose) printf("ofxTUIOSender::setup(host: %s, port: %i\n", host.c_str(), port);
        this->host = host;
        this->port = port;
        if(tuioServer) delete tuioServer;
        tuioServer = new TuioServer((char*)host.c_str(), port);
        tuioServer->enableFullUpdate();
        currentTime = TuioTime::getSessionTime();   
        for(int i = 0; i < OF_MAX_TOUCHES; i++){
            myCursor[i].isAlive = myCursor[i].wasAlive = false;
        }
        cout<<"SET UP AT "<<host<<":"<<port<<endl;
    }
    
    
    void TUIOSender::update() {
        if(tuioServer == NULL) return;
        
        currentTime = TuioTime::getSessionTime();
        tuioServer->initFrame(currentTime);
        
        for(int i=0; i<OF_MAX_TOUCHES; i++) {
            float x = myCursor[i].x;
            float y = myCursor[i].y;
			
            //		char printstr[1024];
            //		sprintf(printstr, "CURSOR %i ALIVE: %i WAS ALIVE: %i MOVED: %i\n", i, myCursor[i].isAlive, myCursor[i].wasAlive, myCursor[i].moved);
            //		printf(printstr);
            
            if(myCursor[i].isAlive && !myCursor[i].wasAlive) {
                if(verbose) printf("ofxTUIOSender - addTuioCursor %i %f, %f\n", i, x, y);
                tuioCursor[i] = tuioServer->addTuioCursor(x,y);   
                
            } else if(!myCursor[i].isAlive && myCursor[i].wasAlive) {
                if(verbose) printf("ofxTUIOSender - removeTuioCursor %i %f, %f\n", i, x, y);
                if(tuioCursor[i]) tuioServer->removeTuioCursor(tuioCursor[i]);
                else printf("** WEIRD: Trying to remove tuioCursor %i but it's null\n", i);
                
            } else if(myCursor[i].isAlive && myCursor[i].wasAlive && myCursor[i].moved) {
                myCursor[i].moved = false;
                if(verbose) printf("ofxTUIOSender - updateTuioCursor %i %f, %f\n", i, x, y);
                if(tuioCursor[i]) tuioServer->updateTuioCursor(tuioCursor[i], x, y);
                else printf("** WEIRD: Trying to update tuioCursor %i but it's null\n", i);
                
            }
            
            myCursor[i].wasAlive = myCursor[i].isAlive;
        }
        //printf("\n\n");
        
        tuioServer->stopUntouchedMovingCursors();
        tuioServer->commitFrame();
    }
}