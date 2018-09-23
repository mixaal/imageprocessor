#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

#include <stdio.h>
#include <filters.h>
#include <shapes.h>

using namespace cv;
using namespace std;

static Rect face;
static float scale;

#define SCALE_SIZE 256.0f

static void face_and_eye_detect(char *image_name) 
{
  Mat img;
  img = imread(image_name, CV_LOAD_IMAGE_COLOR);
  if (!img.data) {
     cerr << "Can't load image data!" << endl;
     exit(-1);
  }
  cvtColor(img, img, COLOR_RGB2GRAY);
  cerr << img.cols << "x" << img.rows << endl;
  scale = img.cols / SCALE_SIZE;
  Size size((int)(img.cols/scale),(int)(img.rows/scale));//the dst image size
  Mat dst;//dst image
  resize(img,dst,size);//resize image
  cerr << dst.cols << "x" << dst.rows << endl;
  cvNamedWindow( "Scaled", WINDOW_AUTOSIZE );
  vector<double> weights;
  vector<int> levels;
  vector<Rect> detections;
  CascadeClassifier model("data/haarcascade_frontalface_default.xml");
  model.detectMultiScale(dst, detections, levels, weights, 1.1, 7, 0, Size(10, 10), Size(128,128), true);
  if (detections.size() == 0) {
    cerr << "nothing detected" << endl;
    exit(-1);
  }
  for(unsigned int i=0; i<detections.size(); i++) {
	  cerr << "Detection " << detections[i] << " with weight " << weights[i] << endl;
  }
  rectangle(dst, detections[0], Scalar(255, 255, 0));
  imshow("Scaled", dst);
  //cerr << "Detection " << detections[0] << " with weight " << weights[0] << endl;
  cvWaitKey();
  face = detections[0];
}

void mark_face(char *image_name)
{
  
  layer_t layer = read_JPEG_file(image_name);
  rect_t face_zone = {int(face.x*scale), int(face.y*scale), int((face.x+face.width)*scale), int((face.y+face.height)*scale)};
  draw_rect(layer, vec3_init(1.0f, 1.0f, 0.0f), 1.0f, face_zone, blend_normal);
  write_JPEG_file(string("face.jpg").c_str(), layer, 90);
}

int main(int argc, char *argv[])
{
  face_and_eye_detect(argv[1]);
  mark_face(argv[1]);
  return 0;
}
