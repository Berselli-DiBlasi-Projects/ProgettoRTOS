#include "../lib/image_processing.h"


Mat filterFrame(Mat img){
	Mat im_out = img, thr;
	//kernel per il filtro seppia
	cv::Mat kern = (cv::Mat_<float>(4,4) <<  0.272, 0.534, 0.131, 0,
                                             0.349, 0.686, 0.168, 0,
                                             0.393, 0.769, 0.189, 0,
                                             0, 0, 0, 1); 

	switch(getFilterChoice()){
		case 0:
			break;
		case 1: //seppia filter
				cv::transform(img, im_out, kern);
				break;

		case 2:	//scala di grigi
				cvtColor(img,im_out,cv::COLOR_RGB2GRAY);
				break;

		case 3: //bianco e nero
				//conversione in scala di grigi
				cvtColor(img, im_out, cv::COLOR_RGB2GRAY);
				//applicazione della soglia
				threshold(im_out, thr, 100,255,THRESH_BINARY );
				im_out = thr.clone();
				break;
		default:
			break;

	}

	return im_out;

	
}
