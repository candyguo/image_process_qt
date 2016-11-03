#include "laplacian.h"

laplacian::laplacian():aperture(3)
{

}

void laplacian::set_aperture(int a)
{
    aperture=a;
}

 Mat laplacian::computeLaplacian(const cv::Mat&image)
 {
     Laplacian(image,lapla,CV_32F,aperture);//对图像进行拉普拉斯变换
     // Keep local copy of the image
     // (used for zero-crossings)
     image_lp= image.clone();
     return lapla;
 }

Mat laplacian::getLaplacianImage(double scale)
 {
    if (scale<0) {
    double lapmin, lapmax;
    cv::minMaxLoc(lapla,&lapmin,&lapmax);
    scale= 127/ std::max(-lapmin,lapmax);
    }
    Mat laplaceImage;
    lapla.convertTo(laplaceImage,CV_8U,scale,128);
    return laplaceImage;
 }

Mat laplacian::getZeroCrossings(float threshold) {
// Create the iterators
    Mat_<float>::const_iterator it=
            lapla.begin<float>()+lapla.step1();
    Mat_<float>::const_iterator itend=
            lapla.end<float>();
    Mat_<float>::const_iterator itup=
            lapla.begin<float>();
    // Binary image initialize to white
    Mat binary(lapla.size(),CV_8U,cv::Scalar(255));
    Mat_<uchar>::iterator itout=
            binary.begin<uchar>()+binary.step1();
    for ( ; it!= itend; ++it, ++itup, ++itout) {
        // if the product of two adjascent pixel is
        // negative then there is a sign change
        if (*it * *(it-1) < threshold)
            *itout= 0; // horizontal zero-crossing
        else if (*it * *itup < threshold)
            *itout= 0; // vertical zero-crossing
    }
    return binary;
}
