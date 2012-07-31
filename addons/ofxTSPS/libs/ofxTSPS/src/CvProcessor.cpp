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
    ofPixels blackPixelsSmall;
    
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
        bFlowTrackedOnce    = false;
        
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
        flow.setNumIterations( 1 );
        flow.setPolyN( 7 );
        flow.setPolySigma( 1.5 );
        flow.setUseGaussian( false );    
    }
    
    //------------------------------------------------------------------------
    void CvProcessor::exit(){}
    
    //------------------------------------------------------------------------
    void CvProcessor::draw(){
        if ( bTrackOpticalFlow && bFlowTrackedOnce ){
            // why isn't this working?
            cameraSmallImage.draw(0,0, tspsWidth, tspsHeight);
            flow.draw(0,0,tspsWidth, tspsHeight);
        }
        
        /*ofPushMatrix();
        ofScale( 1.0f/trackingScale, 1.0f/trackingScale);
        
        for(int j = 0; j < haarObjects.size(); j++) {
            ofRectangle hr = toOf(haarObjects[j]);
            ofRect( hr );
        }
        
        ofPopMatrix();*/
    }
    
    //------------------------------------------------------------------------
    void CvProcessor::setCameraImage( ofBaseImage & image ){
        cameraImage.setFromPixels(image.getPixelsRef());
        cameraImage.update();
		Mat srcMat = toCv(cameraImage), dstMat = toCv(cameraSmallImage);
		//cvtColor(srcMat, srcMatGray, CV_RGB2GRAY);
        cv::resize(srcMat, dstMat, dstMat.size(), 0, 0, INTER_NEAREST);
        ofPixels pix;
        toOf(dstMat, pix);
        cameraSmallImage.setFromPixels(pix);
        cameraSmallImage.update();
    }
    
    //------------------------------------------------------------------------
    void CvProcessor::captureBackground( ofBaseImage & image ){
        backgroundImage.setFromPixels( image.getPixelsRef() );
        progressiveBackgroundImage.setFromPixels( image.getPixelsRef() );
    }
    
    //------------------------------------------------------------------------
    ofPixelsRef CvProcessor::progressiveBackground( ofBaseImage & image, float amount ){
        ofxCv::lerp(image, progressiveBackgroundImage, progressiveBackgroundImage, amount);
        //cv::addWeighted( toCv(backgroundImage), amount, toCv(progressiveBackgroundImage), 1.0f-amount,0, toCv(progressiveBackgroundImage) );
        backgroundImage = progressiveBackgroundImage;
        return backgroundImage.getPixelsRef();
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
        if ( bTrackHaar ){
            processHaar( cameraSmallImage );
        }
        
        if ( bTrackOpticalFlow ){
            processOpticalFlow( cameraSmallImage );
        }
        
        differencedImage.setFromPixels(image.getPixelsRef());
        ofxCv::threshold(differencedImage, threshold);
        
        //reset scene
        scene->percentCovered = 0;
        if ( bTrackOpticalFlow && bFlowTrackedOnce ){
            scene->averageMotion = flow.getAverageFlow();
        } else {
            scene->averageMotion = ofPoint(0,0);
        }
        
        // find contours
        contourFinder.setMinArea( minBlobArea * tspsWidth * tspsHeight );
        contourFinder.setMaxArea( maxBlobArea * tspsWidth * tspsHeight );
        contourFinder.findContours( differencedImage );
        
        // update people
        RectTracker& tracker = contourFinder.getTracker();
        cv::Mat cameraMat = toCv(cameraImage);
        
        //optical flow scale
        // float flowROIScale = tspsWidth/flow.getWidth();
        
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
                cv::Point minLoc, maxLoc;
                double minVal = 0, maxVal = 0;
                cv::Rect rect;
                rect.x      = p->boundingRect.x;
                rect.y      = p->boundingRect.y;
                rect.width  = p->boundingRect.width;
                rect.height = p->boundingRect.height;
                cv::Mat roiMat(cameraMat, rect);
                cv::minMaxLoc( roiMat, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
                
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
                ofRectangle roi = p->getBoundingRectNormalized(tspsWidth, tspsHeight);
                roi.x *= flow.getWidth();
                roi.y *= flow.getHeight();
                roi.width *= flow.getWidth();
                roi.height *= flow.getHeight();                
                
                // sum optical flow for the person
                if ( bTrackOpticalFlow && bFlowTrackedOnce ){
                    // TO-DO!
                    p->opticalFlowVectorAccumulation = flow.getAverageFlowInRegion(roi);
                } else {
                    p->opticalFlowVectorAccumulation.x = p->opticalFlowVectorAccumulation.y = 0;
                }
                
                //detect haar patterns (faces, eyes, etc)
                if ( bTrackHaar ){
                    //find the region of interest, expanded by haarArea.
                    ofRectangle haarROI;
                    haarROI.x		= fmax( (p->boundingRect.x - haarAreaPadding/2) * trackingScale, 0.0f );
                    haarROI.y		= fmax( (p->boundingRect.y - haarAreaPadding/2) * trackingScale, 0.0f );
                    haarROI.width	= fmin( (p->boundingRect.width  + haarAreaPadding*2) * trackingScale, cameraSmallImage.width );
                    haarROI.height	= fmin( (p->boundingRect.height + haarAreaPadding*2) * trackingScale, cameraSmallImage.height );
                    
                    bool haarThisFrame = false;
                    for(int j = 0; j < haarObjects.size(); j++) {
                        ofRectangle hr = toOf(haarObjects[j]);
                        
                        //check to see if the haar is contained within the bounding rectangle
                        if(hr.x > haarROI.x && hr.y > haarROI.y && hr.x+hr.width < haarROI.x+haarROI.width && hr.y+hr.height < haarROI.y+haarROI.height){
                            hr.x /= trackingScale;
                            hr.y /= trackingScale;
                            hr.width /= trackingScale;
                            hr.height /= trackingScale;
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
        bFlowTrackedOnce = true;
    }
    
    //------------------------------------------------------------------------
    void CvProcessor::processHaar( ofBaseImage & image ){
        // don't really need the image here, huh?        
		Mat graySmallMat = toCv(image);
        //equalizeHist(graySmallMat, graySmallMat);        
		haarFinder.detectMultiScale(graySmallMat, haarObjects, 1.06, 1,
                                    //CascadeClassifier::DO_CANNY_PRUNING |
                                    //CascadeClassifier::FIND_BIGGEST_OBJECT |
                                    //CascadeClassifier::DO_ROUGH_SEARCH |
                                    0);
    }
    
    //------------------------------------------------------------------------
    void CvProcessor::setOpticalflowMinMax( float min, float max ){
        // ...
    }
    
    //------------------------------------------------------------------------
    void CvProcessor::setHaarXMLFile( string xmlFile ){
        haarFinder.load(ofToDataPath(xmlFile));        
    }
    
    //------------------------------------------------------------------------
    void CvProcessor::resize( int camWidth, int camHeight ){
        cameraImage.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
        cameraLastImageSmall.allocate((int) camWidth * trackingScale, (int) camHeight * trackingScale, OF_IMAGE_GRAYSCALE);
        cameraSmallImage.allocate((int) camWidth * trackingScale, (int) camHeight * trackingScale, OF_IMAGE_GRAYSCALE);
        
        cout<<cameraSmallImage.width <<":"<<cameraSmallImage.height<<endl;
        
        blackPixelsSmall = ofPixels();
        blackPixelsSmall.allocate(camWidth * trackingScale, camHeight * trackingScale, 1);
        blackPixelsSmall.set(0);
        //cameraSmallImage.setFromPixels(blackPixelsSmall);
        
        backgroundImage.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
        differencedImage.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
        progressiveBackgroundImage.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
    }
}
