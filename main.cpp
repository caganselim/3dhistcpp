#include <iostream>
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <cmath>

using namespace cv;
using namespace std;

string type2str(int type) {
    string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch ( depth ) {
        case CV_8U:  r = "8U"; break;
        case CV_8S:  r = "8S"; break;
        case CV_16U: r = "16U"; break;
        case CV_16S: r = "16S"; break;
        case CV_32S: r = "32S"; break;
        case CV_32F: r = "32F"; break;
        case CV_64F: r = "64F"; break;
        default:     r = "User"; break;
    }

    r += "C";
    r += (chans+'0');

    return r;
}

void printHist(Mat hist, int histSize[]){

    string ty =  type2str( hist.type() );
    cout << "Type: " <<  ty << endl;

    double sum = 0;
    for (int i=0; i < histSize[0]; i++) {
        for (int j=0; j < histSize[1]; j++) {
            for (int k=0; k < histSize[2]; k++) {

                double value = hist.at<float>(i, j, k);
                cout << "Value(" << i << ", " << j << ", " << k <<"): " << value << endl;
                sum += value*value;

            }
        }
    }
    cout << "Total sum of squares: " << sum << endl;

}

int main() {

    //Define histogram subdivisions in each channel, i.e. 5 bins
    int histSize[3] = {5, 5, 5};

    //The limits for the values to be measured (min,max)
    float range[2] = {0, 256};
    const float * ranges[3] = {range, range, range};

    //
    int channels[3] = {0, 1, 2};

    //Create histogram container as a vector
    vector<Mat> histograms;

    //Iterate all over the images
    for(int i = 1; i < 11; i++){

       string filename = to_string(i) + ".jpg";

       cout << "========== Processing image: " << filename << " ==========" << endl;

       Mat inputImg = imread( filename,cv::IMREAD_COLOR);
       Mat hist;

       //Calculate histogram
       calcHist(&inputImg, 1, channels, Mat(), hist, 3, histSize, ranges);

       //Normalize (L2 default)
       normalize(hist,hist);
       printHist(hist,histSize);


       //Save the histogram to the end of the list
       histograms.push_back(hist);

    }

    cout << "==== Evaluating pairwise distances... ====" << endl;

    //Calculate pairwise distances and calculate norms
     vector<Mat>::iterator first = histograms.begin();
     vector<Mat>::iterator last = histograms.end();

    for(; first != last; ++first){
        for(vector<Mat>::iterator next = std::next(first); next != last; ++next){

            int idx1 = first - histograms.begin();
            int idx2 = next - histograms.begin();
            cout << "Diff("<< idx1 + 1 << ".jpg" << "," << idx2 + 1 << ".jpg)= " << norm(*first,*next,NORM_L2) << endl;
        }
    }

    return 0;

}