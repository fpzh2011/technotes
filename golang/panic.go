package main

import (
    "fmt"
    "time"
)

// 在协程r1中panic后，必需在r1中recover，在调用链的任何地方recover都可以，但必需是在r1内。
// 如果放到main中recover，程序仍会崩溃。
// r1 recover后，r2仍正常运行。

func main() {
    fmt.Println("main start")
    go r1()
    go r2()
    time.Sleep(2*time.Minute)
}

func r1() {
    defer func () {fmt.Println("defer in r1")} ()
    defer func() {
        if p := recover(); p != nil {
            fmt.Printf("recover panic in r1: %v\n", p)
        }
    }()
    f1()
}

func f1() {
    fmt.Println("in f1")
    defer func () {fmt.Println("defer in f1")} ()
    f2()
}

func f2() {
    fmt.Println("in f2")
    defer func () {fmt.Println("defer in f2")} ()
    for i := 1; i < 3; i++ {
        fmt.Println("f2")
        time.Sleep(2 * time.Second)
    }
    panic("exit from f2")
}

func r2() {
    for {
        fmt.Println("r2")
        time.Sleep(2*time.Second)
    }
}