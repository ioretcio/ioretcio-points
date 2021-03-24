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
	cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create();
	cv::Ptr<cv::DescriptorExtractor> extractor = cv::ORB::create();
	cv::Mat allArea = cv::imread("/home/pi/ariadne/imageee.JPG");
	cv::BFMatcher brue_force_matcher = cv::BFMatcher(cv::NORM_HAMMING, true);
	cv::Mat oldPicture, currentPicture, oldDescriptors, currentDescriptors,output_image;

	std::vector<cv::DMatch > matches;
	std::vector<cv::KeyPoint > currentKeyPoints;
	//VideoCapture cap("/home/pi/altair/tank.mp4");
	//cap >> oldPicture;

	int xxx = 0;
	int yyy = 0;
	int picsize = 600;
	//
	oldPicture = allArea(cv::Rect(xxx,yyy,picsize,picsize));
	//
	std::vector< cv::KeyPoint > oldKeyPoints;
	detector->detect(oldPicture, oldKeyPoints);
	extractor->compute(oldPicture, oldKeyPoints, oldDescriptors);
	//
	int skips = 3;
	//
	//
	GeoPoint totalMeters;
	totalMeters.Lat = totalMeters.Long = 0;
	GeoPoint totalReal;
	totalReal.Lat = totalReal.Long = 0;
	//
	//
	int count_of_frames = 0;
	while(1)
	{
		std::cout<<"\n\n\n";
		if(count_of_frames<10*2)
		{
			yyy += 40;
		}
		if(count_of_frames>=10*2&&count_of_frames<20*2)
		{
			yyy -= 40;
		}
		if(count_of_frames>=20*2&&count_of_frames<30*2)
		{
			xxx += 40;
		}
		if(count_of_frames>=30*2&&count_of_frames<40*2)
		{
			xxx -= 40;
		}
		if(count_of_frames>=40*2)
			break;
		for(int i=0; i< skips; i++)
			currentPicture = allArea(cv::Rect(xxx,yyy,picsize,picsize));
		//for(int i = 0 ; i< skips ; i++)
		//	cap >> currentPicture;
		detector->detect(currentPicture, currentKeyPoints);
		extractor->compute(currentPicture, currentKeyPoints, currentDescriptors);
		brue_force_matcher.match((const cv::OutputArray)oldDescriptors, (const cv::OutputArray)currentDescriptors,  matches);
	


		std::cout<<"current minlen"<<DMatchSort::sort_matches_increasing(matches, oldKeyPoints,currentKeyPoints )<<std::endl;

		int machlim = 10;
		if (matches.size() > machlim) matches.resize(machlim);

		cv::Scalar colorLine(0,255,0); // Green
		int thicknessLine = 2;

		cv::Mat demo = currentPicture.clone(); 

		GeoPoint average;
		average.Lat =0;
		average.Long = 0;

		for(int i = 0 ; i < matches.size(); i++)
		{
				int img1_idx = matches[i].queryIdx;
    			int img2_idx = matches[i].trainIdx;
				cv::line(demo, oldKeyPoints[img1_idx].pt, currentKeyPoints[img2_idx].pt, colorLine, thicknessLine);
				int x1 = oldKeyPoints[img1_idx].pt.x;
				int y1 = oldKeyPoints[img1_idx].pt.y;
				int x2 = currentKeyPoints[img2_idx].pt.x;
				int y2 = currentKeyPoints[img2_idx].pt.y;

				average.Lat += x2-x1;
				average.Long += y2-y1;
		}
		average.Lat /= matches.size();
		average.Long /= matches.size();

		totalMeters.Lat += average.Lat;
		totalMeters.Long += average.Long;
		count_of_frames ++;
		std::cout<<"average on this step" << average.Lat << " " << average.Long << std::endl;
		std::cout<<"totalMeters " << totalMeters.Lat << " " << totalMeters.Long << std::endl;
		std::cout<<"total average" << totalMeters.Lat/count_of_frames <<  " " << totalMeters.Long/count_of_frames << std::endl;
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