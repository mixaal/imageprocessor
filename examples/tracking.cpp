#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

#include <filters.h>
#include <video.h>
#include <brush.h>
#include <math.h>

using namespace cv;
using namespace std;


#define MAX_FRAMES 1000
#define VIDEO_FILE_IN "/home/fila/Downloads/IMG_2583.MOV"
#define VIDEO_FILE_OUT "output.avi"

static rect_t detected_frames[MAX_FRAMES];
static int frame_no = 0;

static void detect_window();
static void frame_vfx_filter(layer_t layer_t, int frame_no, void *arg);
 


// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()
 
int main(int argc, char **argv)
{
  detect_window();
  fprintf(stderr, "Processed %d frames.\n", frame_no);
  frame_no = 0;
  process_video(VIDEO_FILE_IN, VIDEO_FILE_OUT, frame_vfx_filter);
  return 0;
}


static void frame_vfx_filter(layer_t layer, int frame_no, void *arg)
{
  fprintf(stderr, "frame_no=%d\n", frame_no);
  if (frame_no<0 || frame_no>=MAX_FRAMES) return;
  rect_t zone = detected_frames[frame_no];
  if (zone.minx == -1 && zone.miny == -1) return ;
  
  /**
   * Prepare layers and setup masks.
   */
  layer_t night = layer_copy(layer);
  layer_t lights = layer_copy(layer);
  layer_t lights_mask = layer_new_dim(layer.width, layer.height, layer.color_components, False, False);


  /**
   * Convert daily image into night image.
   */
  colorize(night, vec3_init(0.0f, 0.0f, COLOR_MAX), 0.8f, 0.0f, ALL_LEVELS, False, night.zone);
  exposure(night, -1.9f, night.zone);


  /**
   * Convert daily image into night lamp light, enhance clarity with unsharp mask.
   */
  colorize(lights, vec3_init(COLOR_MAX, COLOR_MAX, 0.0f), 0.8f, 0.0f, ALL_LEVELS, False, lights.zone);
  float cyan_red[3] = { 0.0f, 0.0f, 0.03f };
  float magenta_green[3] = { 0.05f, 0.08f, 0.07f };
  float yellow_blue[3] = { -0.3f, -0.3f, -0.3f };
  adjust_color_balance(lights, cyan_red, magenta_green, yellow_blue, lights.zone);
   /**
   * Create mask for night light image.
   */
  int x2 = (zone.minx + zone.maxx) >> 1;
  int y4 = (zone.maxy - zone.miny) >> 2;
  brush_touch(lights_mask, IMAGE, 400, 0.67f, x2, zone.miny, vec3_init(1.0f, 1.0f, 1.0f), blend_normal);
  brush_touch(lights_mask, IMAGE, 600, 0.27f, x2, zone.miny+y4, vec3_init(1.0f, 1.0f, 1.0f), blend_normal);
  brush_touch(lights_mask, IMAGE, 400, 0.27f, x2, zone.miny+2*y4, vec3_init(1.0f, 1.0f, 1.0f), blend_normal);
  brush_touch(lights_mask, IMAGE, 400, 0.47f, x2, zone.miny+3*y4, vec3_init(1.0f, 1.0f, 1.0f), blend_normal);
  /**
   * Set up lights mask and merge-down.
   */
  lights.mask = lights_mask.image;
  lights.blend_func = blend_normal;
  layer_t layers[2] =  { night, lights };
  layer_t output = layer_merge_down( 2, layers );
  memcpy(layer.image, output.image, layer.width * layer.height * layer.color_components * sizeof(color_t));
}

static void detect_window()
{
    // List of tracker types in OpenCV 3.4.1
    string trackerTypes[8] = {"BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"};
    // vector <string> trackerTypes(types, std::end(types));
 
    // Create a tracker
    string trackerType = trackerTypes[2];
 
    Ptr<Tracker> tracker;
 
    #if (CV_MINOR_VERSION < 3)
    {
        tracker = Tracker::create(trackerType);
    }
    #else
    {
        if (trackerType == "BOOSTING")
            tracker = TrackerBoosting::create();
        if (trackerType == "MIL")
            tracker = TrackerMIL::create();
        if (trackerType == "KCF")
            tracker = TrackerKCF::create();
        if (trackerType == "TLD")
            tracker = TrackerTLD::create();
        if (trackerType == "MEDIANFLOW")
            tracker = TrackerMedianFlow::create();
        if (trackerType == "GOTURN")
            tracker = TrackerGOTURN::create();
        if (trackerType == "MOSSE")
            tracker = TrackerMOSSE::create();
        if (trackerType == "CSRT")
            tracker = TrackerCSRT::create();
    }
    #endif
    // Read video
    VideoCapture video(VIDEO_FILE_IN);
     
    // Exit if video is not opened
    if(!video.isOpened())
    {
        cout << "Could not read video file" << endl; 
        exit(-1) ; 
    } 
 
    // Read first frame 
    Mat frame; 
    bool ok = video.read(frame); 
    if (!ok) {
      cout << "Unable to read video frame!" << endl;
      exit(-1);
    }
 
    // Define initial bounding box 
    Rect2d bbox(950, 410, 220, 320); 
 
     //Uncomment the line below to select a different bounding box 
    // bbox = selectROI(frame, false); 
    // Display bounding box. 
    rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 ); 
 
    imshow("Tracking", frame); 
    tracker->init(frame, bbox);

   detected_frames[frame_no] = { (int)bbox.x, (int)bbox.y, (int)(bbox.x + bbox.width), (int)(bbox.y + bbox.height) };
   frame_no++;
     
    while(video.read(frame))
    {     
        // Start timer
        double timer = (double)getTickCount();
         
        // Update the tracking result
        bool ok = tracker->update(frame, bbox);
         
        // Calculate Frames per second (FPS)
        float fps = getTickFrequency() / ((double)getTickCount() - timer);
         
        if (ok)
        {
            // Tracking success : Draw the tracked object
            rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
            detected_frames[frame_no] = { (int)floor(bbox.x), (int)floor(bbox.y), (int)floor(bbox.x + bbox.width), (int)floor(bbox.y + bbox.height) };
        }
        else
        {
            // Tracking failure detected.
            putText(frame, "Tracking failure detected", Point(100,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
            detected_frames[frame_no] = { -1, -1, -1, -1 };
        }
        frame_no++;
        // process first MAX_FRAMES
        if (frame_no >= MAX_FRAMES) return;
         
        // Display tracker type on frame
        putText(frame, trackerType + " Tracker", Point(100,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50),2);
         
        // Display FPS on frame
        putText(frame, "FPS : " + SSTR(int(fps)), Point(100,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50), 2);
 
        // Display frame.
        imshow("Tracking", frame);
         
        // Exit if ESC pressed.
        int k = waitKey(1);
        if(k == 27)
        {
            break;
        }
 
    }
}
