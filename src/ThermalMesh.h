#pragma once

#include "ofMain.h"
#include "ofxColorGradient.h"
#include "ofxUI.h"
#include "Utils.h"

class ThermalMesh : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
private:
    ofSerial serial; // Serial Connection
    ofxColorGradient gradient; // Convert Temp to Color
    vector<int> data; // Pixel Data
    ofxUICanvas *gui; // Settings GUI
    
    int size;
    int row;
    float tempL;
    float tempH;
    
    ofRectangle r;
    float rot;
    ofImage bg;

};