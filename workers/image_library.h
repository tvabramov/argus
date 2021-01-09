#ifndef WORKERS_IMAGE_LIBRARY_H
#define WORKERS_IMAGE_LIBRARY_H

#include <list>
#include <map>
#include <opencv2/core.hpp>

class ImageLibrary {
 public:
  ImageLibrary() {}
  virtual ~ImageLibrary() {}

  void Push(const cv::Mat &frame, const std::string &key);
  void Push(cv::Mat &&frame, const std::string &key);
  void Delete(const std::string &key);
  std::list<std::string> List();
  bool Load(const std::string &key, const std::string &fn);
  bool Store(const std::string &key, const std::string &fn);
  cv::Mat Get(const std::string &key);

 private:
  std::map<std::string, cv::Mat> m_library;
};

#endif  // WORKERS_IMAGE_LIBRARY_H
