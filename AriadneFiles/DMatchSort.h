#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
class DMatchSort
{
public:
    int seventy_two = 72;
    static int sort_matches_increasing(std::vector<cv::DMatch >& matches, std::vector< cv::KeyPoint > oldKeyPoints, std::vector< cv::KeyPoint > currentKeyPoints );
};