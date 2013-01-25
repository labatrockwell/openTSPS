//
//  Spacebrew.cpp
//  openTSPS
//
//  Created by BRenfer on 12/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "ofxTSPS/Person.h"
#include "ofxTSPS/Scene.h"
#include "ofxTSPS/communication/SpacebrewSender.h"

namespace ofxTSPS {
    
    //---------------------------------------------------------------------------
    SpacebrewSender::SpacebrewSender( string host ) :    
    personEnteredMessage("personEntered", "tspsperson"),
    personUpdatedMessage("personUpdated", "tspsperson"),
    personWillLeaveMessage("personLeft", "tspsperson"),
    sceneMessage("scene","tspsscene")
    {
        connection = new Spacebrew::Connection();
        setupConnection();
        connection->connect(host, "TSPS", "TSPS is an open toolkit for sensing people in spaces");
    }
    
    //---------------------------------------------------------------------------
    void SpacebrewSender::setupConnection(){
        //connection->addPublish("Scene", "TSPSScene");
        connection->addPublish( personEnteredMessage.name, personEnteredMessage.type );
        connection->addPublish( personUpdatedMessage.name, personUpdatedMessage.type );
        connection->addPublish( personWillLeaveMessage.name, personWillLeaveMessage.type );
        connection->addPublish( sceneMessage.name, sceneMessage.type );
        //connection->addPublish("customEvent", "Object"); hm...
    }
    
    //---------------------------------------------------------------------------
    void SpacebrewSender::setHost( string host ){
        if ( host == connection->getHost() ) return;
        
        spacebrewMutex.lock();
        // delete old connection
        try {
            delete connection;
        } catch ( std::exception e){
            //
        }
        
        connection = new Spacebrew::Connection();
        setupConnection();
        connection->connect(host, "TSPS", "TSPS is an open toolkit for sensing people in spaces");
        
        spacebrewMutex.unlock();
    }
    
    //---------------------------------------------------------------------------
    bool SpacebrewSender::isEnabled(){
        return bEnabled;
    }
    
    //---------------------------------------------------------------------------
    void SpacebrewSender::enable(){
        bEnabled = true;
    }
    
    //---------------------------------------------------------------------------
    void SpacebrewSender::disable(){
        bEnabled = false;
    }
    
    //---------------------------------------------------------------------------
    void SpacebrewSender::personEntered ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours ){
        personEnteredMessage.value = p->getJSON("personEntered", cameraWidth, cameraHeight, sendContours);
        if ( spacebrewMutex.tryLock()){
            connection->send( &personEnteredMessage );
            spacebrewMutex.unlock();
        }
    }
    
    //---------------------------------------------------------------------------
    void SpacebrewSender::personUpdated ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours ){
        personUpdatedMessage.value = p->getJSON("personUpdated", cameraWidth, cameraHeight, sendContours);
        if ( spacebrewMutex.tryLock()){
            connection->send( &personUpdatedMessage );
            spacebrewMutex.unlock();
        }
    }
    
    //---------------------------------------------------------------------------
    void SpacebrewSender::personWillLeave ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours ){
        personWillLeaveMessage.value = p->getJSON("personLeft", cameraWidth, cameraHeight, sendContours);
        if ( spacebrewMutex.tryLock()){
            connection->send( &personWillLeaveMessage );
            spacebrewMutex.unlock();
        }
    }
    
    //---------------------------------------------------------------------------
    void SpacebrewSender::customEvent( string eventName, string eventData ){
        // to-do...
    }
    
    //---------------------------------------------------------------------------
    void SpacebrewSender::customEvent( string eventName, vector<string>params ){
        // to-do...
    }
    
    //---------------------------------------------------------------------------
    void SpacebrewSender::customEvent( string eventName, map<string,string>params ){
        // to-do...
    }
    
    //---------------------------------------------------------------------------
    void SpacebrewSender::sceneUpdated( Scene & s ){
        sceneMessage.value = s.getJSONMessge();
        if ( spacebrewMutex.tryLock()){
            connection->send( &sceneMessage );
            spacebrewMutex.unlock();
        }
    }
    
}
