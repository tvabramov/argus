#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

#include "workers/image_library.h"

struct WorkerCommand {
  std::list<std::string> m_keys;
  std::string m_desc_params, m_desc_use;
  std::function<void(std::list<std::string>)> m_operation;

  explicit WorkerCommand(std::list<std::string> keys, std::string desc_params,
                         std::string desc_use,
                         std::function<void(std::list<std::string>)> operation)
      : m_keys(std::move(keys)),
        m_desc_params(desc_params),
        m_desc_use(desc_use),
        m_operation(std::move(operation)) {}
};

void StartShellLoop();

using namespace std;

int main(int argc, char *argv[]) {
  StartShellLoop();

  return EXIT_FAILURE;
}

void StartShellLoop() {
  ImageLibrary library;

  list<WorkerCommand> worker_commands;
  worker_commands.emplace_back(
      list<string>({"ld", "load"}), "<name>, <filename>",
      "Load image from a disk", [&library](list<string> args) {
        if (args.size() != 2) {
          cout << "Invalid number of args. Type \"h\" for help." << endl;
          return;
        }
        if (library.Load(args.front(), args.back())) {
          cout << "Done!" << endl;
        } else {
          cout << "Can not load image" << endl;
        }
      });
  worker_commands.emplace_back(
      list<string>({"s", "store"}), "<name>, <filename>",
      "Store image to a disk", [&library](list<string> args) {
        if (args.size() != 2) {
          cout << "Invalid number of args. Type \"h\" for help." << endl;
          return;
        }
        if (library.Store(args.front(), args.back())) {
          cout << "Done!" << endl;
        } else {
          cout << "Can not store image" << endl;
        }
      });
  worker_commands.emplace_back(
      list<string>({"blur"}), "<from_name>, <to_name>, <size>", "Blur image",
      [&library](list<string> args) {
        if (args.size() != 3) {
          cout << "Invalid number of args. Type \"h\" for help." << endl;
          return;
        }
        cv::Mat frame = library.Get(args.front());
        if (frame.empty()) {
          cout << "Invalid input image. Type \"h\" for help." << endl;
          return;
        }
        int w = stoi(args.back());
        blur(frame, frame, cv::Size(w, w));
        args.pop_front();
        library.Push(move(frame), args.front());
        cout << "Done!" << endl;
      });
  worker_commands.emplace_back(
      list<string>({"resize"}), "<from_name>, <to_name>, <W>, <H>",
      "Resize image", [&library](list<string> args) {
        if (args.size() != 4) {
          cout << "Invalid number of args. Type \"h\" for help." << endl;
          return;
        }
        cv::Mat frame = library.Get(args.front());
        if (frame.empty()) {
          cout << "Invalid input image. Type \"h\" for help." << endl;
          return;
        }
        args.pop_front();
        auto oname = args.front();
        args.pop_front();
        int w = stoi(args.front());
        int h = stoi(args.back());
        resize(frame, frame, cv::Size(w, h));
        library.Push(move(frame), oname);
        cout << "Done!" << endl;
      });
  worker_commands.emplace_back(
      list<string>({"ls", "list"}), "", "List of loaded images",
      [&library](list<string> args) {
        auto keys = library.List();
        if (keys.empty()) {
          cout << "No images loaded" << endl;
        } else {
          cout << accumulate(
                      next(keys.begin()), keys.end(), keys.front(),
                      [](const auto &a, const auto &b) { return a + ", " + b; })
               << endl;
        }
      });
  worker_commands.emplace_back(list<string>({"q", "quit", "exit"}), "",
                               "Quit the ARGUS shell",
                               [](list<string>) { exit(EXIT_SUCCESS); });
  worker_commands.emplace_back(
      list<string>({"h", "help"}), "", "Print help",
      [&worker_commands](list<string> args) {
        cout << left << setw(15) << "Command" << left << setw(35) << "Arguments"
             << left << setw(30) << "Description" << endl;
        for (const auto &cmd : worker_commands) {
          auto keys = accumulate(
              next(cmd.m_keys.begin()), cmd.m_keys.end(), cmd.m_keys.front(),
              [](const auto &a, const auto &b) { return a + ", " + b; });

          cout << left << setw(15) << keys << left << setw(35)
               << cmd.m_desc_params << left << setw(30) << cmd.m_desc_use
               << endl;
        }
      });

  for (;;) {
    cout << "You are in the ARGUS command shell. Type your command (\"h\" for "
            "help):"
         << endl
         << "> ";
    string buf;
    getline(cin, buf);
    list<string> words;
    boost::trim(buf);
    boost::split(words, buf, boost::is_any_of("\t "), boost::token_compress_on);
    words.remove_if([](const auto &item) { return item.empty(); });

    auto it_cmd = find_if(worker_commands.begin(), worker_commands.end(),
                          [&words](const auto &item) {
                            return find(item.m_keys.begin(), item.m_keys.end(),
                                        words.front()) != item.m_keys.end();
                          });

    if (it_cmd == worker_commands.end()) {
      cout << "Invalid command. Type \"h\" for help." << endl;
    } else {
      words.pop_front();
      it_cmd->m_operation(move(words));
    }
  }
}
