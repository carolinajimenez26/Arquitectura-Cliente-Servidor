#include "safeQueue.hh"
#include <atomic>
#include <thread>
#include "safeHash.hh"
#include "safeCounter.hh"

class join_threads {
  std::vector<std::thread> &threads;

public:
  explicit join_threads(std::vector<std::thread> &threads_)
      : threads(threads_) {}
  ~join_threads() {
    // std::cerr << "destructing joiner\n";
    for (unsigned long i = 0; i < threads.size(); ++i) {
      if (threads[i].joinable()) {
        threads[i].join();
      }
    }
  }
};

class thread_pool {

private:
  // std::atomic_bool done;
  threadsafe_counter<int> finish_work;
  threadsafe_queue<std::function<void()>> work_queue;
  std::vector<std::thread> threads;
  threadsafe_hash<std::thread::id,int> m;

  void worker_thread() {
    while (!done) {
      // if (work_queue.size() != 0) cout << "work_queue.size(): " << work_queue.size() << endl;
      std::function<void()> task;
      if (work_queue.try_pop(task)) {
        // std::cerr << "I'm " << std::this_thread::get_id() << std::endl;
        // cout << "hey " << m.count(std::this_thread::get_id()) << endl;
        {
          auto tid = std::this_thread::get_id();
          if (m.count(tid) <= 0) {
            m.insert(tid, 1);
          } else {
            threadsafe_counter<int> c;
            c.initialize(m.get(tid));
            c.increment();
            m.insert(tid, c.get());
          }
        }
        task();
        finish_work.increment();
        // cout << "finish_work " << finish_work.get() << endl;
      } else {
        std::this_thread::yield();
      }
    }
  }

public:
  std::atomic_bool done;
  join_threads *joiner;
  thread_pool() : done(false), joiner(new join_threads(threads)) {
    finish_work.initialize(0);
    unsigned const thread_count = std::thread::hardware_concurrency();
    std::cerr << "Creating pool with " << thread_count << " threads" << endl;
    try {
      for (unsigned i = 0; i < thread_count; ++i) {
        threads.push_back(std::thread(&thread_pool::worker_thread, this));
      }
    } catch (...) {
      done = true;
      throw;
    }
  }
  ~thread_pool() {
    done = true;
    joiner->~join_threads();
    std::string s("Destructing pool ");
    s += std::to_string(work_queue.empty());
    s += '\n';
    std::cerr << s;
  }
  template <typename FunctionType> void submit(FunctionType f) {
    work_queue.push(std::function<void()>(f));
    //    std::cerr << std::this_thread::get_id() << std::endl;
  }

  int getWorkersCount() {
    return m.size();
    // m.print();
  }

  void barrier(int work_needed) {
    while(finish_work.get() < work_needed && !work_queue.empty()) {}
    finish_work.initialize(0);
  }
};
