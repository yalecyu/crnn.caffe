#include <gflags/gflags.h>
#include <glog/logging.h>
#include "boost/algorithm/string.hpp"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <utility>
#include <iosfwd>
#include <string>
#include <iostream>
#include <caffe/caffe.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace caffe;
using namespace std;
using namespace cv;
class Classifier {
 public:
  Classifier(const string& model_file,const string& trained_file);
  std::vector<int> Classify(const cv::Mat& img, int N = 1);
 private:
  std::vector<int> Predict(const cv::Mat& img);
  void WrapInputLayer(std::vector<cv::Mat>* input_channels);
  void Preprocess(const cv::Mat& img,std::vector<cv::Mat>* input_channels);
 private:
  boost::shared_ptr<Net<float> > net_;
  cv::Size input_geometry_;
  int num_channels_;
};
Classifier::Classifier(const string& model_file,const string& trained_file) {
/*#ifdef CPU_ONLY
  Caffe::set_mode(Caffe::CPU);
#else
  Caffe::set_mode(Caffe::GPU);
#endif*/
  /* Load the network. */
  net_.reset(new Net<float>(model_file, caffe::TEST));
  net_->CopyTrainedLayersFrom(trained_file);

  CHECK_EQ(net_->num_inputs(), 1) << "Network should have exactly one input.";
  CHECK_EQ(net_->num_outputs(), 1) << "Network should have exactly one output.";

  Blob<float>* input_layer = net_->input_blobs()[0];
  num_channels_ = input_layer->channels();
  CHECK(num_channels_ == 3 || num_channels_ == 1)
    << "Input layer should have 1 or 3 channels.";
  input_geometry_ = cv::Size(input_layer->width(), input_layer->height());

//Blob<float>* output_layer = net_->output_blobs()[0];
}
/* Return the top N predictions. */
std::vector<int> Classifier::Classify(const cv::Mat& img, int N) {
  std::vector<int> output = Predict(img);
  return output;
}
std::vector<int> Classifier::Predict(const cv::Mat& img) {
  Blob<float>* input_layer = net_->input_blobs()[0];
  input_layer->Reshape(1, num_channels_,input_geometry_.height, input_geometry_.width);
  /* Forward dimension change to all layers. */
  net_->Reshape();
  std::vector<cv::Mat> input_channels;
  WrapInputLayer(&input_channels);
  Preprocess(img, &input_channels);
  net_->Forward();
  /* Copy the output layer to a std::vector */
  Blob<float>* output_layer = net_->output_blobs()[0];
  int alphabet_size=output_layer->shape(2);
  int time_step=output_layer->shape(0);
  vector<int>pred_label_seq_with_blank;
  const float* begin = output_layer->cpu_data();
  for(int t=0;t<time_step;t++){
    pred_label_seq_with_blank.push_back(max_element(begin,begin+alphabet_size)-begin);
    begin+=alphabet_size;
  }
  return pred_label_seq_with_blank;
}
void Classifier::WrapInputLayer(std::vector<cv::Mat>* input_channels) {
  Blob<float>* input_layer = net_->input_blobs()[0];
  int width = input_layer->width();
  int height = input_layer->height();
  float* input_data = input_layer->mutable_cpu_data();
  for (int i = 0; i < input_layer->channels(); ++i) {
    cv::Mat channel(height, width, CV_32FC1, input_data);
    input_channels->push_back(channel);
    input_data += width * height;
  }
}

void Classifier::Preprocess(const cv::Mat& img,std::vector<cv::Mat>* input_channels) {
  /* Convert the input image to the input image format of the network. */
  cv::Mat sample;
  if (img.channels() == 3 && num_channels_ == 1)
    cv::cvtColor(img, sample, cv::COLOR_BGR2GRAY);
  else if (img.channels() == 4 && num_channels_ == 1)
    cv::cvtColor(img, sample, cv::COLOR_BGRA2GRAY);
  else if (img.channels() == 4 && num_channels_ == 3)
    cv::cvtColor(img, sample, cv::COLOR_BGRA2BGR);
  else if (img.channels() == 1 && num_channels_ == 3)
    cv::cvtColor(img, sample, cv::COLOR_GRAY2BGR);
  else
    sample = img;
  cv::Mat sample_resized;
  if (sample.size() != input_geometry_)cv::resize(sample, sample_resized, input_geometry_);
  else sample_resized = sample;
  cv::Mat sample_float;
  if (num_channels_ == 3)sample_resized.convertTo(sample_float, CV_32FC3);
  else sample_resized.convertTo(sample_float, CV_32FC1);
  cv::split(sample_float, *input_channels);
  CHECK(reinterpret_cast<float*>(input_channels->at(0).data)
        == net_->input_blobs()[0]->cpu_data())
    << "Input channels are not wrapping the input layer of the network.";
}
int main(int argc, char** argv){
    //caffe::GlobalInit(&argc,&argv);
    string file_path=(string)argv[1];
    string proto_cnn_file=(string)argv[2];
    string model_cnn_file=(string)argv[3];
    int blank_label = 10;
	Classifier classifier(proto_cnn_file,model_cnn_file);
    Mat image=imread(file_path);
    Mat resizeimg;
    resize(image,resizeimg,Size(128,32),0,0,CV_INTER_LINEAR);
    vector<int> ans=classifier.Classify(resizeimg);
    cout << "Original result" << endl;
    for(int i=0;i<ans.size();i++){
        if(ans[i] == blank_label) cout << "-" ;
        else cout << ans[i];
    }
    cout << endl << "Final result" << endl;
    int pre = -1;
    for(int i = 0; i < ans.size(); i++) {
        if(ans[i] != blank_label && ans[i] != pre) cout << ans[i];
        pre = ans[i];
    }
    cout << endl;
}
