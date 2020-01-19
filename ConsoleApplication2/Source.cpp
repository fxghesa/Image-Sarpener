#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

Mat textBefore(Mat frame){
    putText(frame, 
            "Sebelum",
            Point(5,15), // Coordinates
            FONT_HERSHEY_COMPLEX_SMALL, // Font
            1.0, // Scale. 2.0 = 2x bigger
            Scalar(255,255,255), // Color
            1, // Thickness
            CV_AA); //anti alias
	return frame;
}

Mat textFilter(Mat frame){
    putText(frame, 
            "Filter",
            Point(5,15), // Coordinates
            FONT_HERSHEY_COMPLEX_SMALL, // Font
            1.0, // Scale. 2.0 = 2x bigger
            Scalar(255,255,255), // Color
            1, // Thickness
            CV_AA); //anti alias
	return frame;
}

Mat textAfter(Mat frame){
    putText(frame, 
            "Sesudah",
            Point(5,15), // Coordinates
            FONT_HERSHEY_COMPLEX_SMALL, // Font
            1.0, // Scale. 2.0 = 2x bigger
            Scalar(255,255,255), // Color
            1, // Thickness
            CV_AA); //anti alias
	return frame;
}

int SliderValue1 = 4;
int SliderValue2 = 4;
int n = 0, jangkar = -4;
int a = 0;
Mat pengatur(Mat src){
	namedWindow("Sesudah", CV_WINDOW_AUTOSIZE);
	createTrackbar("Kecerahan", "Sesudah", &SliderValue1, 25);
	createTrackbar("Kontras", "Sesudah", &SliderValue2, 8);
	createTrackbar("Kernel", "Sesudah", &n, 1);
	Mat hasil;
	int Brightness = SliderValue1 - 5;
    double Contrast = SliderValue2 / 5.0;
	switch(n){
		case 0 :
			jangkar = -4;
			a = 0;
			break;
		case 1 :
			jangkar = -8;
			a = 1;
			break;
	}
    src.convertTo(hasil,-1,Contrast,Brightness);
	imshow("Sesudah", hasil);
	return hasil;
}

void pentajam(Mat img){
	//img = imread("lena.png"); //untuk mengecek gambar HD

	Mat imgLaplacian, imgResult;

	// pilihan kernel :
	// manual kernel
    Mat kernel = (Mat_<float>(3,3) << 
			a,  1, a,
			1, jangkar, 1,
			a,  1, a); 

	// laplace kernel
    Mat kernel1 = (Mat_<float>(3,3) << 
			0,  1, 0,
			1, -4, 1,
			0,  1, 0); 

	// Stronger laplace kernel https://stackoverflow.com/users/433685/nkint
	Mat kernel2 = (Mat_<float>(3,3) << 
        1,  1, 1,
        1, -8, 1,
        1,  1, 1);

	//wikipedia sharpen kernel https://en.wikipedia.org/wiki/Kernel_(image_processing)
	Mat kernel3 = (Mat_<float>(3,3) << 
			0,  -1, 0,
			-1, -5, -1,
			0,  -1, 0);

	// Identity kernel https://en.wikipedia.org/wiki/Identity_operation
	Mat kernel4 = (Mat_<float>(3,3) << 
			0,  0, 0,
			0, 1, 0,
			0,  0, 0);

	// cetak kernel
	printf("Gambar dikonvolusi dengan : \n");
	printf("| %d  1  %d |\n",a,a);
	printf("| 1 %d  1 |\n",jangkar);
	printf("| %d  1  %d |\n",a,a);
	//system("cls");

	// konversi depth laplace
	bool laplace = true;
	if(laplace == true){
		//printf("Pakai laplace \n");
		// konversi CV_32F 2nd derivate
		filter2D(img, imgLaplacian, CV_32F, kernel);
		img.convertTo(img, CV_32F);
		imgResult = img - imgLaplacian;

		// konversi kembali 8bits
		img.convertTo(img, CV_8U);
		imgResult.convertTo(imgResult, CV_8U);
		imgLaplacian.convertTo(imgLaplacian, CV_8U);
	} else {
		//printf("Tanpa laplace \n");
		filter2D(img, imgLaplacian, img.depth(), kernel3);
		imgResult = img - imgLaplacian;
	}
    
	//tampilkan semua gambar
	textBefore(img);
	imshow( "Sebelum", img);
	namedWindow("Sebelum", CV_WINDOW_AUTOSIZE);

	textFilter(imgLaplacian);
    namedWindow("Filter", CV_WINDOW_AUTOSIZE);
    imshow( "Filter", imgLaplacian );

	textAfter(imgResult);
	pengatur(imgResult);
}

int main(int, char**){
	Mat normal,hasil;
    VideoCapture cap(0); // 0 = webcam, 1 = cam external
    if(!cap.isOpened()) 
        return -1;
	char dataKeyboard;
    while(true){
        cap >> normal;
		pentajam(normal);
		dataKeyboard = cvWaitKey(10);
    }
	waitKey();
    return 0;
}
