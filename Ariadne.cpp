#include <iostream>
#include <vector>
#include <typeinfo>
#include <string.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include "AriadneFiles/DMatchSort.h"
#include "AriadneFiles/GeoPoint.h"
#include <cmath>
int main()
{
	const uint SKIPPED_FRAMES = 1;
	const uint BEST_MATHCES = 27;
	const cv::Scalar LINECOLOR(255,0,255);
	const uint LINETHICKNESS = 2;


	cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create();
	cv::Ptr<cv::DescriptorExtractor> extractor = cv::ORB::create();
	cv::BFMatcher brue_force_matcher = cv::BFMatcher(cv::NORM_HAMMING, true);

	cv::Mat oldPicture;
	cv::Mat currentPicture; 

	cv::Mat oldDescriptors;
	cv::Mat currentDescriptors;

	cv::Mat output_image;

	std::vector<cv::DMatch > matches;
	
	std::vector< cv::KeyPoint > oldKeyPoints;
	std::vector<cv::KeyPoint > currentKeyPoints;

	// cv::VideoCapture cap("TestVideos/TankHunt.mp4");
	// cap >> oldPicture;
	cv::Mat fullSizedPicture;
	fullSizedPicture = cv::imread("TestPhotos/DSC_1954.JPG");

	detector->detect(oldPicture, oldKeyPoints);
	extractor->compute(oldPicture, oldKeyPoints, oldDescriptors);
	
	GeoPoint totalMeters;
	totalMeters.Lat = totalMeters.Long = 0;
	GeoPoint totalReal;
	totalReal.Lat = totalReal.Long = 0;

	int frameIterator = 0;

	cv::Mat demo;
	GeoPoint average;

	int img1_idx;
	int img2_idx;
	int x1=0;
	int y1=0;

	int tmpx1, tmpx2, tmpy1, tmpy2;

	cv::Rect myROI(x1, y1, 1000, 1000);
	oldPicture = fullSizedPicture(myROI);
	std::string result = "";

	int totalErrorx = 0;
	int totalErrory = 0;
	int countOfFrames = 0;
	while(1)
	{
		countOfFrames ++;
		x1+=10;
		y1+=10;
		cv::Rect myROI(x1, y1, 1000, 1000);
		currentPicture = fullSizedPicture(myROI);
		
		// for(int i = 0 ; i< SKIPPED_FRAMES ; i++)
		// {
		// 	cap >> currentPicture;
		// 	frameIterator ++;
		// }
		// if (currentPicture.empty())
		// {
	    //   	break;
		// }

		
		detector->detect(currentPicture, currentKeyPoints);
		extractor->compute(currentPicture, currentKeyPoints, currentDescriptors);

		brue_force_matcher.match((const cv::OutputArray)oldDescriptors,
			(const cv::OutputArray)currentDescriptors,  matches);

		DMatchSort::sort_matches_increasing(matches, oldKeyPoints,currentKeyPoints);
		
		//std::cout<<std::endl<<"current minlen"<<DMatchSort::sort_matches_increasing(matches, oldKeyPoints,currentKeyPoints )<<std::endl;

		if (matches.size() > BEST_MATHCES)
		{ 
			matches.resize(BEST_MATHCES);
		}

		demo = currentPicture.clone(); 
		average.Lat =0;
		average.Long = 0;
		int stepErrorX = 0;
				int stepErrorY = 0;

		for(int i = 0 ; i < matches.size(); i++)
		{
				img1_idx = matches[i].queryIdx;
    			img2_idx = matches[i].trainIdx;
				cv::line(demo, oldKeyPoints[img1_idx].pt, currentKeyPoints[img2_idx].pt, LINECOLOR, LINETHICKNESS);
				tmpx1 = oldKeyPoints[img1_idx].pt.x;
				tmpy1 = oldKeyPoints[img1_idx].pt.y;
				tmpx2 = currentKeyPoints[img2_idx].pt.x;
				tmpy2 = currentKeyPoints[img2_idx].pt.y;

				stepErrorX += std::abs(10-(tmpx2-tmpx1));
				stepErrorY += std::abs(10-(tmpy2-tmpy1));

				average.Lat += tmpx2-tmpx1;
				average.Long += tmpy2-tmpy1;
		}
		std::cout<<matches.size()<<std::endl;
		if(matches.size()!=0)
		{
		totalErrorx += stepErrorX/ matches.size();
		totalErrory += stepErrorY/ matches.size();
		}

		average.Lat /= matches.size();
		average.Long /= matches.size();

		totalMeters.Lat += average.Lat;
		totalMeters.Long += average.Long;

		// std::cout<<"average on this step" << average.Lat << " " << average.Long << std::endl;
		// std::cout<<"totalMeters " << totalMeters.Lat << " " << totalMeters.Long << std::endl;
		// std::cout<<"total average" << totalMeters.Lat/frameIterator <<  " " << totalMeters.Long/frameIterator << std::endl;
		//video.write(demo);
		imshow("demo", demo);
		oldPicture = currentPicture;
		oldKeyPoints = currentKeyPoints;
		oldDescriptors = currentDescriptors;
		char c = (char)cv::waitKey(25);
        if (c == 27)
            break;
	}
	std::cout<<totalErrorx/countOfFrames << " "<< totalErrory/countOfFrames <<std::endl;
	extractor.release();
	detector.release();
}