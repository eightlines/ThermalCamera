#include "ofApp.h"

void ofApp::setup() {
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetFrameRate(120);
    
    for (int i = 0; i < 64; i++)
        pixels.push_back(0);
    
    size = 26;
    bDynamicThreshold = false;
    bPixelIds = true;
    bShowRange = true;
    tempL = 15;
    tempH = 30;
    
    gui = new ofxUICanvas(0, 0, size * 8, size * 8);
    gui->setPosition(0, size * 8 + 10);
    gui->addToggle("Use Dynamic Threshold", &bDynamicThreshold);
    gui->addToggle("Use Outline", &bTempOutline);
    gui->addToggle("Hide Low Threshold", &bHideLowTemp);
    gui->addToggle("Show Pixel IDs", &bPixelIds);
    gui->addToggle("Show Lowest Pixel Temp", &bShowLowestPixelTemp);
    gui->addToggle("Show Highest Pixel Temp", &bShowHighestPixelTemp);
    gui->addSlider("Low Temp C", 1, 60, &tempL);
    gui->addSlider("High Temp C", 1, 60, &tempH);
    gui->addSlider("Highlight Temp C", 1, 60, &tempHighlight);
    gui->autoSizeToFitWidgets();
    gui->loadSettings("settings.xml");
    
    gradient.addColor(ofColor::black);
    gradient.addColor(ofColor::darkRed);
    
    serial.listDevices();
    serial.setup(0, 115200);
}

void ofApp::update() {
    ofSetWindowTitle("FPS " + ofToString(ofGetFrameRate()));
    
    if (!serial.available())
        return;
    
    string data = ofxGetSerialString(serial, '\n');
    //    cout << data << endl;
    if (data != "") {
        vector<string> _pixels = ofSplitString(data, ",");
        pixels.clear();
        for (int i = 0; i < _pixels.size(); i++)
            pixels.push_back(atoi(_pixels[i].c_str()));
        
        pixelH = -1;
        pixelHVal = 0;
        pixelLVal = 100;
        for (int i = pixels.size() - 1; i >= 0; i--) {
            if (pixels[i] > pixelHVal) {
                pixelH = i;
                pixelHVal = pixels[i];
            }
            
            if (pixelLVal > pixels[i]) {
                pixelL = i;
                pixelLVal = pixels[i];
            }
        }
        
        if (bDynamicThreshold) {
            tempL = pixelLVal;
            tempH = pixelHVal;
        }
    }
}

void ofApp::draw() {
    ofBackgroundGradient(ofColor::grey, ofColor::black);
    
    offset = ofPoint();
    for (int i = pixels.size() - 1; i >= 0; i--) {
        float pixelTemp = ofMap(pixels[i], tempL, tempH, 0, 1); // Map ¼C to %
        ofRectangle r = ofRectangle((size * 7) - (offset.x * size), offset.y * size, size, size);
        
        if (bHideLowTemp) {
            if (pixels[i] >= tempHighlight) {
                ofSetColor(gradient.getColorAtPercent(pixelTemp));
                ofRect(r);
            }
        } else {
            ofSetColor(gradient.getColorAtPercent(pixelTemp));
            ofRect(r);
        }
        
        if (bTempOutline &&
            pixels[i] >= tempHighlight) {
            ofSetColor(ofColor::whiteSmoke);
            ofRect(r);
        }
        
        if (i % 8 == 0 && i > 0) {
            offset.y ++;
            offset.x = 0;
        } else {
            offset.x ++;
        }
    }
    
    gradient.drawDebug(0, size * 8, size * 8, 10);
    
    if (bPixelIds) {
        offset = ofPoint();
        for (int i = pixels.size() - 1; i >= 0; i--) {
            ofSetColor((bShowHighestPixelTemp && pixelH == i) ? 255 : (bShowLowestPixelTemp && pixelL == i) ? 128 : 0);
            ofDrawBitmapString(ofToString(i), ofPoint((size * 7) - offset.x * size + 3, offset.y * size + 15));
            
            if (i % 8 == 0 && i > 0) {
                offset.y ++;
                offset.x = 0;
            } else {
                offset.x ++;
            }
        }
    }
}

void ofApp::exit() {
    serial.close();
    gui->saveSettings("settings.xml");
}
