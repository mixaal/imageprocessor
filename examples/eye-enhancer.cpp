#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

#include <opencv2/objdetect.hpp>
#include <opencv2/objdetect/objdetect_c.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/highgui/highgui_c.h>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <vector>

#include <stdio.h>
#include <filters.h>
#include <shapes.h>

using namespace cv;
using namespace std;

static std::vector<Rect> faces;
static std::vector<Rect> eyes;


static void face_and_eye_detect(char *image_name) 
{
  Mat img;
  img = imread(image_name, CV_LOAD_IMAGE_COLOR);
  if (!img.data) {
     cerr << "Can't load image data!" << endl;
     exit(-1);
  }
  ocl::setUseOpenCL(false);
  VideoCapture capture;
  cvtColor(img, img, COLOR_RGB2GRAY);
  equalizeHist(img, img);
  cerr << img.cols << "x" << img.rows << endl;
  cvNamedWindow( "Image", WINDOW_AUTOSIZE );
  vector<double> weights;
  vector<int> levels;
  vector<Rect> detections;
  CascadeClassifier model("data/haarcascade_frontalface_default.xml");
   model.detectMultiScale(img, detections, levels, weights, 1.1, 5, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30), Size(1024,1024), true);
  //model.detectMultiScale(img, detections, levels, weights, 1.1, 5, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
  if (detections.size() == 0) {
    cerr << "nothing detected" << endl;
    exit(-1);
  }
  for(unsigned int i=0; i<detections.size(); i++) {
    cerr << "Detection " << detections[i] << " with weight " << weights[i] << endl;
    faces.push_back(detections[i]);
  }
  rectangle(img, detections[0], Scalar(255, 255, 0));
  imshow("Image", img);
  //cerr << "Detection " << detections[0] << " with weight " << weights[0] << endl;
//  cvWaitKey();
}

void mark_face(char *image_name)
{
  
  layer_t layer = read_JPEG_file(image_name);

  for(unsigned int i=0; i<faces.size(); ++i) {
    int xx = int(faces.at(i).x);
    int yy = int(faces.at(i).y);
    int w = int(faces.at(i).width);
    int h = int(faces.at(i).height);
    rect_t face_zone = {xx, yy, xx+w, yy+h};
    printf("xx=%d yy=%d w=%d h=%d\n", xx, yy, w, h);
    draw_rect(layer, vec3_init(1.0f, 1.0f, 0.0f), 1.0f, face_zone, blend_normal);
  }
  write_JPEG_file(string("face.jpg").c_str(), layer, 90);
}

int main(int argc, char *argv[])
{
  face_and_eye_detect(argv[1]);
  mark_face(argv[1]);
  return 0;
}
