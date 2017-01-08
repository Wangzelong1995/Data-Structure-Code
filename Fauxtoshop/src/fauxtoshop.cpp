#include <iostream>
#include "console.h"
#include "gwindow.h"
#include "grid.h"
#include "simpio.h"
#include "strlib.h"
#include "gbufferedimage.h"
#include "gevents.h"
#include "math.h" //for sqrt and exp in the optional Gaussian kernel
#include <stdlib.h> // for random integer generation
using namespace std;

static const int    WHITE = 0xFFFFFF;
static const int    BLACK = 0x000000;
static const int    GREEN = 0x00FF00;
static const double PI    = 3.14159265;

void     doFauxtoshop(GWindow &gw, GBufferedImage &img);

bool     openImageFromFilename(GBufferedImage& img, string filename);
bool 	saveImageToFilename(const GBufferedImage &img, string filename);
void     getMouseClickLocation(int &row, int &col);


// helper for open image and check if open successfully
void openImage(GBufferedImage &img, string name){
    bool success = openImageFromFilename(img, name);
    
    while(! success){
        cout << "invalid name, please re-enter file name:"<<endl;
        cin >> name;
        
        success = openImageFromFilename(img,name);
    }
}

// helper for adjusting image size and put in the Gwindow
void adjust(GWindow &gw, GBufferedImage &img){
    gw.requestFocus();
    
    gw.add(&img,0,0);
    gw.setCanvasSize(img.getWidth(), img.getHeight());

}

void scatter(GWindow &gw, Grid<int> & orig, int radius, GBufferedImage & resultImage){
    // check if the radius is valid
    while(!((radius >= 0) && (radius <= 100)))
    {
        radius = getInteger("Invalid value, please enter an integer in 0-100, please re-enter:");
    }

    Grid<int> resultGrid = Grid<int>(orig.numRows(), orig.numCols());
    int pixel,  deltaRow, deltaCol;
    bool generationFinished;
    
    for(int i = 0; i < resultGrid.numRows(); i++){
        for(int j = 0; j < resultGrid.numCols(); j++){
            
            // deltaRow and deltaCol are in (-radius, radius)
            deltaRow = rand() % (2*radius) - radius;
            deltaCol = rand() % (2*radius) - radius;
            
            //check if we finished generating random points by checking if they are valid
            generationFinished = ((i+deltaRow) >= 0) && ((i+deltaRow) < resultGrid.numRows()) 
            && ((j+deltaCol) >= 0) && ((j+deltaCol) < resultGrid.numCols());
            
            while(! generationFinished){
                deltaRow = rand() % (2*radius) - radius;
                deltaCol = rand() % (2*radius) - radius;
            
                generationFinished = ((i+deltaRow) >= 0) && ((i+deltaRow) < resultGrid.numRows()) 
                && ((j+deltaCol) >= 0) && ((j+deltaCol) < resultGrid.numCols());
            }

            //get the pixel value from random point within the radius and assign
            pixel = orig[i+deltaRow][j+deltaCol];
            
            resultGrid.set(i,j,pixel);
        }
    }
    
    // create gbufferedimage from grid and display it in the gwindow    
    resultImage.fromGrid(resultGrid);

    gw.clear();
    gw.add(&resultImage, 0,0);
}

// helper function for computing distance between two pixels from grid g,w, with position (i,j) and (x,y)
int distance(Grid<int> & g, Grid<int> & f, int i, int j, int x, int y){
    int pixel = g[i][j];
    int pixel1 = f[x][y];

    int red,red1,blue, blue1,green, green1;

    GBufferedImage::getRedGreenBlue(pixel, red, green, blue);
    GBufferedImage::getRedGreenBlue(pixel1, red1, green1, blue1);

    return max(max(abs(red-red1),abs(green-green1)), abs(blue-blue1));
}

// helper function for computing distance between a pixel and all of its neighbors
int neighborDist(Grid<int> & g, int x, int y){
    
    int ret = 0;
    
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            if(g.inBounds(x+i,y+j))
                ret = max(ret, distance(g,g,x,y,x+i,y+j));
        }
    }
    return (int) ret;
}

// edge detection implementation
void detectEdge(GWindow &gw,Grid<int> & orig, int threshold, GBufferedImage & resultImage){
    Grid<int> resultGrid = Grid<int>(orig.numRows(), orig.numCols());
    // check if the threshold is valid
    while(threshold <= 0){
        cout<<"invalid threshold, please re-enter a positive one:";
        cin>>threshold;
    }
    // loop all the pixels, if distance between the pixel and its neighbors is large, mark it as edge
    for(int i = 0; i < resultGrid.numRows(); i++){
        for(int j = 0; j < resultGrid.numCols(); j++){
            if(neighborDist(orig, i,j) > threshold)
                resultGrid.set(i,j,BLACK);
            else
                resultGrid.set(i,j,WHITE);
        }
    }
    //create gbuffered image from the grid, and display it
    resultImage.fromGrid(resultGrid);
    gw.clear();
    gw.add(&resultImage, 0,0);
    
}

// helper function for computing distance between pixel value g[i][j] and target
int distanceToColor(Grid<int> & g, int i, int j, int target){

    int pixel = g[i][j];

    int red,red1,blue, blue1,green, green1;

    GBufferedImage::getRedGreenBlue(pixel, red, green, blue);
    GBufferedImage::getRedGreenBlue(target, red1, green1, blue1);

    return fmax(fmax(abs(red-red1),abs(green-green1)), abs(blue-blue1));
}

//helper function for parsing a input string and extracting the row number and col number
void parse(string& input, int& row, int & col){

    string row_str = input.substr(1,input.find(",")-1);
    string col_str = input.substr(input.find(",")+1, input.find(")")-input.find(",")-1);

    row = stringToInteger(row_str);
    col = stringToInteger(col_str);

}

// helper function for getting pasting location either from mouse click or user input
void getLocation(int & row, int & col, string & location){
    
    if(location.length() == 0)
        getMouseClickLocation(row,col);
    else
        parse(location, row, col);
}

// implementation of greenScreen
void greenScreen(GWindow &gw, GBufferedImage & background, int tolerance, GBufferedImage & Add, int row, int col){
    
    // inform user the row and col input
    cout<<"row: "<<row<<endl;
    cout<<"col: "<<col<<endl;
    
    Grid<int> orig_grid = background.toGrid();
    Grid<int> add_grid = Add.toGrid();
    // core algo: if the distance between current pixel and green is large enough, use its original pixel, else
    // keep original pixel value in the background
    for(int i = 0; i < add_grid.numRows(); i++){
        for(int j = 0; j < add_grid.numCols(); j++){
            if((row+i < orig_grid.numRows()) && (col+j < orig_grid.numCols()))
            {    
                if(distanceToColor(add_grid,i,j,GREEN) > tolerance)
                    orig_grid.set(row+i,col+j,add_grid[i][j]);
            }
            
        }
    }
    //create gbuffered image and display
    background.fromGrid(orig_grid);
    gw.clear();
    gw.add(& background, 0,0);
}

// helper function for compare two images
void compareIm(GBufferedImage & A, GBufferedImage & B){
    int num = A.countDiffPixels(B);
    
    if(num > 0)
        cout<<"These images differ in "<<num<<" pixel locations!"<<endl;
    else
        cout<<"These images are the same!"<<endl;
}

Vector<double> gaussKernelForRadius(int radius) {
    if (radius < 1) {
        Vector<double> empty;
        return empty;
    }
    Vector<double> kernel(radius * 2 + 1);
    double magic1 = 1.0 / (2.0 * radius * radius);
    double magic2 = 1.0 / (sqrt(2.0 * PI) * radius);
    int r = -radius;
    double div = 0.0;
    for (int i = 0; i < kernel.size(); i++) {
        double x = r * r;
        kernel[i] = magic2 * exp(-x * magic1);
        r++;
        div += kernel[i];
    }
    for (int i = 0; i < kernel.size(); i++) {
        kernel[i] /= div;
    }
    return kernel;
}
//helper function for convert rgb back to pixel value: for gaussian blurring
int rgbToColor(int r, int g, int b){
    return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}

void GaussianBlurring(Grid<int> & orig, GBufferedImage & result,GWindow & gw, int radius){
    //create the blurred grid, and get the weights from kernel
    Grid<int> blurred = Grid<int>(orig.numRows(), orig.numCols());
    Vector<double> weights = gaussKernelForRadius(radius);

    int red,blue,green, curRed, curGreen, curBlue;

    int redArray [orig.numRows()][orig.numCols()];
    int blueArray [orig.numRows()][orig.numCols()];
    int greenArray [orig.numRows()][orig.numCols()];

    // first pass: horizantal
    for(int i = 0; i < blurred.numRows(); i++)
    {
        for(int j = 0; j < blurred.numCols(); j++)
        {
            red = 0,blue = 0,green = 0;
            // for the current pixel, for each of its neighbors
            for(int r = -radius; r <= radius; r++){
                if(j+r >= 0 && j+r < orig.numCols()){
                    // extract the rgb value, 
                    GBufferedImage::getRedGreenBlue(orig[i][j+r], curRed, curGreen, curBlue);
                    // compute weighted average for rgb
                    red += weights[r+radius]*curRed;
                    blue += weights[r+radius]*curBlue;
                    green += weights[r+radius]*curGreen;
                }
            }
            // store the value after the first convolution pass in the array
            redArray[i][j] = red;
            greenArray[i][j] = green;
            blueArray[i][j] = blue;
        }  
            
    }

    //second pass: vertical
    for(int i = 0; i < blurred.numRows(); i++){
        for(int j = 0; j < blurred.numCols(); j++)
        {
            red = 0,blue = 0,green = 0;
            // for the current pixel, for each of its neighbors
            for(int r = -radius; r <= radius; r++){
                if(i+r >= 0 && i+r < orig.numRows()){
                    // extract the rgb value, 
                    GBufferedImage::getRedGreenBlue(orig[i+r][j], curRed, curGreen, curBlue);
                    // compute weighted average for rgb
                    red += weights[r+radius]*curRed;
                    blue += weights[r+radius]*curBlue;
                    green += weights[r+radius]*curGreen;
                }   
            }
            // add the weighted average to the stored value, which was created after first pass
            redArray[i][j] += red;
            greenArray[i][j] += green;
            blueArray[i][j] += blue;
            //update the value in grid
            blurred.set(i,j,rgbToColor(redArray[i][j]/2, greenArray[i][j]/2,blueArray[i][j]/2));
        }
    }
    //create gbuffered image and display
    result.fromGrid(blurred);
    gw.clear();
    gw.add(& result, 0,0);
}

bool openImageFromFilename(GBufferedImage& img, string filename) {
    try { img.load(filename); }
    catch (...) { return false; }
    return true;
}

bool saveImageToFilename(const GBufferedImage &img, string filename) {
    try { img.save(filename); }
    catch (...) { return false; }
    return true;
}

void getMouseClickLocation(int &row, int &col) {
    GMouseEvent me;
    do {
        me = getNextEvent(MOUSE_EVENT);
    } while (me.getEventType() != MOUSE_CLICKED);
    row = me.getY();
    col = me.getX();
}

void saveFile(GBufferedImage & img, GBufferedImage & result, string & action,  int choice){
    // if user hits ENTER, do nothing
    bool success;
    if(action.length() == 0)
    {
        cout<<"Saving skipped..."<<endl;
    }
    // otherwise save the files
    else {
        // here if we are doing green screen, since I modified the background image, it (aka img) should be saved
        if (choice == 3)
            success = saveImageToFilename(img, action);
        //for all the others, the image I created (aka result) should be saved
        else 
            success = saveImageToFilename(result, action);
        // check failure
        while(! success){
            action = getLine("image saving failed, try again, enter the name to save: ");
            
            if (choice == 3)
                success = saveImageToFilename(img, action);
            else 
                success = saveImageToFilename(result, action);
        }

        cout<<"Image saved !"<<endl;
    } 

}

// core function of Fauxtoshop
void doFauxtoshop(GWindow &gw, GBufferedImage &img) {

    // all the functionality is in a while(true) loop, with user's choice to continueDoing or terminate
    while(true)
    {
        cout << "Welcome to Fauxtoshop!" << endl;

        string name = getLine("Enter name of image file to open (or blank to quit): ");
        //if user hit ENTER, exit
        if(name.length() == 0)
            break;

        cout<<"Opening image file, may take a minute...";
        //open image and adjust gwindow size
        openImage(img, name);
        adjust(gw, img);

        // display functions
        cout<<"Which image filter would you like to apply?"<<endl;

        cout<<"                   1 - Scatter"<<endl;
        cout<<"                   2 - Edge detection"<<endl;
        cout<<"                   3 - \"Green Screen\" with another image"<<endl;
        cout<<"                   4 - Compare image with another image"<<endl;
        cout<<"                   5 - Apply Gaussian blurring"<<endl;

        // get user's choice and check if it is valid
        int choice = getInteger("Your choice:", "Invalid type. It should be an integer in the range of 1-5"); 
        while(choice < 1 || choice > 5){
            choice = getInteger("Invalid value, choice should be an integer in the range of 1-5, please re-enter:");
        }

        // turn input image to grid to access the pixel data
        Grid<int> original = img.toGrid();
        //create a empty result image
        GBufferedImage result;

        //perform different functions with different user choice
        switch(choice){

            case 1:
                {
                    // get radius 
                    int radius;
                    radius = getInteger("Enter degree of scatter [1-100]:", "Invalid type. It should be an integer in the range of 1-100");
                    // do stuff
                    scatter(gw,original,radius,result);
                    break;
                }
            case 2:
                {
                    // get threshold and do stuff
                    int threshold = getInteger("Enter threshold for edge detection:", "Invalid type. It should be an integer");
                    detectEdge(gw,original,threshold,result);
                    break;                   
                }
            case 3:
                {
                    // get sticker image
                    cout<<"Now choose another file to add to your background image."<<endl;
                    string toAdd = getLine("Enter name of image file to open:");
                    
                    cout<<"Opening image file, may take a minute...";
                    openImage(result,toAdd);

                    // get tolerance and pasting location
                    int tolerance = getInteger("Now choose tolerance threshold:");    
                    string location = getLine("Enter location to place image as \"(row,col)\" (or blank to use mouse): ");
                    
                    int row,col;                    
                    getLocation(row,col,location);
                    // do stuff
                    greenScreen(gw, img, tolerance, result, row, col);
                    break;
                }
            case 4:
                {   
                    // get the image to compare
                    cout<<"Now choose another image file to compare to"<<endl;
                    string toCompare = getLine("Enter name of image file to open:");
                    cout<<"Opening image file, may take a minute...";
                    openImage(result,toCompare);
                    cout<<" "<<endl;
                    // do stuff
                    compareIm(result, img);
                    break;    
                }
            case 5:
                {
                    int radius = getInteger("Enter the standard deviation (integer which defines the kernel size): ");
                    GaussianBlurring(original,result,gw,radius);
                    break;
                }
        }
        // start saving procedure
        string action = getLine("Enter filename to save image (or blank to skip saving):");
        
        //save images
        saveFile(img, result, action, choice);
        // task finished, clear the gwindow
        gw.clear();
        cout<<" "<<endl;
    }  
}

int main() {
    GWindow gw;
    gw.setTitle("Fauxtoshop");
    gw.setVisible(true);
    GBufferedImage img;
    doFauxtoshop(gw, img);
    return 0;
}