#pragma once


class guiValue{

    public:
        guiValue(){

        }

       //------------------------------------------------
        void addValue(float val, float _min, float _max){
            value.push_back(val);
            valueI.push_back((int)val);
            valueB.push_back((bool)val);
            min.push_back(_min);
            max.push_back(_max);
            pct.push_back( 0.0 );

            //update our pct
            updatePct(value.size()-1);
        }

        //------------------------------------------------
        void addValueI(int val, int _min, int _max){
            value.push_back((float)val);
            valueI.push_back(val);
            valueB.push_back((bool)val);
            min.push_back((float)_min);
            max.push_back((float)_max);
            pct.push_back( 0.0 );

            //update our pct
            updatePct(value.size()-1);
        }

        //------------------------------------------------
        void addValueB(bool val){
            value.push_back((float)val);
            valueI.push_back((int)val);
            valueB.push_back(val);
            min.push_back(0);
            max.push_back(1);
            pct.push_back( 0.0 );

            //update our pct
            updatePct(value.size()-1);
        }

       //------------------------------------------------
        bool setValue(float val, int which = 0){
            if(which >= 0 && which < value.size() ){

                if( max[which] - min[which] == 0 ) return false;

                //check out bounds
                if(val < min[which])        val = min[which];
                else if(val > max[which])   val = max[which];

                value[which]  =  val;
                valueI[which] = (int)val;
                valueB[which] = (bool)val;

                //recalulate our pct
                updatePct(which);

                return true;
            }
            return false;
        }

        //------------------------------------------------
        bool setValueAsPct(float percent, int which = 0){
            if(which >= 0 && which < value.size() ){

                if( max[which] - min[which] == 0 ) return false;

                if(percent < 0.0) percent = 0.0;
                else if(percent > 1.0) percent = 1.0;

                pct[which]    =  percent;

                value[which]  =  ( pct[which] * ( max[which] - min[which] ) ) + min[which];
                valueI[which] = (int)value[which];
                valueB[which] = (bool)value[which];

                return true;
            }
            return false;
        }

       //------------------------------------------------
        float getValueF(int which = 0){
            if(which >= 0 && which < value.size() ){
                return value[which];
            }
            return 0.0;
        }

       //------------------------------------------------
        float getValueI(int which = 0){
            if(which >= 0 && which < valueI.size() ){
                return valueI[which];
            }
            return false;
        }

       //------------------------------------------------
        float getValueB(int which = 0){
            if(which >= 0 && which < valueB.size() ){
                return valueB[which];
            }
            return false;
        }

        //------------------------------------------------
        float getMin(int which = 0){
            if(which >= 0 && which < min.size() ){
                return min[which];
            }
            return 0.0;
        }

        //------------------------------------------------
        float getMax(int which = 0){
            if(which >= 0 && which < max.size() ){
                return max[which];
            }
            return 0.0;
        }

        //------------------------------------------------
        float getPct(int which = 0){
            if(which >= 0 && which < pct.size() ){
                return pct[which];
            }
            return 0.0;
        }

        //------------------------------------------------
        int getNumValues(){
            return value.size();
        }

    protected:

        //-----------------------------------------------
        void updatePct(int which){
            pct[which] = (value[which] - min[which]) / (max[which] - min[which]);
        }

        //--------------
        //--------------
        vector <float> value;
        vector <float> valueI;
        vector <float> valueB;

        vector <float> min;
        vector <float> max;
        vector <float> pct;

};

