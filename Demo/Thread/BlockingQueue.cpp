#include "BlockingQueue.h"
#include <thread>

int main(){
    std::mutex m;
    std::condition_variable cv;
    BlockingQueue<int>* Myqueue = new BlockingQueue<int>(m , cv , 10);
    std::thread producerThread([&Myqueue] {
    for (int i = 1; i <= 20; ++i) {
        Myqueue->push(i);
    }
    });
    // std::this_thread::sleep_for(std::chrono::seconds(3));
    std::thread consumerThread([&Myqueue] {
    for (int i = 1; i <= 20; ++i) {
      Myqueue->pop();
    }
    });

    producerThread.join();
    consumerThread.join();
  return 0;
}
