#include "ofxCvCoordWarping.h"

static bool pnpoly(int npol, ofPoint * poly, float x, float y){
	int i, j = 0;
	bool c = false;
	for (i = 0, j = npol-1; i < npol; j = i++) {
		if ((((poly[i].y<=y) && (y < poly[j].y)) ||
			 ((poly[j].y<=y) && (y < poly[i].y))) &&
			(x < (poly[j].x - poly[i].x) * (y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x))
			c = !c;
	}
	return c;
}


//---------------------------
ofxCvCoordWarping::ofxCvCoordWarping(){
	translate = cvCreateMat(3,3,CV_32FC1);
}

//---------------------------
void ofxCvCoordWarping::calculateMatrix(ofPoint src[4], ofPoint dst[4]){
	
	cvSetZero(translate);
	for (int i = 0; i < 4; i++){
		cvsrc[i].x = src[i].x;
		cvsrc[i].y = src[i].y;
		cvdst[i].x = dst[i].x;
		cvdst[i].y = dst[i].y;
		
		srcQuad[i] = src[i];
		dstQuad[i] = dst[i];
	}
	
	
	cvWarpPerspectiveQMatrix(cvsrc, cvdst, translate);  // calculate homography
	
	int n       = translate->cols;
	float *data = translate->data.fl;
	
}

//---------------------------		
bool ofxCvCoordWarping::bInQuad(ofPoint pt){
	return pnpoly(4, srcQuad, pt.x, pt.y);
}

//---------------------------		
ofPoint ofxCvCoordWarping::transform(float xIn, float yIn){
	
	ofPoint out;
	
	float *data = translate->data.fl;
	
	float a = data[0];
	float b = data[1];
	float c = data[2];
	float d = data[3];
	
	float e = data[4];
	float f = data[5];
	float i = data[6];
	float j = data[7];
	
	//from Myler & Weeks - so fingers crossed!
	out.x = ((a*xIn + b*yIn + c) / (i*xIn + j*yIn + 1));
	out.y = ((d*xIn + e*yIn + f) / (i*xIn + j*yIn + 1));
	
	return out;
}		
