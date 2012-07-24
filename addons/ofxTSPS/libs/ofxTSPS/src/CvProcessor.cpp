//
//  CvProcessor.cpp
//  openTSPS
//
//  Created by Brett Renfer on 7/22/12.
//  Copyright (c) 2012 Robotconscience. All rights reserved.
//

#include "ofxTSPS/cv/CvProcessor.h"
#include "ofxTSPS/cv/CvPerson.h"

using namespace ofxCv;
using namespace cv;

namespace ofxTSPS {
    //------------------------------------------------------------------------
    CvProcessor::CvProcessor(){
        // capabilities
        bCanTrackHaar       = true;
        bCanTrackContours   = true;
        bCanTrackSkeleton   = false;
        bCanTrackOpticalFlow = true;
        
        // default settings
        bTrackHaar          = false;
        bTrackContours      = true;
        bTrackSkeleton      = false;
        bTrackOpticalFlow   = false;
        
        setThreshold();
        setBlobSettings();
        setOpticalflowMinMax();
        setHaarPadding();
    }
    
    //------------------------------------------------------------------------
    void CvProcessor::setupProcessor(){
        resize(tspsWidth, tspsHeight);
        
        //setup contour finder
        contourFinder.setThreshold(15);
        contourFinder.getTracker().setPersistence(5);
        contourFinder.setAutoThreshold( false );
		contourFinder.getTracker().setMaximumDistance(300);
		contourFinder.setSimplify(false);
        
        //setup optical flow
        
        flow.setPyramidScale( .5 );
        flow.setNumLevels( 4 );
        flow.setWindowSize( 2 );
        flow.setNumIterations( 2 );
        flow.setPolyN( 7 );
        flow.setPolySigma( 1.5 );
        flow.setUseGaussian( 0 );
    }
    
    //------------------------------------------------------------------------
    void CvProcessor::exit(){}
    
    //------------------------------------------------------------------------
    void CvProcessor::draw(){}
    
    //------------------------------------------------------------------------
    void CvProcessor::setCameraImage( ofBaseImage & image ){
        
    }
    
    //------------------------------------------------------------------------
    void CvProcessor::captureBackground( ofBaseImage & image ){
        backgroundImage.setFromPixels( image.getPixelsRef() );
    }
    
    //------------------------------------------------------------------------
    void CvProcessor::progressiveBackground( ofBaseImage & image, float amount ){
        // to-do
    }
    
    //------------------------------------------------------------------------
    void CvProcessor::blankBackground(){
        subtract(backgroundImage, backgroundImage, backgroundImage);
    }
    
    //------------------------------------------------------------------------
    ofPixelsRef CvProcessor::difference( ofBaseImage & image, TrackingType trackingType ){
        if(trackingType == TRACK_ABSOLUTE){
            absdiff( backgroundImage, (ofImage&) image, differencedImage);
        } else {
            if(trackingType == TRACK_LIGHT){
                subtract( (ofImage&) image, backgroundImage, differencedImage);
            } else if(trackingType == TRACK_DARK){ 
                subtract ( backgroundImage, (ofImage&) image, differencedImage);
            }
        }
        differencedImage.update();
        return differencedImage.getPixelsRef();
    }
    
    //------------------------------------------------------------------------
    ofPixelsRef CvProcessor::process ( ofBaseImage & image ){
        differencedImage.setFromPixels(image.getPixelsRef());
        ofxCv::threshold(differencedImage, threshold);
        
        //reset scene
        scene->percentCovered = 0;
        if ( bTrackOpticalFlow ){
            scene->averageMotion = flow.getAverageFlowInRegion( ofRectangle(0,0,tspsWidth,tspsHeight) );
        } else {
            scene->averageMotion = ofPoint(0,0);
        }
        
        // find contours
        contourFinder.setMinArea( minBlobArea * tspsWidth * tspsHeight );
        contourFinder.setMaxArea( maxBlobArea * tspsWidth * tspsHeight );
        contourFinder.findContours( differencedImage );
        
        // update people
        RectTracker& tracker = contourFinder.getTracker();
        
        for(int i = 0; i < contourFinder.size(); i++){
            unsigned int id = contourFinder.getLabel(i);
            if(tracker.existsPrevious(id)) {
                CvPerson* p = (CvPerson *) getTrackedPerson(id);
                //somehow we are not tracking this person, safeguard (shouldn't happen)
                if(NULL == p){
                    ofLog(OF_LOG_WARNING, "Person::warning. encountered persistent blob without a person behind them\n");
                    continue;
                }
                p->oid = i; //hack ;(
                
                scene->percentCovered += p->area;
                
                //update this person with new blob info
                // to-do: make centroid dampening dynamic
                p->update(true);
                
                
                //normalize simple contour
                for (int i=0; i<p->simpleContour.size(); i++){
                    p->simpleContour[i].x /= tspsWidth;
                    p->simpleContour[i].y /= tspsHeight;
                }
                
                //find peak in blob (only useful with Kinect)
                // TO-DO
                /*CvPoint minLoc, maxLoc;
                double minVal = 0, maxVal = 0;
                grayImg.setROI( p->boundingRect );
                cvMinMaxLoc( grayImg.getCvImage(), &minVal, &maxVal, &minLoc, &maxLoc, 0);
                
                // set highest and lowest points: x, y, VALUE stored in .z prop
                // ease vals unless first time you're setting them
                if ( p->highest.x == -1 ){
                    p->highest.set(  p->boundingRect.x + maxLoc.x,  p->boundingRect.y + maxLoc.y, maxVal);
                    p->lowest.set(  p->boundingRect.x + minLoc.x,  p->boundingRect.y + minLoc.y, minVal);
                } else {
                    p->highest.x = ( p->highest.x * .7 ) + ( p->boundingRect.x + maxLoc.x ) * .3;
                    p->highest.y = ( p->highest.y * .7 ) + ( p->boundingRect.y + maxLoc.y ) * .3;
                    p->highest.z = ( p->highest.z * .7) + ( maxVal ) * .3;
                    p->lowest.x = ( p->lowest.x * .7 ) + ( p->boundingRect.x + minLoc.x ) * .3;
                    p->lowest.y = ( p->lowest.y * .7 ) + ( p->boundingRect.y + minLoc.y ) * .3;
                    p->lowest.z = ( p->lowest.z * .7) + ( minVal ) * .3;            
                }*/
                
                // ROI for opticalflow
                ofRectangle roi = ofRectangle( p->boundingRect );
                roi.x *= trackingScale;
                roi.y *= trackingScale;
                roi.width *= trackingScale;
                roi.height *= trackingScale;
                
                // sum optical flow for the person
                if ( bTrackOpticalFlow ){
                    // TO-DO!
                    p->opticalFlowVectorAccumulation = flow.getAverageFlowInRegion(roi);
                } else {
                    p->opticalFlowVectorAccumulation.x = p->opticalFlowVectorAccumulation.y = 0;
                }
                
                //detect haar patterns (faces, eyes, etc)
                if ( bTrackHaar ){
                    // ROI for haar
                    ofRectangle roi;
                    roi.x		= fmax( (p->boundingRect.x - haarAreaPadding) * trackingScale / 2, 0.0f );
                    roi.y		= fmax( (p->boundingRect.y - haarAreaPadding) * trackingScale / 2, 0.0f );
                    //roi.width	= fmin( (p->boundingRect.width  + haarAreaPadding*2) * trackingScale / 2, haarImage.width - roi.x );
                    //roi.height	= fmin( (p->boundingRect.height + haarAreaPadding*2) * trackingScale / 2, haarImage.height - roi.y );	
                    
                    //find the region of interest, expanded by haarArea.
                    bool haarThisFrame = false;
                    /*vector<ofRectangle> haarRects = haarTracker.getCurrentHaarRects();
                    for(int i = 0; i < haarRects.size(); i++){
                        ofRectangle hr = haarRects[i];
                        //check to see if the haar is contained within the bounding rectangle
                        if(hr.x > roi.x && hr.y > roi.y && hr.x+hr.width < roi.x+roi.width && hr.y+hr.height < roi.y+roi.height){
                            hr.x /= trackingScale / 2;
                            hr.y /= trackingScale / 2;
                            hr.width /= trackingScale / 2;
                            hr.height /= trackingScale / 2;
                            p->setHaarRect(hr);
                            haarThisFrame = true;
                            break;
                        }
                    }*/
                    if(!haarThisFrame){
                        p->noHaarThisFrame();
                    }
                }
                
                EventArgs args;
                args.person = p;
                args.scene  = scene;
                
                ofNotifyEvent( Events().personUpdated, args, this );
            } else {
                ofPoint centroid = toOf(contourFinder.getCentroid(i));
                CvPerson* newPerson = new CvPerson(id, i, contourFinder);
                
                EventArgs args;
                args.person = newPerson;
                args.scene  = scene;
                
                trackedPeople->push_back( newPerson );
                ofNotifyEvent( Events().personEntered, args, this );
            }
        }
        
        // delete old blobs
        for (int i=0; i<trackedPeople->size(); i++){
            Person* p = (*trackedPeople)[i];
            EventArgs args;
            args.person = p;
            args.scene  = scene;
            
            if (p == NULL){
                ofNotifyEvent( Events().personWillLeave, args, this );
                trackedPeople->erase(trackedPeople->begin() + i);
            } else if ( !(tracker.existsPrevious( p->pid ) && tracker.existsCurrent(p->pid)) && !tracker.existsCurrent(p->pid) ){
                ofNotifyEvent( Events().personWillLeave, args, this );
                trackedPeople->erase(trackedPeople->begin() + i);
            }
        }
        return differencedImage.getPixelsRef();
    }
    
    //------------------------------------------------------------------------
    void CvProcessor::processOpticalFlow( ofBaseImage & image ){
        flow.calcOpticalFlow(image);
    }
    
    //------------------------------------------------------------------------
    void CvProcessor::processHaar( ofBaseImage & image ){
        // to-do
    }
    
    //------------------------------------------------------------------------
    void CvProcessor::setOpticalflowMinMax( float min, float max ){
        // ...
    }
    
    //------------------------------------------------------------------------
    void CvProcessor::setHaarXMLFile( string xmlFile ){
        
    }
    
    //------------------------------------------------------------------------
    void CvProcessor::resize( int camWidth, int camHeight ){
        backgroundImage.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
        differencedImage.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
    }
}
