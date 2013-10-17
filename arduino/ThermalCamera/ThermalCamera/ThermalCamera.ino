#include <Wire.h>

char addr = 0x68;
byte pixelTempL;
byte pixelTempH;
int celsius;
String data;

void setup() {
    Wire.begin();
    Serial.begin(115200);
}

void loop() {
    pixelTempL = 0x80;
    pixelTempH = 0x81;
    data = "";
    
    for (int pixel = 0; pixel <= 63; pixel++) {
        Wire.beginTransmission(addr);
        Wire.write(pixelTempL);
        Wire.endTransmission();
        Wire.requestFrom(addr, 1);
        
        byte lowerLevel = Wire.read();
        
        Wire.beginTransmission(addr);
        Wire.write(pixelTempH);
        Wire.endTransmission();
        Wire.requestFrom(addr, 1);
        
        byte upperLevel = Wire.read();
        
        int temperature = ((upperLevel << 8) | lowerLevel);
        
        if (upperLevel != 0) {
            temperature = -(2048 - temperature);
        }
        
        celsius = temperature * 0.25;
        data += celsius;
        if (pixel < 63) data += ",";
        
        pixelTempL = pixelTempL + 2;
        pixelTempH = pixelTempH + 2;
    }
    
    Serial.println(data);
}
