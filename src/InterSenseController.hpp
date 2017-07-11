#pragma once 

#include "ofMain.h"
#include "Const.hpp"
#include "ofThread.h"

class InterSenseControllerThread : public ofThread{
    
public:
    InterSenseControllerThread(){
        
        if(!serial.setup(INERTIA_CUBE_PATH, 115200)){
            ofLog(OF_LOG_ERROR) << "could not open serial port";
        }else{
            ofLog() << "opened serial port";
        }
    }
    
    
    void threadedFunction(){
        if(!serial.isInitialized()){
            ofLog() << "Serial device unavailable. abort thread.";
            return;
        }
        while(isThreadRunning()){
            while(serial.available() > 0){
                int byte = serial.readByte();
                switch ( byte){
                    case OF_SERIAL_NO_DATA:{
                        ofLog(OF_LOG_WARNING) << "no data from serial";
                    }
                    case OF_SERIAL_ERROR:{
                        ofLog(OF_LOG_ERROR) << "serial error";
                    }
                    default :{
                        interpret(byte);
                    }
                }
            }
        }
    }
    

    int getFrontBack() {
        lock();
        float fb = frontBack;
        unlock();
        return fb;
    }
    
    int getLeftRight() {
        lock();
        float lr = leftRight;
        unlock();
        return lr;
    }
    
    void startStream(){
        
        std::array<unsigned char, 5> configurationA = {{165, 2, 32, 4, 53}};
        std::array<unsigned char, 5> configurationB = {{165, 2, 33, 0, 56}};

        serial.writeBytes(&configurationA[0], 5);
        serial.writeBytes(&configurationB[0], 5);

        std::array<unsigned char, 3> startCommand = {{165, 5, 86}};
        serial.writeBytes(&startCommand[0], 3);
        startThread(true);
    }
    
    void stopStream(){
        stopThread();
        std::array<unsigned char, 3> stopCommand = {{165, 0, 91}};
        serial.writeBytes(&stopCommand[0], 3);
    }
    
    void close(){
        serial.close();
    }
    
private:
    
    void interpret(int byte){
        static int previous = -1;
        if(byte == 100 && previous == 5){
            if(validate()){
                float fb = gate(scale(toSignedNumber((stack[2] << 8) + stack[3])));
                float lr = gate(scale(toSignedNumber((stack[4] << 8) + stack[5])));
                
                // critical session
                lock();
                frontBack += (fb - frontBack) * 0.1;
                leftRight += (lr - leftRight) * 0.1;
                unlock();
                // critial session
            }
            
            stack.clear();
            
        }else{
            stack.push_back(byte);
        }
        
        previous = byte;
    }
    
    float gate(float input){
        bool neg = input < 0.0;
        float abs = fabs(input);
        if(abs > 2.0){
            abs = abs - 2.0;
            abs = neg ? abs * -1.0 : abs;
            return abs;
        }else{
            return 0;
        }
    }
    
    float scale(int num){
        return -static_cast<float>(num) / 100.0;
    }
    
    int toSignedNumber(int num) const{
        return num < 32767 ? num : num - 65535;
    }
    
    bool validate(){
        int sum = 0;
        for(int i = 0; i <= 6; i++){
            sum += stack[i];
        }
        sum += 105;
        sum %= 256;
        int checkSum =  256 - sum;
        
        return checkSum == stack[7];
    }


    std::vector<int> stack;
    ofSerial serial;
    
    // danger zone
    // we have to lock the thread, when touching these two parameters
    float frontBack; // front positive
    float leftRight; // right positive
};

