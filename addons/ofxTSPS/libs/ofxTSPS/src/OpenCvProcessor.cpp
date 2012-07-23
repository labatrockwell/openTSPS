/*
 *  OpenCvProcessor.cpp
 *  openTSPS
 */

#include "ofxTSPS/opencv/OpenCvProcessor.h"

namespace ofxTSPS {
    //--------------------------------------------------------------
    OpenCvProcessor::OpenCvProcessor(){
        // capabilities
        bCanTrackHaar       = true;
        bCanTrackContours   = true;
        bCanTrackSkeleton   = false;
        bCanTrackOpticalFlow = true;
        
        // default settings
        bTrackHaar = false;
        bTrackContours = true;
        bTrackSkeleton = false;
        bTrackOpticalFlow = false;
        
        setThreshold();
        setBlobSettings();
        setOpticalflowMinMax();
        setHaarPadding();
    }
    
    //--------------------------------------------------------------
    void OpenCvProcessor::setupProcessor(){    
        //allocate images
        resize( tspsWidth, tspsHeight );
        
        opticalFlow.setCalcStep(5,5);
        opticalFlowLastImage = opticalFlowImage;	
        
        // setup blob tracker
        persistentTracker.setListener( this );
    }
    
    //--------------------------------------------------------------
    void OpenCvProcessor::draw(){
        if ( bTrackOpticalFlow ){
            ofSetColor(34,151,210);
            opticalFlow.draw( tspsWidth, tspsHeight );
        }
    }
    
    //--------------------------------------------------------------
    void OpenCvProcessor::setCameraImage( ofBaseImage & image ){
        grayImg.setFromPixels(image.getPixels(), image.getWidth(), image.getHeight());
    }
    
    //--------------------------------------------------------------
    void OpenCvProcessor::captureBackground( ofBaseImage & image ){
        floatBgImg.setFromPixels(image.getPixels(), image.getWidth(), image.getHeight());
        grayBg.setFromPixels(image.getPixels(), image.getWidth(), image.getHeight());
    }
    
    //--------------------------------------------------------------
    void OpenCvProcessor::progressiveBackground( ofBaseImage & image, float amount ){
        dummyImage.setFromPixels(image.getPixels(), image.getWidth(), image.getHeight());
        floatBgImg.addWeighted( dummyImage, amount);
        grayBg = floatBgImg;
    }
    
    //--------------------------------------------------------------
    void OpenCvProcessor::blankBackground(){
        grayBg -= grayBg;
    }
    
    //--------------------------------------------------------------
    ofPixelsRef OpenCvProcessor::difference( ofBaseImage & trackingImage, TrackingType trackingType ){
        grayDiff.setFromPixels(trackingImage.getPixels(), trackingImage.getWidth(), trackingImage.getHeight());
        
        // subtract background
        if(trackingType == TRACK_ABSOLUTE){
            grayDiff.absDiff( grayBg, grayDiff );
        } else {
            if(trackingType == TRACK_LIGHT){
                cvSub(grayBg.getCvImage(), grayDiff.getCvImage(), grayDiff.getCvImage());
            } else if(trackingType == TRACK_DARK){ 
                cvSub(grayDiff.getCvImage(), grayBg.getCvImage(), grayDiff.getCvImage());
            }
            grayDiff.flagImageChanged();
        }
        
        return grayDiff.getPixelsRef();
    }
    
    //--------------------------------------------------------------
    ofPixelsRef OpenCvProcessor::process( ofBaseImage & trackingImage ){
        grayDiff.setFromPixels(trackingImage.getPixels(), trackingImage.getWidth(), trackingImage.getHeight());
        
        // reset scene
        scene->percentCovered = 0;
        
        if ( bTrackHaar ){
            processHaar( trackingImage );
        }
        
        if ( bTrackOpticalFlow ){
            processOpticalFlow( trackingImage );
        }
        
        //threshold	
        grayDiff.threshold( threshold );
        
        // find contours + track blobs
        contourFinder.findContours( grayDiff, minBlobArea, maxBlobArea, 50, bFindHoles );
        persistentTracker.trackBlobs(contourFinder.blobs);
        
        for(int i = 0; i < persistentTracker.blobs.size(); i++){
            ofxCvTrackedBlob blob = persistentTracker.blobs[i];
            OpenCvPerson* p = (OpenCvPerson*) getTrackedPerson(blob.id);
            //somehow we are not tracking this person, safeguard (shouldn't happen)
            if(NULL == p){
                ofLog(OF_LOG_WARNING, "ofxPerson::warning. encountered persistent blob without a person behind them\n");
                continue;
            }
            
            scene->percentCovered += blob.area;
            
            //update this person with new blob info
            p->update( blob, true );
            
            //normalize simple contour
            for (int i=0; i<p->simpleContour.size(); i++){
                p->simpleContour[i].x /= tspsWidth;
                p->simpleContour[i].y /= tspsHeight;
            }
            
            //find peak in blob (only useful with Kinect)
            CvPoint minLoc, maxLoc;
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
            }
            
            // ROI for opticalflow
            ofRectangle roi = ofRectangle( p->boundingRect );
            roi.x *= trackingScale;
            roi.y *= trackingScale;
            roi.width *= trackingScale;
            roi.height *= trackingScale;
            
            // sum optical flow for the person
            if ( bTrackOpticalFlow ){
                p->opticalFlowVectorAccumulation = opticalFlow.flowInRegion(roi);
            } else {
                p->opticalFlowVectorAccumulation.x = p->opticalFlowVectorAccumulation.y = 0;
            }
            
            //detect haar patterns (faces, eyes, etc)
            if ( bTrackHaar ){
                // ROI for haar
                ofRectangle roi;
                roi.x		= fmax( (p->boundingRect.x - haarAreaPadding) * trackingScale / 2, 0.0f );
                roi.y		= fmax( (p->boundingRect.y - haarAreaPadding) * trackingScale / 2, 0.0f );
                roi.width	= fmin( (p->boundingRect.width  + haarAreaPadding*2) * trackingScale / 2, haarImage.width - roi.x );
                roi.height	= fmin( (p->boundingRect.height + haarAreaPadding*2) * trackingScale / 2, haarImage.height - roi.y );	
                
                //find the region of interest, expanded by haarArea.
                bool haarThisFrame = false;
                vector<ofRectangle> haarRects = haarTracker.getCurrentHaarRects();
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
                }
                if(!haarThisFrame){
                    p->noHaarThisFrame();
                }
            }
            
            EventArgs args;
            args.person = p;
            args.scene  = scene;
            
            ofNotifyEvent( Events().personUpdated, args, this );
        }
        
        // reset roi of image
        grayImg.setROI(0,0,tspsWidth,tspsHeight);
        
        // update scene
        scene->percentCovered /= tspsWidth*tspsHeight;
        
        return grayDiff.getPixelsRef();
    }
    
    //--------------------------------------------------------------
    void OpenCvProcessor::processOpticalFlow( ofBaseImage & trackingImage ){
        dummyImage.setFromPixels(trackingImage.getPixels(), trackingImage.getWidth(), trackingImage.getHeight());
        opticalFlowImage.scaleIntoMe( dummyImage );
        opticalFlow.calc(opticalFlowLastImage, opticalFlowImage, 11);
        
        scene->averageMotion = opticalFlow.flowInRegion(0,0,tspsWidth,tspsHeight);
        
        //store the old image
        opticalFlowLastImage = opticalFlowImage;
    }
    
    //--------------------------------------------------------------
    void OpenCvProcessor::processHaar( ofBaseImage & trackingImage ){
        dummyImage.setFromPixels(trackingImage.getPixels(), trackingImage.getWidth(), trackingImage.getHeight());
        haarImage.scaleIntoMe( dummyImage );
        haarTracker.findHaarObjects( haarImage );
    }
    
    //--------------------------------------------------------------
    void OpenCvProcessor::resize( int width, int height ){
        tspsWidth   = width;
        tspsHeight  = height;
        
        grayImg.allocate( tspsWidth, tspsHeight );
        grayDiff.allocate( tspsWidth, tspsHeight );
        grayBg.allocate(tspsWidth, tspsHeight);
        floatBgImg.allocate(tspsWidth, tspsHeight);
        dummyImage.allocate(tspsWidth, tspsHeight);
        opticalFlowImage.allocate( tspsWidth*trackingScale, tspsHeight*trackingScale );	
        opticalFlowLastImage.allocate( tspsWidth*trackingScale, tspsHeight*trackingScale );
        haarImage.allocate( tspsWidth*trackingScale/2, tspsHeight*trackingScale/2 );
        
        //set up optical flow
        opticalFlow.allocate( tspsWidth*trackingScale, tspsHeight*trackingScale );
    }
    
    //--------------------------------------------------------------
    void OpenCvProcessor::setOpticalflowMinMax( float min, float max ){
        opticalFlow.maxVector = max;
        opticalFlow.minVector = min;
    }
    
    //--------------------------------------------------------------
    void OpenCvProcessor::setHaarXMLFile(string haarFile){
        haarFinder.setup(haarFile);
        haarTracker.setup(&haarFinder);
    }
    
    //---------------------------------------------------------------------------
    void OpenCvProcessor::blobOn( int x, int y, int id, int order )
    {
        ofxCvTrackedBlob blob = persistentTracker.getById( id );
        OpenCvPerson* newPerson = new OpenCvPerson(id, order, blob);
        
        EventArgs args;
        args.person = newPerson;
        args.scene  = scene;
        
        trackedPeople->push_back( newPerson );
        
        ofNotifyEvent( Events().personEntered, args, this );
    }
    
    //---------------------------------------------------------------------------
    void OpenCvProcessor::blobMoved( int x, int y, int id, int order ){/*not used*/}
    
    //---------------------------------------------------------------------------
    void OpenCvProcessor::blobOff( int x, int y, int id, int order )
    {
        OpenCvPerson* p = (OpenCvPerson*) getTrackedPerson(id);
        //ensure we are tracking
        if(NULL == p){
            ofLog(OF_LOG_WARNING, "ofxPerson::warning. encountered persistent blob " + ofToString( id ) +" without a person behind them\n");		
            return;
        }
        
        //delete the object and remove it from the vector
        std::vector<Person*>::iterator it;
        for(it = trackedPeople->begin(); it != trackedPeople->end(); it++){
            if((*it)->pid == p->pid){
                trackedPeople->erase(it);
                break;
            }
        }
        
        EventArgs args;
        args.person = p;
        args.scene  = scene;
        ofNotifyEvent( Events().personWillLeave, args, this );
        
        // delete pointer
        delete p;
    }
}
