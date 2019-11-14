package main

import (
"fmt"
"github.com/go-redis/redis"
)

func main() {
	client := getRedisClient()
	testMGet(client)
	//testString(client)
	//testList(client)
	//testHash(client)
	//testSet(client)
}

func testSet(client *redis.Client) {
	s, err := client.SRandMember("set").Result()
	if err == redis.Nil { // 如果key不存在，单个值的err是redis.Nil
		fmt.Println("member set key not exist")
	}
	if err != nil {
		fmt.Printf("error: %v\n", err)
	}
	fmt.Printf("rand member: %v\n", s)
	//members
	all, ea := client.SMembers("set").Result()
	if ea == redis.Nil {
		// 不会进入这里
		fmt.Println("all set key not exist")
	}
	if ea != nil {
		fmt.Printf("error: %v\n", err)
	}
	fmt.Printf("all member: %v\n", all)
}

func testHash(client *redis.Client) {
	// hget
	result, err := client.HGet("hash", "field").Result()
	if err == redis.Nil { // hash key或field不存在，err都是redis.Nil
		fmt.Println("hash or field not exist")
	}
	if err != nil && err != redis.Nil {
		fmt.Printf("error: %v\n", err)
		return
	}
	fmt.Printf("hget result: %v\n", result)
	// hgetall
	ra, ea:= client.HGetAll("hash").Result()
	if ea == redis.Nil { // hash key如果不存在，err是nil。ra是空map{}(不是nil)
		// 不会进入这里
		fmt.Println("hash not exist")
	}
	if ea != nil && ea != redis.Nil {
		fmt.Printf("error: %v\n", ea)
		return
	}
	fmt.Printf("hgetall result: %v %v\n", ra, ra == nil)
}

func testList(client *redis.Client) {
	// stop(如100)超过list size，返回整个list
	result, err := client.LRange("list", 0, -1).Result()
	if err == redis.Nil { // list如果不存在，err是nil，而不是redis.Nil。因为list只要为空，key就被删除。
		// 不会进入这里
		fmt.Println("list key not exist")
		return
	}
	if err != nil {
		fmt.Printf("error: %v\n", err)
		return
	}
	fmt.Printf("result: %v\n", result)
}

func testString(client *redis.Client) {
	str, err := client.Get("zjh").Result()
	if err == redis.Nil { // string如果不存在，err是redis.Nil。因为string的值可以设置为""
		fmt.Println("string key not exist")
		return
	}
	if err != nil {
		fmt.Printf("error: %v\n", err)
		return
	}
	fmt.Printf("string: %v\n", str)
}

func testMGet(client *redis.Client) {
	// 如果key不存在，对应位置的值是nil
	result, err := client.MGet("zjh", "uh").Result()
	if err == redis.Nil { // 不会进入这里
		fmt.Println("string key not exist")
		return
	}
	if err != nil {
		fmt.Printf("error: %v\n", err)
		return
	}
	fmt.Printf("multiple string: %v\n", result)
}


// go ttl:
// * key不存在，-2 * time.Second
// * key存在，但没有设置超时，-1 * time.Second
// * ttl * time.Second

func getRedisClient() *redis.Client {
	return redis.NewClient(&redis.Options{Addr: "localhost:6379"})
}
