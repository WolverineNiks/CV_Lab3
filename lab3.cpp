/**
 * Computer Engineering Master's Degree (AI) - UniPd
 * Computer Vision - 9 CFU
 * Prof: Ghidoni, Prof: Pretto
 * 
 * Nikhil Chander 
 * 2019176
 * 
 */

/*#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core/utils/filesystem.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv){

    //Load Image
    cout << "Loading Images..." << endl;
    Mat img = imread("data/image.jpg", CV_LOAD_IMAGE_COLOR);
    if(!img.data) {
        cout << "No image found!" << endl;
        return -1;
    }

    //Variables for calcHist()
    vector<Mat> imagesBGR; 
    split(img, imagesBGR);

    images.push_back(img); //For this lab, just one image's Histogram is being calculated
    const int* channels[] = {0,1,2};
    Mat noMask;
    Mat hist;
    int histSize = 256;   //Number of bins of the Histogram
    const float* ranges[] = {0,255};   //Range of pixel intensities, in our case RGB has 0 to 255 intensity levels

    //2) Calculate Histogram for R, G and B Channel
    for (int i=0; i<numChannels, i++) {
        int channelIndexArray[1] = {i};
        calcHist( &images, 1, ranges, noMask, hist, 1, histSize, ranges, true, false );
    }

    
    return 0;
}*/
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

void imageEqualization(Mat img);
void showHistogram(std::vector<cv::Mat>& hists);

int main( ) {
    //1) Data loading...
    Mat img = imread("data/image.jpg", IMREAD_COLOR);
    if( !img.data ){ 
        return -1; 
    }
    resize(img, img, Size(img.cols/(1.5), img.rows/(1.5)));

    
    //Lab3 - Part 1 - Image Equalization
    imageEqualization(img);

    return 0;
}

//Given showHistogram function
void showHistogram(std::vector<cv::Mat>& hists)
{
  // Min/Max computation
  double hmax[3] = {0,0,0};
  double min;
  cv::minMaxLoc(hists[0], &min, &hmax[0]);
  cv::minMaxLoc(hists[1], &min, &hmax[1]);
  cv::minMaxLoc(hists[2], &min, &hmax[2]);

  std::string wname[3] = { "blue", "green", "red" };
  cv::Scalar colors[3] = { cv::Scalar(255,0,0), cv::Scalar(0,255,0),
                           cv::Scalar(0,0,255) };

  std::vector<cv::Mat> canvas(hists.size());

  // Display each histogram in a canvas
  for (int i = 0, end = hists.size(); i < end; i++)
  {
    canvas[i] = cv::Mat::ones(125, hists[0].rows, CV_8UC3);

    for (int j = 0, rows = canvas[i].rows; j < hists[0].rows-1; j++)
    {
      cv::line(
            canvas[i],
            cv::Point(j, rows),
            cv::Point(j, rows - (hists[i].at<float>(j) * rows/hmax[i])),
            hists.size() == 1 ? cv::Scalar(200,200,200) : colors[i],
            1, 8, 0
            );
    }

    cv::imshow(hists.size() == 1 ? "value" : wname[i], canvas[i]);
  }
}

void imageEqualization(Mat img){
    //2) Evaluating Histogram for three BGR images
    vector<Mat> images;
    split(img, images); //Splitting image into three channels images

    int numBins = 256;  //It's same for all three channels
    int histSize =  256;   //It should be histSize[] = {#binsChannel1, #binChannel2,...} but in our case they are all same
    float range[] = {0, 256}; //Range for each channel, even here it's same for all three
    const float* histRange = {range};
    Mat bHist, gHist, rHist;

    //Calculating Histograms for each channel
    calcHist( &images[0], 1, 0, Mat(), bHist, 1, &histSize, &histRange, true, false);
    calcHist( &images[1], 1, 0, Mat(), gHist, 1, &histSize, &histRange, true, false);
    calcHist( &images[2], 1, 0, Mat(), rHist, 1, &histSize, &histRange, true, false);
    
    namedWindow("Original Image", WINDOW_AUTOSIZE);
    imshow("Original Image", img);
    

    vector<Mat> hists = {bHist, gHist, rHist};
    showHistogram(hists);    //Draw the histograms

    waitKey(0);
    //3) Equalize Histograms
    for (int i=0; i<hists.size(); i++) {
        equalizeHist(images[i], images[i]);
    }

    Mat equalizedImage;
    merge(images, equalizedImage); //Merging the three images divided by channels into one
    
    namedWindow("Equalized Image", WINDOW_AUTOSIZE);
    imshow("Equalized Image", equalizedImage);

    //4) Show Histograms of Equalized Image
    vector<Mat> eq_images;
    split(equalizedImage, eq_images);
    Mat eq_bHist, eq_gHist, eq_rHist;
    calcHist( &eq_images[0], 1, 0, Mat(), eq_bHist, 1, &histSize, &histRange, true, false);
    calcHist( &eq_images[1], 1, 0, Mat(), eq_gHist, 1, &histSize, &histRange, true, false);
    calcHist( &eq_images[2], 1, 0, Mat(), eq_rHist, 1, &histSize, &histRange, true, false);

    vector<Mat> eq_hists = {eq_bHist, eq_gHist, eq_rHist};
    showHistogram(eq_hists);    //Draw the histograms
    waitKey(0);


    //5) Equalization using HSV Color Space
    Mat hsvImg;
    cvtColor(img, hsvImg, COLOR_BGR2HSV);
    vector<Mat> hsvImages;
    split(hsvImg, hsvImages);
    Mat hsv_hHist, hsv_sHist, hsv_vHist;
    calcHist( &hsvImages[0], 1, 0, Mat(), hsv_hHist, 1, &histSize, &histRange, true, false);
    calcHist( &hsvImages[1], 1, 0, Mat(), hsv_sHist, 1, &histSize, &histRange, true, false);
    calcHist( &hsvImages[2], 1, 0, Mat(), hsv_vHist, 1, &histSize, &histRange, true, false);

    vector<Mat> hsv_hists = {hsv_hHist, hsv_sHist, hsv_vHist};
    equalizeHist(hsvImages[2], hsvImages[2]);   //Equalizing just Value channel 
    Mat hsv_equalizedVChannelImage;
    merge(hsvImages, hsv_equalizedVChannelImage); //Merging the three images divided by channels into one
    
    cvtColor(hsv_equalizedVChannelImage, hsv_equalizedVChannelImage, COLOR_HSV2BGR);    //Changing back to BGR color space for visualizing purpose
    namedWindow("Equalized HSV Image", WINDOW_AUTOSIZE);
    imshow("Equalized HSV Image", hsv_equalizedVChannelImage);

    waitKey(0);



}