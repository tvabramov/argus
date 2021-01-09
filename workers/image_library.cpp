#include "workers/image_library.h"

#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;

void ImageLibrary::Push(const Mat &frame, const string &key) {
  m_library.insert(make_pair(key, frame.clone()));
}

void ImageLibrary::Push(Mat &&frame, const string &key) {
  m_library.insert(make_pair(key, move(frame)));
}

void ImageLibrary::Delete(const string &key) { m_library.erase(key); }

list<string> ImageLibrary::List() {
  list<string> keys;
  transform(m_library.begin(), m_library.end(), back_inserter(keys),
            [](const auto &p) { return p.first; });
  return keys;
}

bool ImageLibrary::Load(const string &key, const string &fn) {
  Mat frame = imread(fn);
  if (frame.empty()) {
    return false;
  }

  Push(move(frame), key);

  return true;
}

bool ImageLibrary::Store(const string &key, const string &fn) {
  auto it_frame = m_library.find(key);
  if (it_frame == m_library.end()) return false;

  return imwrite(fn, it_frame->second);
}

Mat ImageLibrary::Get(const std::string &key) {
  auto it_frame = m_library.find(key);
  return it_frame == m_library.end() ? Mat() : it_frame->second.clone();
}
