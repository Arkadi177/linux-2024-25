#include "blocking_queue.h"

int main(){
  BlockingQueue<int> Myqueue(10);
  std::thread producerThread([&Myqueue] { // Creates a producer thread
        for (int i = 1; i <= 20; ++i) {
            Myqueue.push(i);
        }
    });
  std::this_thread::sleep_for(std::chrono::seconds());
  std::thread consumerThread([&Myqueue] {
      for (int i = 1; i <= 20; ++i) {
          Myqueue.pop();
      }
  });
  producerThread.join();
  consumerThread.join();
  return 0;
}
