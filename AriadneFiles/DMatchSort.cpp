#include "DMatchSort.h"

int DMatchSort::sort_matches_increasing(std::vector< cv::DMatch >& matches, std::vector<  cv::KeyPoint > oldKeyPoints, std::vector< cv::KeyPoint > currentKeyPoints )
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