#ifndef SRC_CONSOLE_THREAD_H
#define SRC_CONSOLE_THREAD_H

#include "tabs/logs.h"
#include "tabs/processes.h"
#include "tabs/profiles.h"
#include "threads/blocking_queue.h"
#include "threads/dispatcher_middleman.h"

#include <chrono>
#include <condition_variable>
#include <future>
#include <glibmm/dispatcher.h>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

enum TabState { PROFILE, PROCESS, LOGS, OTHER };

/**
 * This class creates a separate thread that the main GUI thread can communicate with.
 * This second thread asynchronously calls terminal commands and communicates the results with the main thread.
 **/
class ConsoleThread
{
public:
  ConsoleThread(std::shared_ptr<Profiles> prof, std::shared_ptr<Processes> proc, std::shared_ptr<Logs<StatusColumnRecord>> logs);
  ~ConsoleThread();

  // Delete the copy-constructor, move constructor, and copy assignment operator
  ConsoleThread(const ConsoleThread &other)  = delete;
  ConsoleThread(const ConsoleThread &&other) = delete;
  ConsoleThread &operator=(const ConsoleThread &other) = delete;

  // Create a move assignment operator
  ConsoleThread &operator=(ConsoleThread &&other) noexcept;

  void send_refresh_message(TabState new_state);
  void send_change_profile_status_message(const std::string &profile, const std::string &old_status, const std::string &new_status);
  void send_quit_message();

  void get_status();
  void get_unconfined();
  void get_logs();

protected:
  enum Event { REFRESH, CHANGE_STATUS, QUIT };

  struct Message {
    Event event;
    TabState state;
    std::vector<std::string> data;

    Message(Event a, TabState b, std::vector<std::string> c) : event{a}, state{b}, data{std::move(c)} { }
  };

  void console_caller();

private:
  static constexpr unsigned int TIME_WAIT = 5;
  static std::chrono::time_point<std::chrono::steady_clock> get_wait_time_point();

  Message wait_for_message();
  void run_command(TabState state);

  // Representation of the extra thread
  std::future<void> asynchronous_thread;

  // Member fields
  BlockingQueue<Message, std::deque<Message>, std::mutex> queue;
  TabState last_state;

  // DispatcherMiddleman used to communicate results with main thread
  DispatcherMiddleman<Profiles, Processes, Logs<StatusColumnRecord>, Glib::Dispatcher, std::mutex> dispatch_man;

  // Synchronization Primitives
  std::mutex task_ready_mtx;
  std::condition_variable cv;
};

#endif // CONSOLE_THREAD_H
