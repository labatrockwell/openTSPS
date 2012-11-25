/*
 *  TCPSender.cpp
 *  openTSPS
 */

#include "ofxTSPS/communication/TCPSender.h"
#include "ofxTSPS/Person.h"

namespace ofxTSPS {
    
    TCPSender::TCPSender(){
        bConnected = false;
    }
    
    /***************************************************************
     SETUP + CONNECT
     ***************************************************************/	
    
    void TCPSender::setup( int _port ){
        if ( port != _port || !bConnected ){
            port = _port;
            
            // disconnect old clients + close
            while (TCP.getNumClients() > 0){
                TCP.disconnectClient(0);
            }
            if ( bConnected ){
                TCP.close();
            }
            bConnected = TCP.setup( port );
            if ( !bConnected ){
                TCP.close();
            }
            oldport = port;
        }
    };
    
    /***************************************************************
     UPDATE
     ***************************************************************/	
    
    TCPSender::~TCPSender(){
        TCP.close();
    };
    
    void TCPSender::update(){ 
        if (oldport != port){
            oldport = port;
            setup(port);				
        }
    }
    /***************************************************************
     SEND
     ***************************************************************/	
    
    void TCPSender::send(){
        
        map<int,ofxTCPClient>::iterator it;
        for(it=TCP.TCPConnections.begin(); it!=TCP.TCPConnections.end(); it++){
            int err = 0;
            if(it->second.isConnected()) err = it->second.send(currentString);
        }
        currentString = "";
    };
    
    /***************************************************************
     SEND
     ***************************************************************/
    
    //--------------------------------------------------------------
    void TCPSender::personEntered ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
        stringstream message;
        message<<"TSPS/personEntered/"<<";";
        message<<getPersonString(p,centroid,cameraWidth,cameraHeight,bSendContours );
        message<<"\\";
        
        currentString += message.str();
    };
    
    //--------------------------------------------------------------
    void TCPSender::personMoved ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
        stringstream message;
        message<<"TSPS/personMoved/"<<";";
        message<<getPersonString(p,centroid,cameraWidth,cameraHeight,bSendContours );
        message<<"\\";
        
        currentString += message.str();
    };
    
    //--------------------------------------------------------------
    void TCPSender::personUpdated ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
        stringstream message;
        message<<"TSPS/personUpdated/"<<";";
        message<<getPersonString(p,centroid,cameraWidth,cameraHeight,bSendContours );
        message<<"\\";
        
        currentString += message.str();
    };
    
    //--------------------------------------------------------------
    void TCPSender::personWillLeave ( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours )
    {
        stringstream message;
        message<<"TSPS/personWillLeave/"<<";";
        message<<getPersonString(p,centroid,cameraWidth,cameraHeight,bSendContours );
        message<<"\\";
        
        currentString += message.str();
    };
    
    
    //--------------------------------------------------------------
    void TCPSender::customEvent( string eventName, string eventData ){
        stringstream message;
        message<<"TSPS/customEvent/"<<";";
        message<<"name/"<<eventName<<";data/"<<eventData<<";";
        message<<"\\";
        
        currentString += message.str();
    }
    
    //--------------------------------------------------------------
    void TCPSender::customEvent( string eventName, vector<string>params ){
        stringstream message;
        message<<"TSPS/customEvent/"<<";";
        message<<"name/"<<eventName<<";data/";
        for ( int i=0, len=params.size(); i<len; i++ ){
            message<<params[i];
            if ( i +1 < len ) message<<",";
        }
        message<<";\\";
        
        currentString += message.str();
    }
    
    //--------------------------------------------------------------
    void TCPSender::customEvent( string eventName, map<string,string>params ){
        stringstream message;
        message<<"TSPS/customEvent/"<<";";
        message<<"name/"<<eventName<<";data/";
        
        map<string,string>::iterator it;
        
        for ( it = params.begin(); it != params.end(); it++){
            message<<(*it).first<<"/"<<(*it).second<<";";
        }
        message<<"\\";
        
        currentString += message.str();
    }
    
    /***************************************************************
     GET PERSON STRING (so we don't have to write this so many times)
     ***************************************************************/
    
    string TCPSender::getPersonString( Person * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool bSendContours ){
        
        //construct a message with each value set separated by ;
        // within each set, items are separated by ,
        // and objects are separated by :
        // e.g. velcoity is:
        // "velocity:velocity.x:velocity.y"
        // while contours are "contours:contour0.x:contour0.y,contour1.x:contour1.y..."
        
        stringstream message;
        message<<"id/"<<p->pid<<";";
        message<<"age/"<<p->age<<";";
        message<<"centroid/"<<"x>"<<centroid.x<<":"<<"y>"<<centroid.y<<";"; //pass in normalized centroid
        message<<"velocity/"<<"x>"<<ofToString(p->velocity.x, 3)<<":"<<"y>"<<ofToString(p->velocity.y, 3)<<";";
        message<<"depth/"<<p->depth<<";";
        
        ofRectangle boundingRect = p->getBoundingRectNormalized(cameraWidth,cameraHeight);
        message<<"boundingrect/"<<"x>"<<boundingRect.x<<":"<<"y>"<<boundingRect.y<<":"<<"width>"<<boundingRect.width<<":"<<"height>"<<boundingRect.height<<";";
        
        message<<"opticalflow/"<<"x>"<<p->opticalFlowVectorAccumulation.x<<":"<<"y>"<<p->opticalFlowVectorAccumulation.y<<";";
        ofRectangle haarRect = p->getHaarRectNormalized(cameraWidth,cameraHeight);
        message<<"haarrect/"<<"x>"<<haarRect.x<<":"<<"y>"<<haarRect.y<<":"<<"width>"<<haarRect.width<<":"<<"height>"<<haarRect.height<<";";
        message<<"highest/"<<"x>"<<( (float) p->highest.x / cameraWidth )<<":y>"<<( (float) p->highest.y / cameraHeight)<<";";
        
        if (bSendContours){
            message<<"contours/";
            for (int i=0; i<p->simpleContour.size(); i++){
                message<<"x>"<<p->simpleContour[i].x<<":"<<"y>"<<p->simpleContour[i].y;
                if (i+1 < p->simpleContour.size()) message<<",";
            };
            message<<";";
        }	
        return message.str();
    }
}