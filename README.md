# blockifier
Given a picture in PNG format, this program will create a 'blocky', almost blur-like effect on the image. To do so, portions of the image with similar RGB values will be replaced with a rectangle that contains the average of the RGB values.<br><br>
The image is first represented as a binary tree, where the entire image is the root node, and the child nodes are split images of the parent node. To be more specific, the parent node contains the whole rectangle, the left child contains the parent’s upper left corner and a new lower right corner, and the right child contains a new upper left corner and the parent’s lower right corner. Splitting the parent node between the left and right child node is done in a way to minimize the colour variability between the two nodes.<br><br>
Inspiration behind this program can be found here: <br>
https://mymodernmet.com/algorithm-art-color-study-dimitris-ladopoulos/
