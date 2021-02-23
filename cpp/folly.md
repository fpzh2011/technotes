# folly

## future

https://github.com/facebook/folly/blob/master/folly/docs/Futures.md
https://yq.aliyun.com/articles/115416
https://www.infoq.cn/article/2015/07/facebook-folly-futures


folly和vert.x的问题：
* 无法统计中间步骤的失败次数


## 特定版本

https://github.com/facebook/folly/commit/b0ec99828f0c782a38947513c5e0c242984e269e.patch

## 线程池

https://github.com/facebook/folly/blob/master/folly/docs/Executors.md
https://github.com/facebook/folly/blob/master/folly/executors/CPUThreadPoolExecutor.h
folly/wangle的线程池，full时有两种策略：
* BLOCK: 可能发生死锁
* THROW: 不会死锁，但是如果添加失败，返回的Try是异常。
一般可以用THROW同时给队列设置一个充分大的size。或者确认任务之间没有相互依赖的锁。
或者直接用:
https://github.com/progschj/ThreadPool
https://github.com/inkooboo/thread-pool-cpp


这种方式结合grpc sync server，实现异步操作。


有一个奇怪的现象，就是某些任务返回的try是异常，但是仍会执行。
不知到master最新版本是否存在这种情况。
```c++
// GetQueueSize = 1
// ./server -global_worker_threads=1
#include <string>
#include <vector>

#include <folly/futures/Future.h>

#include "async/async_executor.h"

DECLARE_bool(block_on_global_cpu_pool_full);

folly::Future<bool> do_work(std::string line) {
  LOG(INFO) << "input: " << line << " thread id: " << std::this_thread::get_id();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  LOG(INFO) << "end " << std::this_thread::get_id();
  return true;
}

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  FLAGS_log_dir = "./logs";
  FLAGS_logbufsecs = 0;

  // FLAGS_block_on_global_cpu_pool_full = true;
  LOG(INFO) <<  "FLAGS_block_on_global_cpu_pool_full: " << FLAGS_block_on_global_cpu_pool_full;

  std::vector<folly::Future<bool>> futures;
  std::string line;
  while(std::getline(std::cin, line)) {
    futures.emplace_back(common::async([line](){
      return do_work(line);
    }));
  }
  folly::Future<std::vector<folly::Try<bool>>> fs = folly::collectAll(futures);
  int i = 0;
  for (auto& t: fs.get()) {
    i++;
    if (t.hasException()) {
      LOG(ERROR) << "exception " << i << " " << t.exception().what();
    } else {
      LOG(INFO) << "succ " << i;
    }
  }

  std::this_thread::sleep_for(std::chrono::seconds(10));
  google::ShutdownGoogleLogging();
  return 0;
}
```
