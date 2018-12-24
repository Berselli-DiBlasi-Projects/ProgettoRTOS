#include "../lib/image_processing.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "../lib/FrmMain.h"

using namespace std;
using namespace cv;

FrmMain *frm = 0;

struct ScreenShot
{
    ScreenShot(int x, int y, int width, int height):
        x(x),
        y(y),
        width(width),
        height(height)
    {
        display = XOpenDisplay(nullptr);
        root = DefaultRootWindow(display);

        init = true;
    }

    void operator() (Mat& cvImg)
    {
        if(init == true)
            init = false;
        else
            XDestroyImage(img);

        img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);

        cvImg = Mat(height, width, CV_8UC4, img->data);
    }

    ~ScreenShot()
    {
        if(init == false)
            XDestroyImage(img);

        XCloseDisplay(display);
    }

    Display* display;
    Window root;
    int x,y,width,height;
    XImage* img;

    bool init;
};

/* Funzione per selezionare la regione in cui definire la camera. */
/* Ritorna le coordinate della regione tracciata dall'utente     */
Rect2d select_region(Gtk::Label *lblState)
{
    Display* disp = XOpenDisplay(NULL);
    Screen*  scrn = DefaultScreenOfDisplay(disp);
    int width  = scrn->width;
    int height = scrn->height;
    ScreenShot screen(0, 0, scrn->width, scrn->height);
    Mat img;
    Rect2d rect;
	string title = "Select area (SPACE or ENTER to confirm)";
    screen(img);

    // Select ROI: rect.x, rect.y, rect.x + rect.width,
    // rect.y + regt.height
    do {
    rect = selectROI(title, img);

    if(rect.width > MAX_WIDTH || rect.height > MAX_HEIGHT)
        cout << "Error: maximum ROI dimensions exceeded. (MAX_WIDTH: "<< 
            MAX_WIDTH << ", MAX_HEIGHT: " << MAX_HEIGHT << ")" << endl;

    } while(rect.width > MAX_WIDTH || rect.height > MAX_HEIGHT);
    destroyWindow(title);
    lblState->set_text("Confirm to display a\npreview of the execution.");

    return rect;

    /* Codice valido per il thread camera */
    /* bool area_selected = false;
    char exit_key_press = 0;
	do
	{
		screen(img);
    	imshow(title, img);
        if(!frm->is_visible())
            break;
        exit_key_press = cvWaitKey(27);
        waitKey(1);
	}while(cvGetWindowHandle(title.c_str()) && exit_key_press != 27);
    if(exit_key_press == 27)
        destroyWindow(title);*/
}

/* Funzione che visualizza un dato frame */
void show_main_frame(string glade_file, string frame_id, string title,
     string icon_file, int argc, char** argv)
{
    cout << "Started" << endl;

	Gtk::Main kit(argc, argv);
	Glib::RefPtr<Gtk::Builder> builder = 
        Gtk::Builder::create_from_file(glade_file);

	builder->get_widget_derived(frame_id, frm);
	frm->set_title(title);
	frm->set_icon_from_file(icon_file);
	frm->property_resizable() = false;
	kit.run(*frm);

	cout << "End" << endl;
}

void preview(Rect2d rect)
{
    ScreenShot screen(rect.x, rect.y, rect.width, rect.height);
    Mat imgCamera;
	string titleCamera = "Camera";
    screen(imgCamera);
	
    while(true)
	{
		screen(imgCamera);
    	imshow(titleCamera, imgCamera);
        if(!frm->is_visible())
            break;
        waitKey(1);
    }
}

/**
 * Calcola l'istogramma dei colori dello spettro RGB di un'immagine
 * @param   : Mat src immagine da processare
 * @return  : null
*/
void histogram( Mat src){


    //! [Separate the image in 3 places ( B, G and R )]
    vector<Mat> bgr_planes;
    split( src, bgr_planes );
    //! [Separate the image in 3 places ( B, G and R )]

    //! [Establish the number of bins]
    int histSize = 256;
    //! [Establish the number of bins]

    //! [Set the ranges ( for B,G,R) )]
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange = { range };
    //! [Set the ranges ( for B,G,R) )]

    //! [Set histogram param]
    bool uniform = true, accumulate = false;
    //! [Set histogram param]

    //! [Compute the histograms]
    Mat b_hist, g_hist, r_hist;
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
    //! [Compute the histograms]

    //! [Draw the histograms for B, G and R]
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    //! [Draw the histograms for B, G and R]

    //! [Normalize the result to ( 0, histImage.rows )]
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    //! [Normalize the result to ( 0, histImage.rows )]

    //! [Draw for each channel]
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
              Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
              Scalar( 0, 0, 255), 2, 8, 0  );
    }
    //! [Draw for each channel]

    imshow("calcHist Demo", histImage );
    //waitKey();
    //! [Display]

    return;
}


//deve prendere in ingresso il gestore della memoria condivisa
void camera(Rect2d rect){
    ScreenShot screen(rect.x, rect.y, rect.width, rect.height);
    Mat imgCamera;
	string titleCamera = "Camera";
    screen(imgCamera);
	/*
    la camera cattura frame alla massima velocità
    inserisce lo screenshot nell'oggetto del gestore della
    memoria condivisa.
    
    */
    while(true)
	{

		screen(imgCamera);
    	//--- questo pezzo andrà cancellato ----
        imshow(titleCamera, imgCamera);
        //--- fine codice da cancellare     ----

        if(!frm->is_visible())
            break;

        waitKey(1);
    }
}