/*
 *  ofxTSPS::Scene.cpp
 *  TSPSPeopleVision
 *
 */

#include "ofxTSPS/Scene.h"
#include "ofxTSPS/Person.h"
#include "ofxOscMessage.h"
#include "ofUtils.h"

namespace ofxTSPS {
    //------------------------------------------------------------------------
    Scene::Scene():
    numPeople(0),
    percentCovered(0),
    gridX(0), gridY(0)
    {
        
    }
    
    //------------------------------------------------------------------------
    void Scene::reset(){
        percentCovered = 0;
        numPeople = 0;
        for ( int i=0; i<activeGrid.size(); i++){
            activeGrid[i] = false;
        }
    }
    
    //------------------------------------------------------------------------
    void Scene::update( vector<Person*> * people, int cameraWidth, int cameraHeight ){
        reset();
        numPeople = people->size();
        for (int i=0; i<people->size(); i++){
            processPerson((*people)[i], cameraWidth, cameraHeight);
        }
        if ( numPeople > 0 ) averageMotion /= numPeople;
    }
    
    //------------------------------------------------------------------------
    void Scene::draw( int width, int height ){
        ofPushStyle();{
            for (int i=0; i<grid.size(); i++){
                ofRectangle check = grid[i];
                check.x *= width;
                check.y *= height;
                check.scale( width, height );
                if ( activeGrid[i] ){
                    ofFill();
                    ofSetColor(255,100);
                } else {
                    ofNoFill();
                    ofSetColor(255);
                }
                ofRect(check);
            }
        }; ofPopStyle();
    }
    
    //------------------------------------------------------------------------
    void Scene::processPerson( Person * p, int cameraWidth, int cameraHeight ){
        // check grid
        for (int i=0; i<grid.size(); i++){
            ofRectangle check = grid[i];
            check.x *= cameraWidth;
            check.y *= cameraHeight;
            check.scale( cameraWidth, cameraHeight );
            if ( check.inside(p->centroid)){
                activeGrid[i] = true;
            }
        }
        
        // add to percent covered (to-do)
        
        // avg motion
        averageMotion += p->velocity;
    }
    
    //------------------------------------------------------------------------
    void Scene::buildGrid( int w, int h ){
        grid.clear();
        activeGrid.clear();
        
        gridX = w;
        gridY = h;
        
        if ( w == 0 || h == 0 ) return;
        
        for (int x=0; x<w; x++){
            for (int y=0; y<h; y++){
                grid.push_back(ofRectangle((float) x/w, (float) y/h, 1.0 / (float) w, 1.0 / (float) h));
                activeGrid.push_back(false);
            }
        }
    }
    
    //------------------------------------------------------------------------
    vector<ofRectangle> Scene::getGrid(){
        return grid;
    }
    
    //------------------------------------------------------------------------
    int Scene::getGridX(){
        return gridX;
    }
    
    //------------------------------------------------------------------------
    int Scene::getGridY(){
        return gridY;
    }
#pragma mark communication
    
    //------------------------------------------------------------------------
    ofxOscMessage Scene::getOscMessage( string address ){
        ofxOscMessage m;
        m.setAddress(address);
        m.addIntArg(ofGetElapsedTimeMillis());
        m.addIntArg(numPeople);
        m.addFloatArg(percentCovered);
        m.addFloatArg(averageMotion.x);
        m.addFloatArg(averageMotion.y);
        
        // grid
        m.addIntArg(gridX);
        m.addIntArg(gridY);
        for (int i=0; i<activeGrid.size(); i++){
            m.addIntArg(activeGrid[i]);
        }
        return m;
    }
    
    //------------------------------------------------------------------------
    string Scene::getTCPMessage(){
        stringstream ss;
        ss<<"TSPS/scene/"<<";";
        ss<<"numPeople/"<<numPeople<<";";
        ss<<"percentCovered/"<<percentCovered<<";";
        ss<<"averageMotion/"<<averageMotion.x<<","<<averageMotion.y<<";";
        ss<<"gridX/"<<gridX<<";";
        ss<<"gridY/"<<gridY<<";";
        ss<<"grid/";
        for (int i=0; i<activeGrid.size(); i++){
            ss<<activeGrid[i];
            if ( i + 1 < activeGrid.size() ){
                ss<<",";
            }
        }
        ss<<";";
        ss<<"\\";
        
        return ss.str();
    }
    
    //------------------------------------------------------------------------
    string Scene::getJSONMessge(){
        stringstream ss;
        ss<<"{\"type\":\"scene\",";
        ss<<"\"numPeople\":\""<<numPeople<<"\",\"percentCovered\":\""<<percentCovered<<"\",";
        ss<<"\"averageMotion\":{\"x\":\""<<averageMotion.x<<"\",\"y\":\""<<averageMotion.y<<"\"},";
        ss<<"\"gridX\":\""<<gridX<<"\",\"gridY\":\""<<gridY<<"\",";
        ss<<"\"grid\":[";
        for (int i=0; i<activeGrid.size(); i++){
            ss<<activeGrid[i];
            if ( i + 1 < activeGrid.size() ){
                ss<<",";
            }
        }
        ss<<"]";
        ss<<"}";
        return ss.str();
    }
}