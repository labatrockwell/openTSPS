#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

typedef struct{
    string msg;
    int level;

    string logStr;

    long timestamp;
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int seconds;
}logRecord;

class simpleLogger : public ofBaseDraws{

     public:

        simpleLogger(){
            fileLoaded = false;
            bDate = true;
            bTime = true;
            bLevel = true;
        }

        //------------------------------------------------
        ~simpleLogger(){
            if(fileLoaded){
                saveFile();
            }
        }

        //------------------------------------------------
        void setup(string logFileName, bool overwrite){
            logFile = logFileName;

            fileLoaded = xml.loadFile(logFile);
            if(overwrite){
                xml.clear();
                xml.saveFile(logFile);
            }

            if( !fileLoaded ){
                xml.saveFile(logFile);
                fileLoaded = true;
            }
        }

        void setIncludeDate(bool bIncludeDate){
            bDate = bIncludeDate;
        }

        void setIncludeTime(bool bIncludeTime){
            bTime = bIncludeTime;
        }

        void setIncludeLevelbool(bool  bIncludeLevel){
            bLevel = bIncludeLevel;
        }

        //--------------------------------------------------
        void log(int logLevel, const char* format, ...){
            //thanks stefan!
            //http://www.ozzu.com/cpp-tutorials/tutorial-writing-custom-printf-wrapper-function-t89166.html

            logRecord record;
            record.year     = ofGetYear();
            record.month    = ofGetMonth();
            record.day      = ofGetDay();
            record.hour     = ofGetHours();
            record.minute   = ofGetMinutes();
            record.seconds  = ofGetSeconds();
            record.level    = logLevel;

            char str[2048] = {0};

            va_list args;
            va_start( args, format );
            vsprintf(str, format, args );
            va_end( args );

            record.msg      = str;
            record.logStr   =  convertToString(record);

            logs.push_back(record);
            logToXml(record);
        }

        float getWidth(){
           return 0;
        }

        float getHeight(){
            return 0;
        }

        //----------------------------------------------
        string convertToString(logRecord & record){
            string timeStr;
            string dateStr;
            string levelStr;

            if(bDate) dateStr = ofToString(record.year) +"-"+ofToString(record.month) +"-"+ ofToString(record.day)+" ";
            if(bTime) timeStr = ofToString(record.hour) + ":"+ ofToString(record.minute) + ":"+ ofToString(record.seconds)+" ";

            if( bLevel ){
                if(record.level == OF_LOG_VERBOSE) levelStr = "VERBOSE: ";
                else if(record.level == OF_LOG_NOTICE)levelStr = "NOTICE: ";
                else if(record.level == OF_LOG_WARNING)levelStr = "WARNING: ";
                else if(record.level == OF_LOG_ERROR)levelStr = "ERROR: ";
                else if(record.level == OF_LOG_FATAL_ERROR)levelStr = "FATAL_ERROR: ";
            }

            return dateStr + timeStr + levelStr + record.msg;
        }

        //-----------------------------------------------
        void logToXml(logRecord & record){
            if(!fileLoaded)return;
            xml.addValue("log", record.logStr);
        }

        //-----------------------------------------------.
        void saveFile(){
            xml.saveFile(logFile);
        }

        //-----------------------------------------------.
        void draw(float x, float y){
            ofPushStyle();
                float yPos;
                for(int i = logs.size()-1; i >= 0; i--){
                    yPos += 13.6;
                    string str = logs[i].logStr;
                    ofDrawBitmapString(str, x, y + yPos);
                }
            ofPopStyle();
        }

        //-----------------------------------------------.
        void draw(float x, float y, float width, float height){
            ofPushStyle();
                float yPos;
                for(int i = logs.size()-1; i >= 0; i--){
                    yPos += 13.6;
                    if(yPos >= height)break;

                    string str = logs[i].logStr;
                    if( str.length() * 8> width ){
                        int newLen = (float)width / 8;
                        //newLen = ofClamp(newLen, 1, 999999);
                        str = str.substr(0, newLen);
                    }

                    ofDrawBitmapString(str, x, y + yPos);
                }
            ofPopStyle();
        }

        string logFile;
        ofxXmlSettings xml;
        bool fileLoaded;

        bool bLevel;
        bool bTime;
        bool bDate;

        vector <logRecord> logs;
};
