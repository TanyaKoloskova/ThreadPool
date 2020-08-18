#pragma once
#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <vector>


class ThreadPool
{
 private:
  bool alive;
  std::mutex mutex_;
  std::condition_variable notifier;
  std::vector<std::thread> threads;
  std::queue<std::packaged_task<void()>> tasks;

  void ThreadFunction(){
    std::packaged_task<void()> task;
    while (alive) {
      {
        std::unique_lock<std::mutex> lock(mutex_);
        while( alive && tasks.empty() )
          notifier.wait(lock);
        if (!alive && tasks.empty())
          return;
        task = std::move(tasks.front());
        tasks.pop();
      }
      task();
    }
  }

 public:
  //constructors
  explicit ThreadPool(size_t poolSize): alive(true) {
    for (size_t i = 0; i < poolSize; ++i) {
      threads.emplace_back(&ThreadPool::ThreadFunction, this);
    }
  }
  ~ThreadPool() {
    {
      std::lock_guard<std::mutex> lock(mutex_);
      alive = false;
    }
    notifier.notify_all();
    for (auto &&thread : threads)
      thread.join();
  }

  template <class Func, class... Args>
  auto Exec(Func&& func, Args&&... args){
    using return_type = decltype(func(args...));
    std::packaged_task<return_type()> task(std::bind(func, args...));
    auto result = task.get_future();
    {
      std::lock_guard<std::mutex> lock(mutex_);
      tasks.emplace(std::move(task));
    }
    notifier.notify_one();
    return result;
  }

  template <typename Func>
  auto Exec(Func&& func) {
    using return_type = decltype(func());
    std::packaged_task<return_type()> task(std::forward<Func>(func));
    auto result = task.get_future();
    {
      std::lock_guard<std::mutex> lock(mutex_);
      tasks.emplace(std::move(task));
    }
    notifier.notify_one();
    return result;
  }
};
