#include "ofMain.h"
#include "ofApp.h"
#include "ThermalMesh.h"

int main() {
	ofSetupOpenGL(900, 500, OF_WINDOW);
//	ofRunApp(new ofApp()); // GRID VIEW
	ofRunApp(new ThermalMesh()); // MESH VIEW
}
