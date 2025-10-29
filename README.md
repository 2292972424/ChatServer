# ChatSever
可以工作在nginx tcp负载均衡中的集群聊天服务器和客户端源码 基于muduo库 redis json mysql

# 代码结构
bin:可执行文件;

build:编译产生的文件;

include:头文件;

src:源代码;

test:项目初期的测试代码;

thirdparty:第三方库，主要是json.hpp;


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


