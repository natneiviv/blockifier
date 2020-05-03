
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"

/* given */
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

/* given */
twoDtree::~twoDtree(){
	clear();
}

/* given */
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}

/* given */
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}
/**
* Constructor that builds a twoDtree out of the given PNG.
* Every leaf in the tree corresponds to a pixel in the PNG.
* Every non-leaf node corresponds to a rectangle of pixels 
* in the original PNG, represented by an (x,y) pair for the 
* upper left corner of the rectangle and an (x,y) pair for 
* lower right corner of the rectangle. In addition, the Node
* stores a pixel representing the average color over the 
* rectangle. 
*
* Every node's left and right children correspond to a partition
* of the node's rectangle into two smaller rectangles. The node's
* rectangle is split by the horizontal or vertical line that 
* results in the two smaller rectangles whose sum of squared 
* differences from their mean is as small as possible.
*
* The left child of the node will contain the upper left corner
* of the node's rectangle, and the right child will contain the
* lower right corner.
*
* This function will build the stats object used to score the 
* splitting lines. It will also call helper function buildTree.
*/
twoDtree::twoDtree(PNG & imIn){
	cout << "begin: twoDtree constructor" << endl;
	 stats obj(imIn);
	 height = imIn.height()-1;
	 width = imIn.width()-1;
	 pair<int, int> ul = make_pair(0, 0);
	 pair<int, int> lr = make_pair(width, height);
	 root = buildTree(obj, ul, lr);
	 cout << "end: twoDtree constructor" << endl;
}

/**
* Private helper function for the constructor. Recursively builds
* the tree according to the specification of the constructor.
* @param s Contains the data used to split the rectangles
* @param ul upper left point of current node's rectangle.
* @param lr lower right point of current node's rectangle.
*/
twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {
	RGBAPixel avg = s.getAvg(ul, lr);
	Node* n = new Node(ul, lr, avg);
	if (ul==lr){
		return n;
	}	
	int lrX = get<0>(lr);
	int lrY = get<1>(lr);
	int ulX = get<0>(ul);
	int ulY = get<1>(ul);
	long horizMinVar = LONG_MAX ;
	long vertMinVar = LONG_MAX;
	int x, y;
	// horizontal split
	for (int i=ulY; i <lrY; i++){
		pair<int, int> upHoriz = make_pair(lrX, i);
		pair<int, int> botHoriz = make_pair(ulX, i+1);
		long hVarUp = s.getScore(ul, upHoriz); 
		long hVarBot = s.getScore(botHoriz, lr);
		long diffVar = hVarUp + hVarBot;
		if (diffVar <= horizMinVar){
			horizMinVar = diffVar;
			y = i;
		}	
	}
	// vertical split
	for (int i=ulX; i<lrX; i++){
		pair<int, int> leftVert = make_pair(i, lrY);
		pair<int, int> rightVert = make_pair(i+1, ulY);
		long vVarLeft = s.getScore(ul, leftVert);
		long vVarRight = s.getScore(rightVert, lr);
		long diffVar = vVarRight + vVarLeft;
		if (diffVar <= vertMinVar){
			vertMinVar = diffVar;
			x = i;
		}
	}
	pair<int, int> newUL, newLR;
	if (vertMinVar < horizMinVar){ // split vertically
		newLR.first = x;
		newLR.second = lrY;
		newUL.first = x+1;
		newUL.second = ulY;
	} else { // split horizontally
		newLR.first = lrX;
		newLR.second = y;
		newUL.first = ulX;
		newUL.second = y+1;
	}
	n->left = buildTree(s, ul, newLR);
	n->right = buildTree(s, newUL, lr);
	return n;
}

void twoDtree::renderLeaf(Node* n, PNG & image){
	if (!n) return;
	if (!n->left && !n->right){
		int ulX = get<0>(n->upLeft);
		int ulY = get<1>(n->upLeft);
		int lrX = get<0>(n->lowRight);
		int lrY = get<1>(n->lowRight);
		if (n->upLeft == n->lowRight){ // one pixel
			*image.getPixel(ulX, ulY)= n->avg;
		} else if (n->upLeft!=n->lowRight){ // this case is for the pruning...
			for (int y = ulY; y <= lrY; y++){ 
				for (int x = ulX; x <= lrX; x++) {
					*image.getPixel(x, y)= n->avg;
				}
			}
		}
	}
	renderLeaf(n->left, image);
	renderLeaf(n->right, image);
}
/**
* Render returns a PNG image consisting of the pixels
* stored in the tree. may be used on pruned trees. Draws
* every leaf node's rectangle onto a PNG canvas using the 
* average color stored in the node.
*/
PNG twoDtree::render(){
	PNG ret = PNG(width+1, height+1);
	renderLeaf(root, ret);
	return ret;
}
// returns the number of leaves
double twoDtree::numLeaves(Node* n){
	if (!n) return 0;
	if (!n->left && !n->right){
		return 1;
	} else return numLeaves(n->left) + numLeaves(n->right);
}

// returns the number of leaves that are within the tol
double twoDtree::tolHelper(Node* compare, Node* curr, int tol){
	if (!curr) return 0;
	if (!curr->left && !curr->right){
		int r = compare->avg.r - curr->avg.r;
		int g = compare->avg.g - curr->avg.g;
		int b = compare->avg.b - curr->avg.b;
		int d = (r*r)+(g*g)+(b*b);
		if (d <= tol) {
			return 1;
		} else return 0;
	}
	return tolHelper(compare, curr->left, tol)+tolHelper(compare, curr->right, tol);
}

void twoDtree::clearHelper(Node* n){
	if (n==NULL) return;
	clearHelper(n->left);
	clearHelper(n->right);
	delete n;
}
void twoDtree::pruneHelper(Node* n, double pct, int tol){
	if (!n) return;
	double totalLeaf = numLeaves(n);
	double numWithinTol = tolHelper(n, n, tol);
	double pctLeaf = numWithinTol/totalLeaf;
	if (pctLeaf>=pct){
		clearHelper(n->left);
		clearHelper(n->right);
		n->left = NULL;
		n->right = NULL;
	} else {
		pruneHelper(n->left, pct, tol);
		pruneHelper(n->right, pct, tol);
	}
}
/*
*  Prune function trims subtrees as high as possible in the tree.
*  A subtree is pruned (cleared) if at least pct of its leaves are within 
*  tol of the average color stored in the root of the subtree. 
*  Pruning criteria should be evaluated on the original tree, not 
*  on a pruned subtree. (we only expect that trees would be pruned once.)
*  
* You may want a recursive helper function for this one.
*/
void twoDtree::prune(double pct, int tol){
	pruneHelper(root, pct, tol);

}
/**
* Destroys all dynamically allocated memory associated with the
* current twoDtree class. Complete for PA3.
* You may want a recursive helper function for this one.
*/
void twoDtree::clear() { // looks like this works fine
	clearHelper(root);
	root = NULL;
}

twoDtree::Node * twoDtree::copyHelper(const Node* node) { // works fine
	if (!node) return NULL;
	Node* newNode = new Node(node->upLeft, node->lowRight, node->avg);
	newNode->left = copyHelper(node->left);
	newNode->right = copyHelper(node->right);
	return newNode;
	
}

/**
* Copies the parameter other twoDtree into the current twoDtree.
* Does not free any memory. Called by copy constructor and op=.
* You may want a recursive helper function for this one.
* @param other The twoDtree to be copied.
*/
void twoDtree::copy(const twoDtree & orig){ // works fine
	height = orig.height;
	width = orig.width;
	root = copyHelper(orig.root);
}