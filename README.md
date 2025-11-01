# ChatSever
可以工作在nginx tcp负载均衡中的集群聊天服务器和客户端源码 基于muduo库 redis json mysql

# 运行环境
Linux Ubuntu 24.04

<img width="1484" height="1018" alt="image" src="https://github.com/user-attachments/assets/5fa80000-3d36-4d44-ad79-2e218fa93357" />

# 环境搭建
1.JSON：直接下载json.hpp文件即可,在源码的thirdparty文件夹中

2.boost+muduo网络库:参考

# 代码结构
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

│   └── dbconnection.cpp # 连接池管理

├── redis/               # Redis 客户端

│   └── redis.cpp        # Redis 操作封装

├── thirdparty/          # 第三方库

├── bin/                 # 编译输出

├── build/               # 构建目录

└── conf/                # 配置文件


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


