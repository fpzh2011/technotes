# golang并发模式

## 参考资料

https://blog.golang.org/context
https://blog.golang.org/pipelines
https://blog.golang.org/go-concurrency-patterns-timing-out-and
https://blog.golang.org/advanced-go-concurrency-patterns
https://blog.golang.org/concurrency-is-not-parallelism

## 无缓冲channel

* 使用一个无缓冲channel接收结果
* 创建协程个数与任务个数相同，所以任务数量很大时不适用。
* 使用WaitGroup判断是否所有任务结束、全部协程退出
* Wait与close必须在一起、前后顺序执行。否则close之后，对channel的写入会panic。
* Wat必须在Add之后执行，否则Wait直接返回、没有任何作用。
* Wait/close必须在独立协程中执行。如果Wait在主协程执行，因为channel无缓冲(缓冲小于任务数也不行)，Done不会被执行，Wait会被阻塞，对sizes的读也无法执行，channel不能被读消费，整个程序死锁。
```golang
// Go程序设计语言P185
// https://github.com/adonovan/gopl.io/blob/master/ch8/thumbnail/thumbnail_test.go#L117
func makeThumbnails6(filenames <-chan string) int64 {
	sizes := make(chan int64)
	var wg sync.WaitGroup // number of working goroutines
	for f := range filenames {
		wg.Add(1)
		// worker
		go func(f string) {
			defer wg.Done()
			thumb, err := thumbnail.ImageFile(f)
			if err != nil {
				log.Println(err)
				return
			}
			info, _ := os.Stat(thumb) // OK to ignore error
			sizes <- info.Size()
		}(f)
	}

	// closer
	go func() {
		wg.Wait()
		close(sizes)
	}()

	var total int64
	for size := range sizes {
		total += size
	}
	return total
}
```

## 缓冲channel

[Anatomy of Channels in Go - Concurrency in Go](https://medium.com/rungo/anatomy-of-channels-in-go-concurrency-in-go-1ec336086adb)
Buffer size or channel capacity
* 任务队列和结果都使用buffer channel，cap与任务个数相同
* 通过close(tasks)明确所有任务都输出完毕，即表明这是固定数量的任务，不是无限期持续运行任务。
* worker协程数量可定制
* 通过WaitGroup判断所有worker协程是否都退出。
```golang
// https://play.golang.org/p/0rRfchn7sL1
package main

import (
	"fmt"
	"sync"
	"time"
)

// worker than make squares
func sqrWorker(wg *sync.WaitGroup, tasks <-chan int, results chan<- int, instance int) {
	for num := range tasks {
		time.Sleep(time.Millisecond)
		fmt.Printf("[worker %v] Sending result by worker %v\n", instance, instance)
		results <- num * num
	}

	// done with worker
	wg.Done() // defer
}

func main() {
	fmt.Println("[main] main() started")

	var wg sync.WaitGroup

	tasks := make(chan int, 10)
	results := make(chan int, 10)

	// launching 3 worker goroutines
	for i := 0; i < 3; i++ {
		wg.Add(1)
		go sqrWorker(&wg, tasks, results, i)
	}

	// passing 5 tasks
	for i := 0; i < 5; i++ {
		tasks <- i * 2 // non-blocking as buffer capacity is 10
	}

	fmt.Println("[main] Wrote 5 tasks")

	// closing tasks
	close(tasks)

	// wait until all workers done their job
	wg.Wait()
	// 这里可以加一个 results.close() 避免worker panic

	// receving results from all workers
	for i := 0; i < 5; i++ {
		result := <-results // non-blocking because buffer is non-empty
		fmt.Println("[main] Result", i, ":", result)
	}

	fmt.Println("[main] main() stopped")
}
```