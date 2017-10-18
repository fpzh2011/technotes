# docker概要

## 参考资料

### 快速入门

http://www.jianshu.com/p/9a4cb9067632
https://zhuanlan.zhihu.com/p/27187231
https://www.qcloud.com/community/article/164816001481011805
https://www.qcloud.com/community/article/164816001481011808
https://www.qcloud.com/community/article/164816001481011809
http://dockone.io/article/1412
https://coolshell.cn/articles/17061.html
容器连接/网络连接。翻译的docker官网教程
http://www.oschina.net/translate/dockerlinks

### 速查手册

https://wenku.baidu.com/view/d512154d58f5f61fb6366686.html
http://qiankunli.github.io/2014/07/10/overview_of_docker.html

### github book

https://www.gitbook.com/book/yeasy/docker_practice/details
https://yeasy.gitbooks.io/docker_practice/content/
https://jiajially.gitbooks.io/dockerguide/content/chapter_fastlearn/index.html
https://hujb2000.gitbooks.io/docker-flow-evolution/content/cn/index.html

### 用户定义网络
https://stackoverflow.com/questions/41768157/how-to-link-container-in-docker
https://stackoverflow.com/questions/32744509/docker-network-link-to-2-or-multiple-containers



start可以启动一个容器，但是退出后，容器也退出了。能否以dameon方式start一个已经创建的容器？如果attach一个dameon容器，退出后，容器会退出吗？



## 安装

ubuntu 16.04
https://docs.docker.com/engine/installation/linux/docker-ce/ubuntu/
```shell
sudo apt-get update
sudo apt-get install \
    linux-image-extra-$(uname -r) \
    linux-image-extra-virtual
sudo apt-get update
sudo apt-get install \
    apt-transport-https \
    ca-certificates \
    curl \
    software-properties-common
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
sudo add-apt-repository \
   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
   $(lsb_release -cs) \
   stable"
sudo apt-get update #必须执行，否则可能找不到docker-ce安装包
sudo apt-get install docker-ce
sudo docker run hello-world #验证安装成功
sudo ls -l /var/lib/docker
docker -v
systemctl start docker #启动Docker服务
sudo docker run -d --name mynginx nginx #启动nginx镜像，本地没有会自动pull
# https://www.digitalocean.com/community/tutorials/how-to-run-nginx-in-a-docker-container-on-ubuntu-14-04
sudo docker run --name docker-nginx -p 80:80 nginx
# 指定数据卷映射目录。: 的左侧是宿主机目录。查看数据卷映射 docker inspect -f {{.Volumes}} docker-nginx
sudo docker run --name docker-nginx -p 80:80 -d -v ~/docker-nginx/html:/usr/share/nginx/html nginx
sudo docker stop bfd094233f96 #停止一个容器，根据容器 id 进行删除
sudo docker rm bfd094233f96 #删除一个容器
sudo docker attach d20f3dc6cd92 #进入一个正在运行的容器
sudo docker inspect nginx #查看镜像配置
# 查看某个镜像构建历史。
sudo docker history image_id
# 或者用 https://github.com/justone/dockviz/releases
# sudo ./dockviz images apache2 --tree
# 查看镜像
sudo ls -l /var/lib/docker/aufs/diff/<id>
# 启动docker实例后，查看aufs mount情况
cat /sys/fs/aufs/si_411895d330e96eaa/*
# 从宿主机进入已经启动的docker实例
sudo docker exec -it c982aae09cb3 bash
sudo docker attach c982aae09cb3
# 启动一个已经创建的容器
sudo docker start -i myubuntu
# 在容器内运行交互式命令，命令退出后，容器不会退出
sudo docker exec -t -i myubuntu /bin/bash
# 启动永久运行的ubuntu
sudo docker run -d --name ubuntu-d ubuntu:16.04 /bin/sh -c "while true; do echo hello; sleep 1; done"
```

[配置国内镜像加速源](http://www.jianshu.com/p/9a4cb9067632)
```shell
sudo vim /etc/systemd/system/multi-user.target.wants/docker.service
# ExecStart -> ExecStart=/usr/bin/dockerd --registry-mirror=https://jxus37ad.mirror.aliyuncs.com
sudo systemctl daemon-reload
sudo systemctl restart docker
sudo docker pull ubuntu:16.04
sudo docker run -it --rm ubuntu:16.04 bash
```

## 应用的端口访问

https://www.1and1.com/cloud-community/learn/containers/docker/using-redis-in-docker-containers/

以 redis 为例。访问 redis docker 有以下几种方式：
1、手工操作。在宿主机上通过 sudo docker attach 进入 redis docker 实例，然后通过 redis-cli 访问。但这不适用于Java程序。
2、通过 -p 7001:6379 进行端口转发，这样会暴露端口 7001 给宿主机外部。
3、通过 --link 实现同一宿主机的不同 docker 之间的通讯。

## 每个运行的容器都有一个独立的进程ID

https://stackoverflow.com/questions/25101312/does-all-running-docker-containers-have-a-separate-process-id
`docker inspect --format '{{ .State.Pid }}' CONTAINER_ID`

## 问题

如果宿主机用`sudo docker network create sxs`创建了网络，启动docker容器时如果不指定网络，可能无法连接外网。

### apt update太慢

http://dockone.io/question/154
`
RUN sed -i 's/http:\/\/archive\.ubuntu\.com\/ubuntu\//http:\/\/mirrors\.163\.com\/ubuntu\//g' /etc/apt/sources.list
RUN sed -i 's#http://archive\.ubuntu\.com/ubuntu/#http://mirrors\.163\.com/ubuntu/#g' /etc/apt/sources.list
`
sed #

## 疑问

防火墙ufw、iptables配置：p17,116
docker中的程序如果创建多个进程，宿主机也会相应创建进程吗？？？？



