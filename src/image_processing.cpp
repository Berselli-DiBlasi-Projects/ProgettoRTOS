#include "../lib/image_processing.h"

using namespace std;
using namespace cv;

#define STATE_IDLE      0
#define STATE_ANALISYS  1
#define PER     20      /* task period in ms    */
#define DREL    20     /* realtive deadline in ms	*/
#define PRIO    80     /* task priority		*/
#define MAX_WIDTH   1920
#define MAX_HEIGHT  1080

bool cancel_signal = false;
bool execution_started = false;
int filter_choice = 0;
bool begin_difference = true;
int frame_difference_value = 0;
int frame_difference_count = 0;
Mat img_difference_old;
Mat img_difference_out;
int frame_scaling_value = 100;
int threshold_type;
int threshold_value;
int fps_value;
int bit_for_channel;
bool filtering_active = true;
bool frame_difference_active = true;
bool threshold_active = true;
bool histogram_active = true;

/**
 * Struct che definisce uno screenshot dello schermo.
*/
struct ScreenShot
{
    Display* display;
    Window root;
    int x,y,width,height;
    XImage* img;
    bool init;

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
        if (init == true)
            init = false;
        else
            XDestroyImage(img);

        img = XGetImage(display, root, x, y, width, height, 
                        AllPlanes, ZPixmap);

        cvImg = Mat(height, width, CV_8UC4, img->data);
    }

    ~ScreenShot()
    {
        if (init == false)
            XDestroyImage(img);

        XCloseDisplay(display);
    }
};

/**
 * Struct che contiene le risorse condivise dei thread di elaborazione.
*/
struct gestore_t
{
    //mutua esclusione
    pthread_mutex_t mutex;
    
    //private semaphore
    pthread_mutex_t sem_camera, 
                    sem_histo, 
                    sem_filter, 
                    sem_threshold,
                    sem_difference;
    int nhist, ndiff, nthres, nfilter;  //numero thread di analisi attivi
    int bhist, bdiff, bthres, bfilter;  //numero thread di analisi bloccati
    int ncam;   //numero camere attive
    int state;  //stato del sistema

} gestore;

/**
 * Struct che contiene le risorse di output condivise.
*/
struct output_g
{
	Rect2d rect;
	Mat histogram;
	Mat filter;
	Mat diff;
	Mat thr;
	Mat cam;
	pthread_mutex_t sem_camera, 
                sem_histo, 
                sem_filter, 
                sem_threshold,
                sem_difference;
} output;

/**
 * Ritorna il bool execution_started, di supporto all'esecuzione.
 * @param   : none.
 * @return  : bool.
*/
bool getExecutionStarted()
{
	return execution_started;
}

/**
 * Riduce le dimensioni dell'immagine prendendo come 
 * parametro di riduzione quello impostato dall'utente
 * in frame settings.
 * @param : Mat src; matrice da ridimensionare.
 * @return : Mat dst; resized Mat.
*/
Mat imageScale(Mat src)
{
	float reduction;
    cv::Size s;
	Mat dst;

    reduction = (float) frame_scaling_value;

	if (reduction != 100)
	{
		reduction = reduction / 100;
		s = src.size();
		s.height = s.height * reduction;
		s.width = s.width * reduction;
		cv::resize(src, dst, s);
	}
	else
		dst = src.clone();
	
	return dst;
}

/**
 * Crea le finestre di preview dell'esecuzione.
 * @param   : frmMain; riferimento al frmMain chiamante.
 * @return  : void.
*/
void preview(FrmMain *frmMain)
{
    Mat placeholder_image, placeholder_image_resized;
	Rect2d rect;
	Mat imgCamera;
	string titleCamera;
    time_t start, time_taken;
    int count, i, ret_value;

    ret_value = 1;
    rect = getRect();
    titleCamera = "Camera";
    ScreenShot screen(rect.x, rect.y, rect.width, rect.height);
    screen(imgCamera);
	cancel_signal = false;

	placeholder_image = imread("../media/placeholder.png",
								IMREAD_COLOR);

	resize(placeholder_image, placeholder_image_resized,
		   rect.size(), 0, 0, cv::INTER_CUBIC);
	string placeholder_titles[4];

	placeholder_titles[0] = "Histogram";
	placeholder_titles[1] = "Filter";
	placeholder_titles[2] = "Frame difference";
	placeholder_titles[3] = "Threshold";

    start = time(0);
    time_taken = 0;
    count = 0;
	do {
		time_t end;
        end = time(0);

		if (time_taken != end - start) {
			cout << "FPS: " << count << endl;
			count = 0;
		}else{
			count++;
        }
		time_taken = end - start;
		screen(imgCamera);
		imshow(titleCamera, imgCamera);
		//Mostra le finestre dei placeholders
		for (i = 0; i < 
			sizeof(placeholder_titles) / sizeof(placeholder_titles[0]); i++)
			imshow(placeholder_titles[i], placeholder_image_resized);
		if (!frmMain->is_visible() || cancel_signal)		{
			destroyAllWindows();
			ret_value = 0;
		}
		waitKey(16);    //16 ms -> limit to 60 FPS.
	} while(ret_value);
}

/**
 * Permette all'utente di selezionare l'area di schermo da registrare.
 * @param   : Gtk::Label *lblState; label di stato del frame FrmMain.
 * @return  : Rect2d; Il rettangolo che definisce l'area scelta.
*/
void selectRegion(Gtk::Label *lblState)
{
	Display* disp;
	Screen*  scrn;
	int width;
	int height;
	Mat img;
	Rect2d rect;
	string title;
    
    disp = XOpenDisplay(NULL);
    scrn = DefaultScreenOfDisplay(disp);
    width = scrn->width;
    height = scrn->height;
    title = "Select area (SPACE or ENTER to confirm)";

    ScreenShot screen(0, 0, scrn->width, scrn->height);
	screen(img);
	
	do {
		rect = selectROI(title, img);

		if (rect.width > MAX_WIDTH || rect.height > MAX_HEIGHT)
			cout << "Error: maximum ROI dimensions exceeded. (MAX_WIDTH: "<<
				MAX_WIDTH << ", MAX_HEIGHT: " << MAX_HEIGHT << ")" << endl;

	} while (rect.width > MAX_WIDTH || rect.height > MAX_HEIGHT);
	destroyWindow(title);
	lblState->set_text("Confirm to display a\npreview of the execution.");

	setRect(rect);
}

/**
 * Setta il bool cancel_signal, di supporto alla funzione preview.
 * @param   : bool signal; Il booleano da utilizzare.
 * @return  : void.
*/
void setCancelSignal(bool signal)
{
	cancel_signal = signal;
}

/**
 * Setta il bool execution_started, di supporto all'esecuzione.
 * @param   : bool started; Il booleano da utilizzare.
 * @return  : void.
*/
void setExecutionStarted(bool started)
{
	execution_started = started;
}

/**
 * Setta il valore di filter_choice per il filtering.
 * @param   : int; choice
 * @return  : void.
*/
void setFilterChoice(int choice)
{
	filter_choice = choice;
}

/**
 * Setta il valore di frame_difference, di supporto all'esecuzione.
 * @param   : int difference; Il valore da utilizzare.
 * @return  : void.
*/
void setFrameDifferenceValue(int difference)
{
	frame_difference_value = difference;
}

/**
 * Setta il valore di frame_scaling, di supporto all'esecuzione.
 * @param   : int scaling; Il valore da utilizzare.
 * @return  : void.
*/
void setFrameScalingValue(int scaling)
{
	frame_scaling_value = scaling;
}

/**
 * Setta il valore di threshold_type, di supporto all'esecuzione.
 * @param   : int value; Il valore da utilizzare.
 * @return  : void.
*/
void setThresholdType(int value)
{
	threshold_type = value;
}

/**
 * Setta il valore di threshold, di supporto all'esecuzione.
 * @param   : int value; Il valore da utilizzare.
 * @return  : void.
*/
void setThresholdValue(int value)
{
	threshold_value = value;
}

//_____________________________________________________________________________
//_____________________________________________________________________________

//				FUNZIONI PER LA GESTIONE DEI THREAD DI ANALISI

//_____________________________________________________________________________
//_____________________________________________________________________________

/**
 * Inizializza la struct gestore_t.
 * @param   : struct gestore_t *g; Riferimento all'oggetto struct gestore_t
 *                                 da inizializzare.
 * @return  : void.
*/
void initGestore(struct gestore_t *g)
{
    pthread_mutex_init(&g->mutex, NULL);
    pthread_mutex_init(&g->sem_camera, NULL);
    pthread_mutex_init(&g->sem_filter, NULL);
    pthread_mutex_init(&g->sem_histo, NULL);
    pthread_mutex_init(&g->sem_threshold, NULL);
    pthread_mutex_init(&g->sem_difference, NULL);
    
    g->ncam = 0;
    g->nhist = g->ndiff = g->nthres = g->nfilter = 0;
    g->bhist = g->bdiff = g->bthres = g->bfilter = 0;

    pthread_mutex_lock(&g->sem_camera);
    pthread_mutex_lock(&g->sem_filter);
    pthread_mutex_lock(&g->sem_histo);
    pthread_mutex_lock(&g->sem_threshold);
    pthread_mutex_lock(&g->sem_difference);

    g->state=STATE_IDLE;

    ptask_init(SCHED_RR, GLOBAL, NO_PROTOCOL);
}

/**
 * Ritorna il valore di filtering_active.
 * @param   : none.
 * @return  : bool.
*/
bool getFilteringActive()
{
    return filtering_active;
}

/**
 * Ritorna il valore di frame_difference_active.
 * @param   : none.
 * @return  : bool.
*/
bool getFrameDifferenceActive()
{
    return frame_difference_active;
}

/**
 * Ritorna il valore di histogram_active.
 * @param   : none.
 * @return  : bool.
*/
bool getHistogramActive()
{
    return histogram_active;
}

/**
 * Ritorna il valore di threshold_active.
 * @param   : none.
 * @return  : bool.
*/
bool getThresholdActive()
{
    return threshold_active;
}

/**
 * Setta il valore di dei bit per canale.
 * @param   : int value.
 * @return  : void.
*/
void setBitForChannelValue(int value){
    bit_for_channel = value;
}

/**
 * Setta il valore di filtering_active.
 * @param   : bool; value.
 * @return  : void.
*/
void setFilteringActive(bool value)
{
    filtering_active = value;
}

/**
 * Setta il valore di fps scelto dall'utente.
 * @param   : int; choice.
 * @return  : void.
*/
void setFpsValue(int fps)
{
    fps_value = fps;
}

/**
 * Setta il valore di frame_difference_active.
 * @param   : bool; value.
 * @return  : void.
*/
void setFrameDifferenceActive(bool value)
{
    frame_difference_active = value;
}

/**
 * Setta il valore di histogram_active.
 * @param   : bool; value.
 * @return  : void.
*/
void setHistogramActive(bool value)
{
    histogram_active = value;
}

/**
 * Setta il valore di threshold_active.
 * @param   : bool; value.
 * @return  : void.
*/
void setThresholdActive(bool value)
{
    threshold_active = value;
}

/**
 * Coordina il thread in mutua esclusione.
 * @param   : struct gestore_t *g; riferimento a struct gestore_t.
 * @return  : void.
*/
void startCamera(struct gestore_t *g)
{
    pthread_mutex_lock(&g->mutex);

    g->ncam++;
    if (g->state == STATE_IDLE){
        pthread_mutex_unlock(&g->sem_camera);
        //se c'è lo stato di attesa può partire l'analisi
    }
    pthread_mutex_unlock(&g->mutex);

    pthread_mutex_lock(&g->sem_camera);
    //può effettuare l'analisi
}

/**
 * Coordina il thread in mutua esclusione.
 * @param   : struct gestore_t *g; riferimento a struct gestore_t.
 * @return  : void.
*/
void endCamera(struct gestore_t *g)
{
    pthread_mutex_lock(&g->mutex);

    g->ncam--;
    g->state = STATE_ANALISYS;
    g->nhist++;
    g->nthres++;
    g->ndiff++;
    g->nfilter++;

    pthread_mutex_unlock(&g->sem_histo);
    pthread_mutex_unlock(&g->sem_difference);
    pthread_mutex_unlock(&g->sem_filter);
    pthread_mutex_unlock(&g->sem_threshold);
    //quando la telecamera finisce sveglia i thread di analisi

    pthread_mutex_unlock(&g->mutex);
}

/**
 * Coordina il thread in mutua esclusione.
 * @param   : struct gestore_t *g; riferimento a struct gestore_t.
 * @return  : void.
*/
void startDifference(struct gestore_t *g)
{
    /*
    per prima cosa si blocca sul suo semaforo
    in questo modo si ha la certezza che, se la camera,
    non ha finito di prelevare il fotogramma esso non potrà
    continuare l'analisi
    successivamente blocca il mutex ed aggiorna il contatore
    */
    pthread_mutex_lock(&g->sem_difference);
    
}

/**
 * Coordina il thread in mutua esclusione.
 * @param   : struct gestore_t *g; riferimento a struct gestore_t.
 * @return  : void.
*/
void endDifference(struct gestore_t *g)
{
    pthread_mutex_lock(&g->mutex);
    g->ndiff--;
    if (!g->nhist && !g->nthres && !g->nfilter){
        //non c'è nessun thread di analisi attivo   
        g->state=STATE_IDLE;
        pthread_mutex_unlock(&g->sem_camera);
    }
    pthread_mutex_unlock(&g->mutex);
}

/**
 * Coordina il thread in mutua esclusione.
 * @param   : struct gestore_t *g; riferimento a struct gestore_t.
 * @return  : void.
*/
void startHistogram(struct gestore_t *g)
{
    /*
    per prima cosa si blocca sul suo semaforo
    in questo modo si ha la certezza che, se la camera,
    non ha finito di prelevare il fotogramma esso non potrà
    continuare l'analisi
    successivamente blocca il mutex ed aggiorna il contatore
    */
    pthread_mutex_lock(&g->sem_histo);
    
    //RICHIAMO FUNZIONE HISTOGRAMMA
}

/**
 * Coordina il thread in mutua esclusione.
 * @param   : struct gestore_t *g; riferimento a struct gestore_t.
 * @return  : void.
*/
void endHistogram(struct gestore_t *g)
{
    pthread_mutex_lock(&g->mutex);

    g->nhist--;
    if (!g->ndiff && !g->nthres && !g->nfilter){
        //non c'è nessun thread di analisi attivo   
        g->state=STATE_IDLE;
        pthread_mutex_unlock(&g->sem_camera);
    }
    pthread_mutex_unlock(&g->mutex);
}

/**
 * Coordina il thread in mutua esclusione.
 * @param   : struct gestore_t *g; riferimento a struct gestore_t.
 * @return  : void.
*/
void startThreshold(struct gestore_t *g)
{
    /*
    per prima cosa si blocca sul suo semaforo
    in questo modo si ha la certezza che, se la camera,
    non ha finito di prelevare il fotogramma esso non potrà
    continuare l'analisi
    successivamente blocca il mutex ed aggiorna il contatore
    */
    pthread_mutex_lock(&g->sem_threshold);
    
}

/**
 * Coordina il thread in mutua esclusione.
 * @param   : struct gestore_t *g; riferimento a struct gestore_t.
 * @return  : void.
*/
void endThreshold(struct gestore_t *g)
{
    pthread_mutex_lock(&g->mutex);
    g->nthres--;
    if (!g->nhist && !g->ndiff && !g->nfilter){
        //non c'è nessun thread di analisi attivo   
        g->state=STATE_IDLE;
        pthread_mutex_unlock(&g->sem_camera);
    }
    pthread_mutex_unlock(&g->mutex);
}

/**
 * Coordina il thread in mutua esclusione.
 * @param   : struct gestore_t *g; riferimento a struct gestore_t.
 * @return  : void.
*/
void startFilter(struct gestore_t *g)
{
    /*
    per prima cosa si blocca sul suo semaforo
    in questo modo si ha la certezza che, se la camera,
    non ha finito di prelevare il fotogramma esso non potrà
    continuare l'analisi
    successivamente blocca il mutex ed aggiorna il contatore
    */
    pthread_mutex_lock(&g->sem_filter);
    
    //RICHIAMO FUNZIONE FILTER
}

/**
 * Coordina il thread in mutua esclusione.
 * @param   : struct gestore_t *g; riferimento a struct gestore_t.
 * @return  : void.
*/
void endFilter(struct gestore_t *g)
{
    pthread_mutex_lock(&g->mutex);

    g->nfilter--;
    if (!g->nhist && !g->nthres && !g->ndiff){
        //non c'è nessun thread di analisi attivo   
        g->state=STATE_IDLE;
        pthread_mutex_unlock(&g->sem_camera);
    }
    pthread_mutex_unlock(&g->mutex);
}

/**
 * Esegue un'iterazione completa dell'elaborazione Camera.
 * @param   : void.
 * @return  : void.
*/
void bodyCamera()
{
    startCamera(&gestore);
    setOutCamera(takeAPicture(getRect()));
    endCamera(&gestore);
}

/**
 * Esegue un'iterazione completa dell'elaborazione Difference.
 * @param   : void.
 * @return  : void.
*/
void bodyDifference()
{
    bool toggle; //used to increase performance when feature is hidden.
    toggle = false;
    while (1){
        startDifference(&gestore);
        if (frame_difference_active)
        {
            toggle = true;
            setOutDifference(frameDifference(getOutCamera()));
        }
        else
            if (toggle)
            {
                destroyWindow("Frame difference");
                toggle = false;
            }
        endDifference(&gestore);
    }
}

/**
 * Esegue un'iterazione completa dell'elaborazione Filter.
 * @param   : void.
 * @return  : void.
*/
void bodyFilter()
{
    bool toggle;    //used to increase performance when feature is hidden.
    toggle = false;
    while (1){
        startFilter(&gestore);
        if (filtering_active)
        {
            toggle = true;
            setOutFilter(filterFrame(getOutCamera()));
        }
        else
            if (toggle)
            {
                destroyWindow("Filter");
                toggle = false;
            }
        endFilter(&gestore);
    }
}

/**
 * Esegue un'iterazione completa dell'elaborazione Histogram.
 * @param   : void.
 * @return  : void.
*/
void bodyHistogram()
{
    bool toggle;    //used to increase performance when feature is hidden.
    toggle = false;
    while (1){
        startHistogram(&gestore);
        if (histogram_active)
        {
            toggle = true;
            setOutPlotHistogram(plotHistogram(getOutCamera()));
        }
        else
            if (toggle)
            {
                destroyWindow("Histogram");
                toggle = false;
            }
        endHistogram(&gestore);
    }
}

/**
 * Esegue un'iterazione completa dell'elaborazione Threshold.
 * @param   : void.
 * @return  : void.
*/
void bodyThreshold()
{
    bool toggle;    //used to increase performance when feature is hidden.
    toggle = false;
    while (1){
        startThreshold(&gestore);
        if (threshold_active)
        {
            toggle = true;
            setOutThreshold(threshold(getOutCamera()));
        }
        else
            if (toggle)
            {
                destroyWindow("Threshold");
                toggle = false;
            }
        endThreshold(&gestore);
    }
}

/**
 * Calcola il tempo di wait in base alla scelta dell'utente.
 * @param   : void.
 * @return  : int base_wait; Tempo base di attesa tra un'iterazione e l'altra.
*/
int computeWaitTime()
{
    int base_wait;

    if (fps_value == 15)
        base_wait = 65;
    if (fps_value == 30)
        base_wait = 31;
    if (fps_value == 40)
        base_wait = 23;
    if (fps_value == 50)
        base_wait = 18;
    if (fps_value == 55)
        base_wait = 16;
    if (fps_value == 60)
        base_wait = 15;
    
    return base_wait;
}

/**
 * Crea i task per l'elaborazione.
 * @param   : tpars params[]; Parametri del thread.
 * @param   : int *last_proc; Ultimo processore assegnato.
 * @param   : int *max_proc; Massimo numero di processori.
 * @return  : void;
*/
void createTasks(tpars params[], int *last_proc, int *max_proc)
{
    int i, ret;
    //inizializzazione della memoria condivisa
    initGestore(&gestore);
    initOutput();

    for (i = 0; i < 4; i++)
    {
        params[i] = TASK_SPEC_DFL;
        params[i].period = tspec_from(PER, MILLI);
        params[i].rdline = tspec_from(DREL, MILLI);
        params[i].priority = PRIO;
        params[i].measure_flag = 1;
        params[i].act_flag = NOW;
        params[i].processor = *last_proc++;
        if (*last_proc >= *max_proc)
            *last_proc = 0;
    }

    ret = ptask_create_param(bodyFilter, &params[0]);
    if (ret == -1) {
        printf("Error in creating task!\n");
    }
    ret = ptask_create_param(bodyDifference, &params[0]);
    if (ret == -1) {
        printf("Error in creating task!\n");
    }
    ret = ptask_create_param(bodyThreshold, &params[0]);
    if (ret == -1) {
        printf("Error in creating task!\n");
    }
    ret = ptask_create_param(bodyHistogram, &params[0]);
    if (ret == -1) {
        printf("Error in creating task!\n");
    }
}

/**
 * Mostra i risultati delle elaborazioni.
 * @param   : FrmSettings *frmSettings; Parametro per tenere sotto controllo 
 *                                      il frame settings. 
 * @param   : int wait_time; tempo di attesa dopo gli imshow.
 * @return  : int ret_value; 0 se il frame settings è stato chiuso,
 *                           1 altrimenti.
*/
int showResults(FrmSettings *frmSettings, int wait_time)
{
    int ret_value;

    if (!getOutCameraScaled().empty() &&
       !getOutFilter().empty() &&
       !getOutDifference().empty() &&
       !getOutThreshold().empty() &&
       !getOutPlotHistogram().empty()){     //Used to prevent empty imshows

        imshow("Camera", getOutCameraScaled());
        if (filtering_active)
            imshow("Filter", getOutFilter());
        if (frame_difference_active)
            imshow("Frame difference", getOutDifference());
        if (threshold_active)
            imshow("Threshold", getOutThreshold());
        if (histogram_active)
            imshow("Histogram", getOutPlotHistogram());
    }

    if (!frmSettings->is_visible()){
        destroyAllWindows();
        ret_value = 0;
    }
    else
        ret_value = 1;

    waitKey(wait_time);

    return ret_value;
}

/**
 * Avvia e mantiene attivo il funzionamento dei thread.
 * @param   : FrmSettings *frmSettings; Parametro per tenere sotto controllo 
 *                                      il frame settings.
 * @return  : void.
*/
void runExecutionThreads(FrmSettings *frmSettings)
{
    tpars params[4];    /* Parametri dei thread */
    int last_proc;      /* last assigned processor */   
    int max_proc;       /* max number of procs  */
    int results_value, base_wait, wait_time, count, bias;
    time_t start, time_taken, end;

    last_proc = 0;
    max_proc = ptask_getnumcores();
    count = 0;
    bias = 0;

    createTasks(params, &last_proc, &max_proc);
    start = time(0);
    time_taken = 0;
    base_wait = wait_time = computeWaitTime();

    do{
        end = time(0);

        if (time_taken != end - start) {
            cout << "FPS: " << count << endl;
            if (count < fps_value)
                bias--;
            else
                bias++;
            if (base_wait + bias > 5)
                wait_time = base_wait + bias;
            else
                wait_time = base_wait;
            count = 0;
        }
        else
            count++;

        time_taken = end - start;

        bodyCamera();
        
        results_value = showResults(frmSettings, wait_time);
        
    } while (results_value);
}

//_____________________________________________________________________________
//_____________________________________________________________________________

//				FUNZIONI PER L'ELABORAZIONE DEL FOTOGRAMMA CATTURATO

//_____________________________________________________________________________
//_____________________________________________________________________________

/**
 * Filtra l'immagin catturata modificandola sendo la scelta utente.
 * @param   : Mat src; immagine da processare.
 * @return  : restituisce l'oggetto Mat contenente il grafico.
*/
Mat filterFrame(Mat img)
{
	Mat im_out, thr;
    im_out = img.clone();
	float reduction;
    reduction = frame_scaling_value / 100;
	Mat kern;
    kern = (cv::Mat_<float>(4,4) <<  0.272, 0.534, 0.131, 0,
									 0.349, 0.686, 0.168, 0,
							         0.393, 0.769, 0.189, 0,
									 0, 0, 0, 1); 
	switch (filter_choice){
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
	return imageScale(im_out);
	
}

/**
 * Fa la differenza assoluta fra due frame tenendo conto
 * della lunghezza di passo.
 * @param   : Mat img; Immagine in ingresso, l'ultima catturata dalla camera.
 * @return  : Mat; Immagine risultante dopo aver applicato la sottrazione.
*/
Mat frameDifference(Mat img)
{
    if (frame_difference_count == 0)
        img_difference_old = img;
    if (begin_difference)
        img_difference_out = img;
    if (frame_difference_value == frame_difference_count){
        absdiff(img_difference_old, img, img_difference_out);
        frame_difference_count = 0;
    }else{
        frame_difference_count++;
    }
    if (frame_difference_value < frame_difference_count)
        frame_difference_count = 0;
    begin_difference = false;
    return imageScale(img_difference_out);
}

/**
 * Calcola l'istogramma dei colori dello spettro RGB di un'immagine.
 * @param   : Mat src; immagine da processare.
 * @return  : restituisce l'oggetto Mat contenente il grafico.
*/
Mat plotHistogram(Mat src)
{
    int hist_w, hist_h, histSize, i, bin_w;
    vector<Mat> bgr_planes;
    float range[] = { 0, 256 }; 
    bool uniform, accumulate;
    const float* histRange;
    Mat b_hist, g_hist, r_hist;

	//separo l'immagine nei 3 layer  B G R
	split( src, bgr_planes );
    histSize = 256;
    histRange = { range };
	//parametri dell'istogramma
    uniform = true;
    accumulate = false;
	//calcolo di 3 istogrammi del colore
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize,
			 &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize,
			 &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize,
			 &histRange, uniform, accumulate);
   	//tracciamento delle curve
    hist_w = getRect().width;
    hist_h = getRect().height;
	
    bin_w = cvRound((double) hist_w / histSize);
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar( 0,0,0));
	//plot dell'istogramma
	//normalizzazione del risultato
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	//plot per ogni colore
	for (i = 1; i < histSize; i++ ){
		line( histImage, Point( bin_w*(i-1), 
			  hist_h - cvRound(b_hist.at<float>(i-1))),
			  Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
			  Scalar( 255, 0, 0), 2, 8, 0  );
		line( histImage, Point( bin_w*(i-1),
			  hist_h - cvRound(g_hist.at<float>(i-1))),
			  Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
			  Scalar( 0, 255, 0), 2, 8, 0  );
		line( histImage, Point( bin_w*(i-1),
			  hist_h - cvRound(r_hist.at<float>(i-1))),
			  Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
			  Scalar( 0, 0, 255), 2, 8, 0  );
	}
	return imageScale(histImage);
}

/**
 * Modifica il numero di bit per canale.
 * @param   : Mat inImage; immagienda modificare.
 * @param   : int numBits; numero di bit desiderati.
 * @return  : void.
*/
Mat quantizeImage(Mat inImage, int numBits)
{
    int i, j;
	cv::Mat retImage;
    retImage = inImage.clone();

	uchar maskBit;
    maskBit = 0xFF;

	maskBit = maskBit << (8 - numBits);

	for (j = 0; j < retImage.rows; j++)
		for (i = 0; i < retImage.cols; i++)
		{
			cv::Vec3b valVec;
            valVec = retImage.at<cv::Vec3b>(j, i);
			valVec[0] = valVec[0] & maskBit;
			valVec[1] = valVec[1] & maskBit;
			valVec[2] = valVec[2] & maskBit;
			retImage.at<cv::Vec3b>(j, i) = valVec;
		}

	return retImage;
}

/**
 * Cattura dello schermo (deve prendere in ingresso l'area
 * tracciata dall'utente).
 * @param   : Rect2d rect; Definisce l'area di registrazione della camera.
 * @return  : void.
*/
Mat takeAPicture(Rect2d rect)
{
	ScreenShot screen(rect.x, rect.y, rect.width, rect.height);
	Mat imgCamera, ret;
	int nbit = bit_for_channel;

	screen(imgCamera);
	if (nbit != 0){
		ret = quantizeImage(imgCamera, nbit).clone();
	}else{
		ret = imgCamera.clone();
	}
	return ret;
}

/**
 * Applica un threshold all'immagine in ingresso e restituisce il risultato.
 * @param   : Mat img; Immagine in ingresso, l'ultima catturata dalla camera.
 * @return  : Mat; Immagine risultante dopo aver applicato il threshold.
*/
Mat threshold(Mat img)
{
	Mat img_out, img_gray;
	threshold(img, img_out, threshold_value, 255, threshold_type);
	
	return imageScale(img_out);
}

//_____________________________________________________________________________
//_____________________________________________________________________________

//				FUNZIONI PER L'OUTPUT A VIDEO

//_____________________________________________________________________________
//_____________________________________________________________________________

/**
 * Inizializza la struct output.
 * @param   : void.
 * @return  : void.
*/
void initOutput()
{
    pmux_create_pi(&output.sem_camera);
    pmux_create_pi(&output.sem_filter);
    pmux_create_pi(&output.sem_histo);
    pmux_create_pi(&output.sem_threshold);
    pmux_create_pi(&output.sem_difference);
}

/**
 * Preleva il frame camera da analizzare.
 * @param   : None.
 * @return  : Mat r; frame camera ritornato.
*/
Mat getOutCamera()
{
    Mat r;

    pthread_mutex_lock(&output.sem_camera);
    r=output.cam.clone();
    pthread_mutex_unlock(&output.sem_camera);
    return r;
}

/**
 * Preleva il frame camera da stampare a video effettuando il resize;
 * in questo modo si preserva la coerenza del tracciamento dei grafici.
 * @param   : None.
 * @return  : Mat r; frame camera ritornato.
*/
Mat getOutCameraScaled()
{
    Mat r;

    pthread_mutex_lock(&output.sem_camera);
    r=output.cam.clone();
    pthread_mutex_unlock(&output.sem_camera);
    return imageScale(r);
}

/**
 * Preleva il frame difference da stampare a video.
 * @param   : None.
 * @return  : Mat r; frame Difference ritornato.
*/
Mat getOutDifference()
{
    Mat r;

    pthread_mutex_lock(&output.sem_difference);
    r=output.diff.clone();
    pthread_mutex_unlock(&output.sem_difference);
    return r;
}

/**
 * Preleva il frame filtro da stampare a video.
 * @param   : None.
 * @return  : Mat r; frame filtro ritornato.
*/
Mat getOutFilter()
{
    Mat r;

    pthread_mutex_lock(&output.sem_filter);
    r=output.filter.clone();
    pthread_mutex_unlock(&output.sem_filter);
    return r;
}

/**
 * Preleva il frame istogramma da stampare a video.
 * @param   : None.
 * @return  : Mat r; frame istogramma ritornato.
*/
Mat getOutPlotHistogram()
{
    Mat r;

    pthread_mutex_lock(&output.sem_histo);
    r=output.histogram.clone();
    pthread_mutex_unlock(&output.sem_histo);
    return r;
}

/**
 * Preleva il frame threshold da stampare a video.
 * @param   : None.
 * @return  : Mat r; frame threshold ritornato.
*/
Mat getOutThreshold()
{
    Mat r;

    pthread_mutex_lock(&output.sem_threshold);
    r=output.thr.clone();
    pthread_mutex_unlock(&output.sem_threshold);
    return r;
}

/**
 * Preleva il rettangolo della ROI.
 * @param   : None.
 * @return  : Rect2d r; rect ritornato.
*/
Rect2d getRect()
{
    return output.rect;
}

/**
 * Setta il frame camera da stampare a video.
 * @param   : Mat img; frame Camera da settare.
 * @return  : None.
*/
void setOutCamera(Mat img)
{
    pthread_mutex_lock(&output.sem_camera);
    output.cam=img.clone();
    pthread_mutex_unlock(&output.sem_camera);
}

/**
 * Setta il frame difference da stampare a video.
 * @param   : Mat img; frame Difference da settare.
 * @return  : None.
*/
void setOutDifference(Mat img)
{
    pthread_mutex_lock(&output.sem_difference);
    output.diff=img.clone();
    pthread_mutex_unlock(&output.sem_difference);
}

/**
 * Setta il frame filtro da stampare a video.
 * @param   : Mat img; frame filtro da settare.
 * @return  : None.
*/
void setOutFilter(Mat img)
{
    pthread_mutex_lock(&output.sem_filter);
    output.filter=img.clone();
    pthread_mutex_unlock(&output.sem_filter);
}

/**
 * Setta il frame istogramma da stampare a video.
 * @param   : Mat img; frame istogramma da settare.
 * @return  : None.
*/
void setOutPlotHistogram(Mat img)
{
    pthread_mutex_lock(&output.sem_histo);
    output.histogram=img.clone();
    pthread_mutex_unlock(&output.sem_histo);
}

/**
 * Setta il frame threshold da stampare a video.
 * @param   : Mat img; frame Threshold da settare.
 * @return  : None.
*/
void setOutThreshold(Mat img)
{
    pthread_mutex_lock(&output.sem_threshold);
    output.thr=img.clone();
    pthread_mutex_unlock(&output.sem_threshold);
}

/**
 * Setta il rettangolo della ROI.
 * @param   : Rect2d r; Il rettangolo da settare.
 * @return  : None.
*/
void setRect(Rect2d r)
{
    output.rect = r;
}