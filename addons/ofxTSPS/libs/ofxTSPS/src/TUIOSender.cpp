/*
 *  TUIOSender.cpp
 *  openTSPS
 */

#include "ofxTSPS/communication/TUIOSender.h"
#include "ofxTSPS/Scene.h"

namespace ofxTSPS {
    void TUIOSender::cursorPressed(float x, float y, int cursorId) {
        // insert cursor
        newCursors[cursorId] = TuioPersonCursor();
        newCursors[cursorId].x      = x;
        newCursors[cursorId].y      = y;
        newCursors[cursorId].isAlive = true;
        newCursors[cursorId].wasAlive = false;
    }
    
    void TUIOSender::cursorDragged(float x, float y, int cursorId) {
        currentCursors[cursorId].x      = x;
        currentCursors[cursorId].y      = y;
        currentCursors[cursorId].moved   = true;
        currentCursors[cursorId].isAlive = true;
    }
    
    void TUIOSender::cursorReleased(float x, float y, int cursorId) {
        currentCursors[cursorId].x      = x;
        currentCursors[cursorId].y      = y;
        currentCursors[cursorId].isAlive   = false;
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
        cout<<"SET UP AT "<<host<<":"<<port<<endl;
    }
    
    
    void TUIOSender::update() {
        if(tuioServer == NULL) return;
        
        currentTime = TuioTime::getSessionTime();
        tuioServer->initFrame(currentTime);
        
        
        static map<int, TuioPersonCursor>::iterator it;
        
        // insert new cursors
        
        for (it = newCursors.begin(); it != newCursors.end(); ++it){
            currentCursors[it->first] = TuioPersonCursor();
            currentCursors[it->first].set( it->second);
        }
        
        newCursors.clear();
        
        vector<int> erase;
        
        for (it = currentCursors.begin(); it != currentCursors.end(); ++it){
            float x = it->second.x;
            float y = it->second.y;
			
            //		char printstr[1024];
            //		sprintf(printstr, "CURSOR %i ALIVE: %i WAS ALIVE: %i MOVED: %i\n", i, currentCursors[i].isAlive, currentCursors[i].wasAlive, currentCursors[i].moved);
            //		printf(printstr);
            
            // new cursor
            if(it->second.isAlive && tuioCursor.count(it->first) == 0) {
                ofLogVerbose()<<"ofxTUIOSender - addTuioCursor "<<it->first<<","<<x<<","<<"y";
                tuioCursor[it->first] = tuioServer->addTuioCursor(x,y);
                
            // cursor removed
            } else if(!it->second.isAlive && it->second.wasAlive) {
                if(verbose) printf("ofxTUIOSender - removeTuioCursor %i %f, %f\n", it->first, x, y);
                if(tuioCursor[it->first]) tuioServer->removeTuioCursor( tuioCursor[it->first] );
                else printf("** WEIRD: Trying to remove tuioCursor %i but it's null\n", it->first);
                
                erase.push_back(it->first);
                
            } else if(it->second.isAlive && it->second.wasAlive && it->second.moved) {
                it->second.moved = false;
                if(verbose) printf("ofxTUIOSender - updateTuioCursor %i %f, %f\n", it->first, x, y);
                if( tuioCursor[it->first] ) tuioServer->updateTuioCursor( tuioCursor[it->first], x, y);
                else printf("** WEIRD: Trying to update tuioCursor %i but it's null\n", it->first);
                
            }
            
            if ( currentCursors.count(it->first) != 0 ){
                it->second.wasAlive = it->second.isAlive;
            }
        }
        for (int i=0; i<erase.size(); i++){
            currentCursors.erase(erase[i]);
            tuioCursor.erase(erase[i]);
        }
        erase.clear();
        //printf("\n\n");
        
        tuioServer->stopUntouchedMovingCursors();
        tuioServer->commitFrame();
    }
}