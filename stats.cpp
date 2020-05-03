#include "stats.h"
#include <cmath>
#include <cstring>

// initialize the private vectors so that, for each color,  entry 
// (x,y) is the cumulative sum of the the color values from (0,0)
// to (x,y). Similarly, the sumSq vectors are the cumulative
// sum of squares from (0,0) to (x,y).
stats::stats(PNG & im){
	// the structure of all the private vectors are 
	// < x, x, x, x, x, x >
	// < x, x, x, x, x, x >
	// < x, x, x, x, x, x >
	// < x, x, x, x, x, x >
	// < x, x, x, x, x, x >
	// < x, x, x, x, x, x >
	// thus, to access, it is vector[y][x]
	// initializes the <0,0> pos of the vectors
	sumRed.resize(im.height());
	sumBlue.resize(im.height());
	sumGreen.resize(im.height());
	sumsqRed.resize(im.height());
	sumsqBlue.resize(im.height());
	sumsqGreen.resize(im.height());
	sumRed[0].resize(im.width());
	sumBlue[0].resize(im.width());
	sumGreen[0].resize(im.width());
	sumsqRed[0].resize(im.width());
	sumsqBlue[0].resize(im.width());
	sumsqGreen[0].resize(im.width());

	sumRed[0][0]= im.getPixel(0, 0)->r;
	sumBlue[0][0] = im.getPixel(0, 0)->b;
	sumGreen[0][0] = im.getPixel(0, 0)->g;
	sumsqRed[0][0] = im.getPixel(0, 0)->r*im.getPixel(0, 0)->r;
	sumsqBlue[0][0] = im.getPixel(0, 0)->b*im.getPixel(0, 0)->b;
	sumsqGreen[0][0] =im.getPixel(0, 0)->g*im.getPixel(0, 0)->g;
	// initalizes the <x, 0> pos of the vector 
	for (unsigned long x=1; x<im.width(); x++){		
		sumRed[0][x] = sumRed[0][x-1] +im.getPixel(x, 0)->r;
		sumBlue[0][x] = sumBlue[0][x-1] + im.getPixel(x, 0)->b;
		sumGreen[0][x] = sumGreen[0][x-1] + im.getPixel(x, 0)->g;
		sumsqRed[0][x] = sumsqRed[0][x-1] + (im.getPixel(x, 0)->r*im.getPixel(x, 0)->r);
		sumsqBlue[0][x] = sumsqBlue[0][x-1] + (im.getPixel(x, 0)->b*im.getPixel(x, 0)->b);
		sumsqGreen[0][x] = sumsqGreen[0][x-1] + (im.getPixel(x, 0)->g*im.getPixel(x, 0)->g);
	}
	// initializes the <0, y> pos of the vector
	for (unsigned long y=1; y< im.height(); y++){
		
		sumRed[y].resize(im.width());
		sumBlue[y].resize(im.width());
		sumGreen[y].resize(im.width());
		sumsqRed[y].resize(im.width());
		sumsqBlue[y].resize(im.width());
		sumsqGreen[y].resize(im.width());
		
		sumRed[y][0] = sumRed[y-1][0] + im.getPixel(0, y)->r;
		sumBlue[y][0] = sumBlue[y-1][0] + im.getPixel(0, y)->b;
		sumGreen[y][0] = sumGreen[y-1][0] + im.getPixel(0, y)->g;
		sumsqRed[y][0] = sumsqRed[y-1][0] + (im.getPixel(0, y)->r*im.getPixel(0, y)->r);
		sumsqBlue[y][0] = sumsqBlue[y-1][0] + (im.getPixel(0, y)->b*im.getPixel(0, y)->b);
		sumsqGreen[y][0] = sumsqGreen[y-1][0] + (im.getPixel(0, y)->g*im.getPixel(0, y)->g);
	}
	// gets the rest of the cumulative colour values
	for (unsigned long y=1; y < im.height(); y++){
		for (unsigned long x=1; x < im.width(); x++){
			sumRed[y][x] = sumRed[y-1][x] + sumRed[y][x-1] + im.getPixel(x, y)->r-sumRed[y-1][x-1];
			sumBlue[y][x] = sumBlue[y-1][x] + sumBlue[y][x-1] + im.getPixel(x, y)->b -sumBlue[y-1][x-1];
			sumGreen[y][x] = sumGreen[y-1][x] + sumGreen[y][x-1]+ im.getPixel(x, y)->g-sumGreen[y-1][x-1];
			sumsqRed[y][x] = sumsqRed[y-1][x] + sumsqRed[y][x-1] + (im.getPixel(x, y)->r*im.getPixel(x, y)->r) - sumsqRed[y-1][x-1] ;
			sumsqBlue[y][x] = sumsqBlue[y-1][x] + sumsqBlue[y][x-1] + (im.getPixel(x, y)->b*im.getPixel(x, y)->b) - sumsqBlue[y-1][x-1];
			sumsqGreen[y][x] = sumsqGreen[y-1][x] + sumsqGreen[y][x-1] + (im.getPixel(x, y)->g*im.getPixel(x, y)->g) - sumsqGreen[y-1][x-1];
		}
	}
}

/* returns the sums of all pixel values across the given color channels.
* useful in computing the score of a rectangle
* PA3 function
* @param channel is one of r, g, or b
* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::getSum(char channel, pair<int,int> ul, pair<int,int> lr){
	int ulX = get<0>(ul);
	int ulY = get<1>(ul);
	int lrX = get<0>(lr);
	int lrY = get<1>(lr);
	if (channel=='r'){
		if (ulX == 0 && ulY == 0) return sumRed[lrY][lrX];
		if (ulX == 0) return sumRed[lrY][lrX]-sumRed[ulY-1][lrX];
		if (ulY== 0) return sumRed[lrY][lrX]-sumRed[lrY][ulX-1];
		else return sumRed[lrY][lrX]-sumRed[ulY-1][lrX]-sumRed[lrY][ulX-1]+sumRed[ulY-1][ulX-1];
	}
	else if (channel=='g'){
		if (ulX == 0 && ulY == 0) return sumGreen[lrY][lrX];
		if (ulX == 0) return sumGreen[lrY][lrX]-sumGreen[ulY-1][lrX];
		if (ulY== 0) return sumGreen[lrY][lrX]-sumGreen[lrY][ulX-1];
		else return sumGreen[lrY][lrX]-sumGreen[ulY-1][lrX]-sumGreen[lrY][ulX-1]+sumGreen[ulY-1][ulX-1];
	}
	else {
		if (ulX == 0 && ulY == 0) return sumBlue[lrY][lrX];
		if (ulX == 0) return sumBlue[lrY][lrX]-sumBlue[ulY-1][lrX];
		if (ulY== 0) return sumBlue[lrY][lrX]-sumBlue[lrY][ulX-1];
		else return sumBlue[lrY][lrX]-sumBlue[ulY-1][lrX]-sumBlue[lrY][ulX-1]+sumBlue[ulY-1][ulX-1];
	}
}
/* returns the sums of squares of all pixel values across all color channels.
* useful in computing the score of a rectangle
* PA3 function
* @param channel is one of r, g, or b
* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr){
	int ulX = get<0>(ul);
	int ulY = get<1>(ul);
	int lrX = get<0>(lr);
	int lrY = get<1>(lr);
	if (channel=='r'){
		if (ulX == 0 && ulY == 0) return sumsqRed[lrY][lrX];
		if (ulX == 0) return sumsqRed[lrY][lrX]-sumsqRed[ulY-1][lrX];
		if (ulY== 0) return sumsqRed[lrY][lrX]-sumsqRed[lrY][ulX-1];
		else 
			return sumsqRed[lrY][lrX]-sumsqRed[ulY-1][lrX]-sumsqRed[lrY][ulX-1]+sumsqRed[ulY-1][ulX-1];
	}
	else if (channel=='g'){
		if (ulX == 0 && ulY == 0) return sumsqGreen[lrY][lrX];
		if (ulX == 0) return sumsqGreen[lrY][lrX]-sumsqGreen[ulY-1][lrX];
		if (ulY== 0) return sumsqGreen[lrY][lrX]-sumsqGreen[lrY][ulX-1];
		else
			return sumsqGreen[lrY][lrX]-sumsqGreen[ulY-1][lrX]-sumsqGreen[lrY][ulX-1]+sumsqGreen[ulY-1][ulX-1];
	}
	else {
		if (ulX == 0 && ulY == 0) return sumsqBlue[lrY][lrX];
		if (ulX == 0) return sumsqBlue[lrY][lrX]-sumsqBlue[ulY-1][lrX];
		if (ulY== 0) return sumsqBlue[lrY][lrX]-sumsqBlue[lrY][ulX-1];
		else return sumsqBlue[lrY][lrX]-sumsqBlue[ulY-1][lrX]-sumsqBlue[lrY][ulX-1]+sumsqBlue[ulY-1][ulX-1];
	}

}
// given a rectangle, return the number of pixels in the rectangle
/* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle 
*/
long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
	long numX = get<0>(lr) - get<0>(ul)+1;
	long numY = get<1>(lr) - get<1>(ul)+1;
	return numX*numY;

}
// given a rectangle, return the average color value over the 
// rectangle as a pixel.
/* Each color component of the pixel is the average value of that 
* component over the rectangle.
* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle 
*/
RGBAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
	long totalPixels = rectArea(ul, lr);
	long redAvg = getSum('r', ul, lr) / totalPixels;
	long greenAvg = getSum('g', ul, lr) / totalPixels;
	long blueAvg = getSum('b', ul, lr) / totalPixels;
	RGBAPixel avgPix = RGBAPixel(redAvg, greenAvg, blueAvg);
	return avgPix;
}

// given a rectangle, compute its sum of squared deviations from 
// mean, over all color channels. Will be used to make split when
// building tree.
/* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle 
*/
long stats::getScore(pair<int,int> ul, pair<int,int> lr){
	long totalPixels = abs(rectArea(ul, lr));
	long redSqVal = getSumSq('r', ul, lr);
	long greenSqVal = getSumSq('g', ul, lr);
	long blueSqVal = getSumSq('b', ul, lr);
	long redValSqAvg = (getSum('r', ul, lr)*getSum('r', ul, lr))/ totalPixels;
	long greenValSqAvg = (getSum('g', ul, lr)*getSum('g', ul, lr)) / totalPixels;
	long blueValSqAvg = (getSum('b', ul, lr)*getSum('b', ul, lr))/ totalPixels;
 	long red = redSqVal - redValSqAvg;
	long green = greenSqVal - greenValSqAvg;
	long blue = blueSqVal - blueValSqAvg;
	return red + blue + green;
}