package tsps;

import netP5.*;
import oscP5.*;
import processing.core.PApplet;
import processing.core.PVector;
import java.util.ArrayList;

public class TSPSScene
{

    private PVector averageMotion;
    private float   percentCovered;
    private int     numPeople;

    private int gridX, gridY;
 	private ArrayList<Boolean>		gridValues;
    private ArrayList<Rectangle>	grid;

	private final PApplet app;

    public TSPSScene( PApplet _app ){
    	app = _app;
    	averageMotion 	= new PVector(0,0);
    	percentCovered 	= 0.0f;
    	numPeople 		= 0;
    	gridX = gridY 	= 0;
    	gridValues		= new ArrayList<Boolean>();
    	grid			= new ArrayList<Rectangle>();
    }
    
    /**
     * @returns {PVector} Average motion (via optical flow) in Scene
     */
    public PVector getAverageMotion(){
    	return averageMotion;
    }

    /**
     * @returns {float} How much of the scene is covered by people
     */
    public float	getPercentCovered(){
    	return percentCovered;
    }

    /**
     * @returns {int} Number of people
     */
    public int		getNumPeople(){
    	return numPeople;
    }

    public void 	update( OscMessage m ){
    	numPeople      			= m.get(1).intValue();
        percentCovered 			= m.get(2).floatValue();
        averageMotion.x  	 	= m.get(3).floatValue();
        averageMotion.y  		= m.get(4).floatValue();
        
        int _gridX               = m.get(5).intValue();
        int _gridY               = m.get(6).intValue();
        
        if ( _gridX != getGridX() || _gridY != getGridY() ){
            buildGrid( _gridX, _gridY);
        }
        
        // double check
        if ( grid.size() == m.arguments().length - 7 ){
            for (int i=0; i<grid.size(); i++){
                gridValues.set(i, m.get(i + 7).intValue() == 1);
            }
        }
    }

    /**
     * Debug drawing of scene (with grid)
     */
    public void draw( int width, int height ){
    	for (int i=0; i<grid.size(); i++){
            Rectangle check = grid.get(i);

            if ( gridValues.get(i) == true ){
            	app.noStroke();
            	app.fill(255,255,255,100);
            } else {
            	app.fill(255,255,255,0);
            	app.noFill();
            	app.stroke(255);
            }
            app.rect(check.x * width, check.y * height, check.width  * width, check.height * height);
            app.fill(255);
        }
    }
    
    private void buildGrid( int w, int h ){
    	grid.clear();
        gridValues.clear();
        
        gridX = w;
        gridY = h;
        
        if ( w == 0 || h == 0 ) return;
        
        for (int x=0; x<w; x++){
            for (int y=0; y<h; y++){
            	Rectangle rect = new Rectangle();
            	rect.x = (float) x/w;
            	rect.y = (float) y/h;
            	rect.width = (float)1.0 / (float) w;
            	rect.height = (float) 1.0 / (float) h;
                grid.add(rect);
                gridValues.add(false);
            }
        }
    }

    /**
     * Get grid rectangles (note: rects are normalized).
     */
    public ArrayList<Rectangle> getGrid(){
    	return grid;
    }

    /**
     * Get arraylist of grid booleans
     */
    public ArrayList<Boolean> getGridValues(){
    	return gridValues;
    }
    
    /**
     * @returns Number of horizontal grid squares
     */
    public int getGridX(){
    	return gridX;
    }
    
    /**
     * @returns Number of vertical grid squares
     */
    public int getGridY(){
    	return gridY;
    }  
};