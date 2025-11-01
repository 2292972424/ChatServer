# ChatSever
一个可以工作在nginx TCP负载均衡中的集群聊天服务器和客户端源码，基于muduo网络库、Redis、JSON和MySQL技术栈实现。

# 运行环境
Linux Ubuntu 24.04

<img width="1484" height="1018" alt="image" src="https://github.com/user-attachments/assets/5fa80000-3d36-4d44-ad79-2e218fa93357" />

# 环境搭建
1.JSON:直接下载json.hpp文件即可,在源码的thirdparty文件夹中。

2.boost+muduo网络库:参考源代码中的md文件。

3.redis:使用ubuntu自带的apt命令安装即可。

4.mysql:同上,可以使用DataGrip远程连接。

5.nginx:如果是较新的Ubuntu版本,安装1.14及以上的版本,用wget下载压缩包解压安装即可。

nginx.conf文件:

```
#nginx tcp loadbalance config

stream {
    upstream MyServer {
        server 127.0.0.1:6000 weight=1 max_fails=3 fail_timeout=30s;
        server 127.0.0.1:6002 weight=1 max_fails=3 fail_timeout=30s;
        
    }

    server {
        proxy_connect_timeout 1s;
        #proxy_timeout 3s;
        listen 8000;
        proxy_pass MyServer;
        tcp_nodelay on;
    }
}
```

6.cmake:在ubuntu上直接执行apt install cmake即可,vs上也要安装cmake方便编译。

7.开发环境:vs+linux(vs远程连接Ubuntu),网上教程很多,随便找一个就行。

# 数据库表结构
<img width="933" height="203" alt="image" src="https://github.com/user-attachments/assets/e65b3bac-c422-44b0-9f7f-95714df8b191" />

## User表
<img width="918" height="171" alt="image" src="https://github.com/user-attachments/assets/a64e6f5b-9587-45f9-8977-ab5685f8e475" />

## Offlinemessage表
<img width="920" height="102" alt="image" src="https://github.com/user-attachments/assets/468c2ea3-5b46-4f24-b533-c1a1bab3eed7" />

## allgroup表
<img width="918" height="136" alt="image" src="https://github.com/user-attachments/assets/57c03dca-e43b-4c68-b589-48e7e5c9e3b2" />

## groupuser表
<img width="919" height="137" alt="image" src="https://github.com/user-attachments/assets/7d6e20c5-c149-4e1d-816c-28897984e203" />

## friend表
<img width="918" height="100" alt="image" src="https://github.com/user-attachments/assets/57d6f0fd-9577-4a17-ac2e-9c853efce90a" />

# 代码结构

```
ChatServer/
├── src/
│   ├── server/           # 服务器端代码
│   │   ├── chatserver.cpp    # 服务器主类
│   │   ├── chatservice.cpp   # 业务逻辑处理
│   │   └── main.cpp          # 程序入口
│   ├── client/           # 客户端代码
│   └── public/           # 公共定义
├── model/                # 数据模型
│   ├── usermodel.cpp     # 用户数据操作
│   ├── friendmodel.cpp   # 好友关系操作
│   ├── groupmodel.cpp    # 群组数据操作
│   └── offlinemessagemodel.cpp  # 离线消息处理
├── db/                   # 数据库层
│   ├── db.cpp           # 数据库连接封装
│   └── dbconnection.cpp # 连接池管理（还未实现，后续可能会加）
├── redis/               # Redis 客户端
│   └── redis.cpp        # Redis 操作封装
├── thirdparty/          # 第三方库
├── bin/                 # 编译输出
├── build/               # 构建目录
└── conf/                # 配置文件
```

# 编译方式
下载源码后，执行./build.sh即可

或者进入build文件夹

执行cmake .. make

# 自动生成mysql表结构
执行./chat.sql即可


# 功能演示：

## 连接服务器和登录客户端

<img width="732" height="809" alt="image" src="https://github.com/user-attachments/assets/98248ea8-f3e1-4a5b-8314-d32986b480ea" />

<img width="1139" height="775" alt="image" src="https://github.com/user-attachments/assets/22e6e913-6e2d-46f8-b3ca-4db972ba162d" />

## 跨服务器聊天

<img width="1139" height="775" alt="image" src="https://github.com/user-attachments/assets/5fd1388e-48ab-4a9b-ac6a-d01a54856a94" />

# 常见问题

## nginx跨服务器通信问题

我在复刻项目的过程中遇到不能同时开启两台服务器的问题，检查了nginx.conf配置，也检查了nginx代码，还是不知道什么问题，最后在源代码的main.cpp中找到了问题所在：

<img width="1004" height="633" alt="image" src="https://github.com/user-attachments/assets/7cd78713-98f6-43f1-b319-1f2519f9935a" />

在项目初期服务器固定ip地址和端口号，但是加入nginx负载均衡后又不是固定的ip地址和端口号，这时我们的main.cpp中需要改成通过解析命令行来获取IP地址和端口号。

## redis问题

在项目最后要加入redis消息订阅功能实现跨服务器通信，redis运行是正常的，但是客户端连接上服务器时，选择login输入用户id和密码然后服务器端报了段错误问题，这个问题是redis有初始密码导致的。

我们需要进入redis的配置文件将有"requirepass"+"你的密码(可能是数字可能是字母)"的那一行注释掉，然后再去登录问题就解决了。
