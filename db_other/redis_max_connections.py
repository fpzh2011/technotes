#! /usr/bin/env python
# -*- coding: utf-8 -*-

import logging
import sys
import time
from redis import Redis, ConnectionPool
from threading import Thread


'''
测试redis连接数超过max_connections后抛出异常
'''
def brpop(pool):
    try:
        client = Redis(connection_pool=pool)
        logging.info(client.brpop('test'))
    except:
        logging.exception('redis failed')


def main():
    logging.basicConfig(stream=sys.stdout, level=logging.INFO,
                        format="%(levelname)s %(asctime)s " +
                        "[%(filename)s %(funcName)s %(lineno)d] %(message)s")
    max_conn = 2
    conf = {
            "host": "localhost",
            "port": 6379,
            "max_connections": max_conn,
        }
    pool = ConnectionPool(**conf)

    for i in range(max_conn + 1):
        t = Thread(target=brpop, args=(pool,))
        t.daemon = True
        t.start()
    time.sleep(2)


if __name__ == '__main__':
    main()
