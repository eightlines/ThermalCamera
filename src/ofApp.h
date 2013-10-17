#pragma once

#include "ofMain.h"
#include "ofxColorGradient.h"
#include "ofxUI.h"
#include "Utils.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
    void exit();

private:
    ofSerial serial; // Serial Connection
    
    ofPoint offset; // Drawing offset
    ofxColorGradient gradient; // Convert Temp to Color
    
    vector<int> pixels; // Grid-EYE Pixel Values
    int pixelH;
    int pixelL;
    int size; // Pixel Size
    
    float tempL;
    float tempH;
    float tempHighlight;
    float pixelHVal;
    float pixelLVal;
    
    bool bPixelIds;
    bool bDynamicThreshold;
    bool bShowRange;
    bool bTempOutline;
    bool bHideLowTemp;
    bool bShowLowestPixelTemp;
    bool bShowHighestPixelTemp;
    
    ofxUICanvas *gui; // Settings GUI

};
