#include "ThermalMesh.h"

vector<int> data;

void ThermalMesh::setup() {
    ofBackground(0);
    ofEnableAlphaBlending();
    
    gradient.addColor(ofColor::black);
    gradient.addColor(ofColor::blue);
    gradient.addColor(ofColor::red);
    gradient.addColor(ofColor::yellow);
    gradient.addColor(ofColor::lightYellow);
    
    serial.listDevices();
    serial.setup(0, 115200);
    
    size = 80;
    row = 8;
    r = ofRectangle(0, 0, 300, 200);
    rot = 0.;
    
    gui = new ofxUICanvas();
    gui->setPosition(ofGetWidth() - 211, 20);
    gui->addSlider("Low Temp C", 1, 60, &tempL);
    gui->addSlider("High Temp C", 1, 60, &tempH);
    gui->setTheme(OFX_UI_THEME_BLUEBLUE);
    gui->autoSizeToFitWidgets();
    gui->loadSettings("settings.xml");
}

void ThermalMesh::update() {
    if (!serial.available()) return;
    
    string dataString = ofxGetSerialString(serial, '\n');
    if (dataString != "") {
        vector<string> dataTemp = ofSplitString(dataString, ",");
        data.clear();
        for (int i = 0; i < dataTemp.size(); i++) {
            data.push_back(atoi(dataTemp[i].c_str()));
        }
    }
}

void ThermalMesh::draw() {
    ofPushView();
    if (data.size() > 0) {
        ofPoint p = ofPoint();
        int min = 10;
        int max = 32;
        float r, g, b;
        
        glEnable( GL_BLEND );
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glScalef(size, size, 0);
        glTranslatef(p.x * size, p.y * size, 1);
        
        for (int i = data.size() - row - 2; i >= 0; i--) {
            ofColor c0 = gradient.getColorAtPercent(ofMap(data[i + row + 1], tempL, tempH, 0, 1));
            ofColor c3 = gradient.getColorAtPercent(ofMap(data[i + 1], tempL, tempH, 0, 1));
            ofColor c1 = gradient.getColorAtPercent(ofMap(data[i + row], tempL, tempH, 0, 1));
            ofColor c2 = gradient.getColorAtPercent(ofMap(data[i], tempL, tempH, 0, 1));
            
            if ((i + 1) % 8 != 0 || i == 0) {
                glBegin(GL_QUADS);
                glPushMatrix();
                
                r = ofMap((float)c0.r, 0, 255, 0, 1);
                g = ofMap((float)c0.g, 0, 255, 0, 1);
                b = ofMap((float)c0.b, 0, 255, 0, 1);
                glColor3f(r, g, b);
                glVertex3f(p.x, p.y, 0);
                
                r = ofMap((float)c1.r, 0, 255, 0, 1);
                g = ofMap((float)c1.g, 0, 255, 0, 1);
                b = ofMap((float)c1.b, 0, 255, 0, 1);
                glColor3f(r, g, b);
                glVertex3f(1 + p.x, p.y, 0);
                
                r = ofMap((float)c2.r, 0, 255, 0, 1);
                g = ofMap((float)c2.g, 0, 255, 0, 1);
                b = ofMap((float)c2.b, 0, 255, 0, 1);
                glColor3f(r, g, b);
                glVertex3f(1 + p.x, 1 + p.y, 0);
                
                r = ofMap((float)c3.r, 0, 255, 0, 1);
                g = ofMap((float)c3.g, 0, 255, 0, 1);
                b = ofMap((float)c3.b, 0, 255, 0, 1);
                glColor3f(r, g, b);
                glVertex3f(p.x, 1 + p.y, 0);
                
                glPopMatrix();
                glEnd();
                glFlush();
            }
            
            if (p.x / (row - 1) == 1 && p.x != 0) {
                p.y ++;
                p.x = 0;
            } else {
                p.x ++;
            }
        }
    }
    ofPopView();
    
    gradient.drawDebug(ofGetWidth() - 160, 0, 160, 20);
}

void ThermalMesh::exit() {
    serial.close();
}