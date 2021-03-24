#include <iostream>
#include <vector>
#include <typeinfo>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include "AriadneFiles/DMatchSort.h"
#include "AriadneFiles/GeoPoint.h"

int main()
{
	const uint SKIPPED_FRAMES = 3;
	const uint BEST_MATHCES = 10;


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

	cv::VideoCapture cap("TestVideos/TankHunt.mp4");
	cap >> oldPicture;

	detector->detect(oldPicture, oldKeyPoints);
	extractor->compute(oldPicture, oldKeyPoints, oldDescriptors);
	
	GeoPoint totalMeters;
	totalMeters.Lat = totalMeters.Long = 0;
	GeoPoint totalReal;
	totalReal.Lat = totalReal.Long = 0;

	int frameIterator = 0;
	const cv::Scalar LINECOLOR(0,255,0);
	const uint LINETHICKNESS = 2;
	cv::Mat demo;
	GeoPoint average;

	int img1_idx;
	int img2_idx;
	int x1;
	int y1 ;
	int x2;
	int y2;

	int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

	cv::VideoWriter video("TestVideos/outcpp.avi",cv::CV_FOURCC('M','J','P','G'),10, cv::Size(frame_width,frame_height));

	while(1)
	{
		for(int i = 0 ; i< SKIPPED_FRAMES ; i++)
		{
			cap >> currentPicture;
			frameIterator ++;
		}

		detector->detect(currentPicture, currentKeyPoints);
		extractor->compute(currentPicture, currentKeyPoints, currentDescriptors);
		brue_force_matcher.match((const cv::OutputArray)oldDescriptors, (const cv::OutputArray)currentDescriptors,  matches);
		std::cout<<std::endl<<"current minlen"<<DMatchSort::sort_matches_increasing(matches, oldKeyPoints,currentKeyPoints )<<std::endl;

		if (matches.size() > BEST_MATHCES)
		{ 
			matches.resize(BEST_MATHCES);
		}

		demo = currentPicture.clone(); 
		average.Lat =0;
		average.Long = 0;

		for(int i = 0 ; i < matches.size(); i++)
		{
				img1_idx = matches[i].queryIdx;
    			img2_idx = matches[i].trainIdx;
				cv::line(demo, oldKeyPoints[img1_idx].pt, currentKeyPoints[img2_idx].pt, LINECOLOR, LINETHICKNESS);
				x1 = oldKeyPoints[img1_idx].pt.x;
				y1 = oldKeyPoints[img1_idx].pt.y;
				x2 = currentKeyPoints[img2_idx].pt.x;
				y2 = currentKeyPoints[img2_idx].pt.y;
				average.Lat += x2-x1;
				average.Long += y2-y1;
		}

		average.Lat /= matches.size();
		average.Long /= matches.size();

		totalMeters.Lat += average.Lat;
		totalMeters.Long += average.Long;

		std::cout<<"average on this step" << average.Lat << " " << average.Long << std::endl;
		std::cout<<"totalMeters " << totalMeters.Lat << " " << totalMeters.Long << std::endl;
		std::cout<<"total average" << totalMeters.Lat/frameIterator <<  " " << totalMeters.Long/frameIterator << std::endl;
		imshow("demo", demo);
		oldPicture = currentPicture;
		oldKeyPoints = currentKeyPoints;
		oldDescriptors = currentDescriptors;
		char c = (char)cv::waitKey(25);
        if (c == 27)
            break;
	}
	extractor.release();
	detector.release();
}