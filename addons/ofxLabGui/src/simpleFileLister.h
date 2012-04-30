#pragma once

#include "ofMain.h"

typedef struct{
    string filename;
    string fullpath;
}entry;

//bool sortEntries(entry & e1, entry & e2){
//    if( e1
//}

class simpleFileLister : public ofDirectory{

     public:

        simpleFileLister(){
            bRevSort        = false;
            selectedChanged = false;
            selected        = 0;
            lastDirectory   = "";
        }

        int refreshDir(){

            if( lastDirectory != ""){
                 int num =  simpleFileLister::listDir(lastDirectory);
                 return num;
            }
            else return 0;
        }

        int listDir(string directory){
            bRevSort        = false;
            entries.clear();

            ofDirectory::reset();
            int numFiles = ofDirectory::listDir(directory);
            entries.assign(numFiles, entry());

            for(int i = 0; i < numFiles; i++){
                entries[i].filename = ofDirectory::getName(i);
                entries[i].fullpath = ofDirectory::getPath(i);
            }

            lastDirectory = directory;
            return entries.size();
        }

        void reverseOrder(){
            if( entries.size() ){
                reverse(entries.begin(), entries.end());
                bRevSort = !bRevSort;
            }
        }

        bool selectedHasChanged(){
            return selectedChanged;
        }

        void clearChangedFlag(){
            selectedChanged = false;
        }

        //------------------------------------------------
        string getName(int which){
            if( which >= 0 && which < entries.size() ){
                return entries[which].filename;
            }else{
                return "";
            }
        }

        //------------------------------------------------
        string getPath(int which){
            if( which >= 0 && which < entries.size() ){
                return entries[which].fullpath;
            }else{
                return "";
            }
        }

        //------------------------------------------------
        void setSelectedFile(int which){
            if( which >= 0 && which < entries.size() ){
                selected    = which;
                selectedChanged = true;
            }
        }

        //------------------------------------------------
        string getSelectedName(){
            if( selected >= 0 && selected < entries.size() ){
                return entries[selected].filename;
            }else{
                return "";
            }
        }

        //------------------------------------------------
        string getSelectedPath(){
            if( selected >= 0 && selected < entries.size() ){
                return entries[selected].fullpath;
            }else{
                return "";
            }
        }

        int  selected;
        bool selectedChanged;
        bool bRevSort;

        string lastDirectory;

        vector <entry> entries;
};
