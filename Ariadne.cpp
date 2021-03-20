#include <iostream>
#include <vector>
#include <typeinfo>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <math.h>
//#include <mavsdk/mavsdk.h>
//
//
//
using namespace std;
using namespace cv;
//
//
const int RTerra = 6378137;
//
//
struct TerraPoint{
	double Lat;
	double Long;
};
TerraPoint getOffset(TerraPoint start, double xmOffset, double ymOffset)
{
	TerraPoint result;
	result.Lat = xmOffset / RTerra;
	result.Long = ymOffset/(RTerra*cos(M_PI*start.Lat/180));
	return result;
}
double GetHeight(Mat image)
{
	Mat img, img_gray, channel[3];
	img = image.clone();
	double distance = 0;
	cvtColor(img, img_gray, COLOR_RGB2GRAY);

	split(img, channel);
	subtract(channel[2], img_gray, img_gray);
	threshold(img_gray, img_gray, 90, 255, THRESH_BINARY);
	erode(img_gray, img_gray, Mat(), Point(-1,-1), 4);
	dilate(img_gray, img_gray, Mat(), Point(-1,-1), 4);

	vector<vector<Size>> contors;
	vector<Vec4i> heirarcy;
	findContours(img_gray, contors, heirarcy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0,0));
	vector<Rect> boundRect(contors.size());
		vector<vector<Point>> contor_poly(contors.size());
	
		for(int i =0; i< contors.size(); i++)
		{
			approxPolyDP(Mat(contors[i]), contor_poly[i], 3, true);
			boundRect[i] = boundingRect(Mat(contor_poly[i]));
		}
		int max_index = 0,max_area = 0;
		for(int i =0; i< boundRect.size(); i++)
		{
			int a = boundRect[i].area(); 
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(255,255,0), 2, 8, 0);
			if( a > max_area)
			{ 
				max_area = a;
				max_index = i;
			}
		}
		int confidence = 0;
		for(int i=0; i< boundRect.size(); i++)
		{
			if((boundRect[i].x < boundRect[max_index].x + boundRect[max_index].width && boundRect[i].x > boundRect[max_index].x - int(0.1*boundRect[max_index].width)) && (boundRect[i].y > boundRect[max_index].y) )
				confidence += 45; 
		}
		if(boundRect.size() > 0)
		{	
			if(confidence > 99)
				confidence = 0;
			distance = 8414.7*pow(boundRect[max_index].area(), -0.468);
			cout << distance << " cm." <<" Confidence: "<<confidence<<endl;
			imshow("Frame", img);
			
		}
	return distance;
}
int sort_matches_increasing(vector< DMatch >& matches, vector< KeyPoint > oldKeyPoints, vector< KeyPoint > currentKeyPoints )//сорт збігів, щоб уникнути випадкових (чим ближче - тим краще)
{
	int minlen = 1000;
	for (int i = 0; i < matches.size(); i++)
		for (int j = 0; j < matches.size() - 1; j++)
		{
			int img1_idx = matches[j].queryIdx;
    		int img2_idx = matches[j].trainIdx;
			int x1 = oldKeyPoints[img1_idx].pt.x;
			int y1 = oldKeyPoints[img1_idx].pt.y;
			int x2 = currentKeyPoints[img2_idx].pt.x;
			int y2 = currentKeyPoints[img2_idx].pt.y;

			int dist1 = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
			
			img1_idx = matches[j+1].queryIdx;
    		img2_idx = matches[j+1].trainIdx;
			x1 = oldKeyPoints[img1_idx].pt.x;
			y1 = oldKeyPoints[img1_idx].pt.y;
			x2 = currentKeyPoints[img2_idx].pt.x;
			y2 = currentKeyPoints[img2_idx].pt.y;

			int dist2 = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));

			//cout << "compare" << dist1 << "and" << dist2 << endl;
			if (dist1 > dist2)
			{
				if(minlen>dist2)
					minlen=dist2;
				auto temp = matches[j];
				matches[j] = matches[j + 1];
				matches[j + 1] = temp;
			}
		}
		return minlen;
}

int main()
{
	//
	//
	Ptr<FeatureDetector> detector = ORB::create();
	Ptr<DescriptorExtractor> extractor = ORB::create();
	//
	//
	Mat allArea = imread("/home/pi/ariadne/imageee.JPG");
	//
	//
	BFMatcher brue_force_matcher = BFMatcher(NORM_HAMMING, true);
	Mat oldPicture, currentPicture, oldDescriptors, currentDescriptors,output_image;
	//
	//
	vector< DMatch > matches;
	vector< KeyPoint > currentKeyPoints;
	//VideoCapture cap("/home/pi/altair/tank.mp4");
	//cap >> oldPicture;
	//
	//
	int xxx = 0;
	int yyy = 0;
	int picsize = 600;
	//
	oldPicture = allArea(Rect(xxx,yyy,picsize,picsize));
	//
	vector< KeyPoint > oldKeyPoints;
	detector->detect(oldPicture, oldKeyPoints);
	extractor->compute(oldPicture, oldKeyPoints, oldDescriptors);
	//
	int skips = 3;
	//
	//
	TerraPoint totalMeters;
	totalMeters.Lat = totalMeters.Long = 0;
	TerraPoint totalReal;
	totalReal.Lat = totalReal.Long = 0;
	//
	//
	int count_of_frames = 0;
	while(1)
	{
		cout<<"\n\n\n";
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
			currentPicture = allArea(Rect(xxx,yyy,picsize,picsize));
		GetHeight(currentPicture);
		//for(int i = 0 ; i< skips ; i++)
		//	cap >> currentPicture;
		detector->detect(currentPicture, currentKeyPoints);
		extractor->compute(currentPicture, currentKeyPoints, currentDescriptors);
		brue_force_matcher.match((const OutputArray)oldDescriptors, (const OutputArray)currentDescriptors,  matches);
		cout<<"current minlen"<< sort_matches_increasing(matches, oldKeyPoints,currentKeyPoints )<<endl;


		//
		//
		int machlim = 10;
		if (matches.size() > machlim) matches.resize(machlim);
		//
		//
		
		//
		//
		Scalar colorLine(0,255,0); // Green
		int thicknessLine = 2;
		//
		//
		Mat demo = currentPicture.clone(); 
		//
		//
		TerraPoint average;
		average.Lat =0;
		average.Long = 0;
		//
		//
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

				//cout << x2-x1 << " " << y2-y1 << endl;
		}
		average.Lat /= matches.size();
		average.Long /= matches.size();

		totalMeters.Lat += average.Lat;
		totalMeters.Long += average.Long;
		count_of_frames ++;
		cout<<"average on this step" << average.Lat << " " << average.Long << endl;
		cout<<"totalMeters " << totalMeters.Lat << " " << totalMeters.Long << endl;
		cout<<"total average" << totalMeters.Lat/count_of_frames <<  " " << totalMeters.Long/count_of_frames << endl;
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