
// File:        main.cpp
// Author:      cheeren
// Date:        2018-02-25
// Description: Partial test of PA3 functionality
//              Reads Point data from external files
//              Produces PNG images of the point sets


#include "twoDtree.h"
#include "cs221util/RGBAPixel.h"
#include "cs221util/PNG.h"

using namespace cs221util;
using namespace std;

int main()
{

	// read in image
	PNG origIm0;
	origIm0.readFromFile("images/test.png");
	PNG origIm1;
	origIm1.readFromFile("images/stanley-totem-poles.png");
	PNG origIm2;
	origIm2.readFromFile("images/ubc-totem-poles.png");
	PNG origIm3;
	origIm3.readFromFile("images/remb.png");
	PNG origIm4;
	origIm4.readFromFile("images/rosa.png");

	// use it to build a twoDtree
	//twoDtree t0(origIm0);
	twoDtree t1(origIm1);
	twoDtree tCopy1(t1);
	twoDtree t2(origIm2);
	twoDtree tCopy2(t2);
	twoDtree t3(origIm3);
	twoDtree tCopy3(t3);
	twoDtree t4(origIm4);
	twoDtree tCopy4(t4);

	// prune the twoDtree
	//where at least 100*X% of pixels are within Y of mean
	cout << "pruning" << endl;
	tCopy1.prune(1.0, 10000); 
	tCopy2.prune(1.0, 10000); 
	tCopy3.prune(1.0, 3000); 
	tCopy4.prune(1.0, 3000); 

	PNG ppic1 = tCopy1.render();
	PNG ppic2 = tCopy2.render();
	PNG ppic3 = tCopy3.render();
	PNG ppic4 = tCopy4.render();
	ppic1.writeToFile("images/prunedstanleytotem.png");
	ppic2.writeToFile("images/prunedubctotem.png");
	ppic3.writeToFile("images/prunedremb.png");
	ppic4.writeToFile("images/prunedrosa.png");
	cout << "end" << endl;

  return 0;
}

