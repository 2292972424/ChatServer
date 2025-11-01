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

```
#方式一：使用构建脚本
./build.sh

#方式二：手动编译
cd build
cmake ..
make
```

# 数据库初始化
```
#自动生成MySQL表结构
./chat.sql
```

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
```
#1. 编辑Redis配置文件
sudo vim /etc/redis/redis.conf

#2. 注释掉或删除requirepass行
# requirepass your_password

#3. 重启Redis服务
sudo systemctl restart redis-server
```
# 数据明文传输的安全问题

## 对称加密算法

采用单钥密码系统的加密方法，同一个密钥用来解密和加密，常见的对称加密算法有DES,3DES,AES,RC2,RC4,RC5等

优点：

加密计算量小，速度快，适合对大数据量进行加密的场景。

缺点：

密钥传输问题和管理问题。

### AES加密算法

AES加密算法采用分组密码体制，每个分组数据的长度为128位16个字节，密钥长度可以是128位16个字节、192位或256位，一共有四种加密模式，我们通常采用需要初始向量IV的CBC模式，初始向量的长度也是128位16个字节。

AES加密的五个关键词，分别是：分组密码体制、Padding、密钥、初始向量IV和四种加密模式。

分组密码体制：

所谓分组密码体制就是指将明文切成一段一段的来加密，然后再把一段一段的密文拼起来形成最终密文的加密方式。AES采用分组密码体制，
即AES加密会首先把明文切成一段一段的，而且每段数据的长度要求必须是128位16个字节，如果最后一段不够16个字节了，就
需要用Padding来把这段数据填满16个字节，然后分别对每段数据进行加密，最后再把每段加密数据拼起来形成最终的密文。

Padding：

Padding就是用来把不满16个字节的分组数据填满16个字节用的，它有三种模式PKCS5、PKCS7和NOPADDING。PKCS5是指分组数
据缺少几个字节，就在数据的末尾填充几个字节的几，比如缺少5个字节，就在末尾填充5个字节的5。PKCS7是指分组数据缺少几个字
节，就在数据的末尾填充几个字节的0，比如缺少7个字节，就在末尾填充7个字节的0。NoPadding是指不需要填充，也就是说数据的发
送方肯定会保证最后一段数据也正好是16个字节。那如果在PKCS5模式下，最后一段数据的内容刚好就是16个16怎么办？那解密端就不
知道这一段数据到底是有效数据还是填充数据了，因此对于这种情况，PKCS5模式会自动帮我们在最后一段数据后再添加16个字节的数
据，而且填充数据也是16个16，这样解密段就能知道谁是有效数据谁是填充数据了。PKCS7最后一段数据的内容是16个0，也是同样的
道理。解密端需要使用和加密端同样的Padding模式，才能准确的识别有效数据和填充数据。我们开发通常采用PKCS7Padding模式。

密钥：

AES要求密钥的长度可以是128位16个字节、192位或者256位，位数越高，加密强度自然越大，但是加密的效率自然会低一些，因此要
做好衡量。我们开发通常采用128位16个字节的密钥，我们使用AES加密时需要主动提供密钥，而且只需要提供一个密钥就够了，每段
数据加密使用的都是这一个密钥，密钥来源为随机生成（我们开发时传入的那个为初始密钥，除了初始密钥以外，后面每一轮的密钥都是
由上一轮的密钥扩展而来的，密钥扩展有四个步骤：排列、置换、与轮常量异或、生成下一轮密钥的其他列）。

初始向量IV：

初始向量IV的作用是使加密更加安全可靠，我们使用AES加密时需要主动提供初始向量，而且只需要提供一个初始向量就够了，后面每
段数据的加密向量都是前面一段的密文。初始向量IV的长度规定为128位16个字节，初始向量的来源为随机生成。至于为什么初始向量
能使加密更安全可靠，会在下面的加密模式中提到。

四种加密模式：

AES一共有四种加密模式，分别是ECB（电子密码本模式）、CBC（密码分组链接模式）、CFB、OFB，我们一般使用的是CBC模
式。四种模式中除了ECB相对不安全之外，其它三种模式的区别并没有那么大（ECB模式是最基本的加密模式，即仅仅使用明文和密钥
来加密数据，相同的明文块会被加密成相同的密文块，这样明文和密文的结构将是完全一样的，就会更容易被破解，相对来说不是那么安
全，因此很少使用，而CBC模式则比ECB模式多了一个初始向量IV，加密的时候，第一个明文块会首先和初始向量IV做异或操作，然
后再经过密钥加密，然后第一个密文块又会作为第二个明文块的加密向量来异或，依次类推下去，这样相同的明文块加密出的密文块就是
不同的，明文的结构和密文的结构也将是不同的，因此更加安全，我们常用的就是CBC加密模式）。

## 非对称加密算法

非对称加密需要两个密钥：公钥（publickey）和私钥（privatekey)。公钥和私钥是一对，如果用公钥对数据加密，那么只能用对应
的私钥解密。如果用私钥对数据加密，只能用对应的公钥进行解密。因为加密和解密用的是不同的密钥，所以称为非对称加密。

算法强度复杂、安全性依赖于算法与密钥但是由于其算法复杂，而使得加密解密速度没有对称加密解密的速度快。对称密码体制中
只有一种密钥，并且是非公开的，如果要解密就得让对方知道密钥。所以保证其安全性就是保证密钥的安全，而非对称密钥体制有
两种密钥，其中一个是公开的，这样就可以不需要像对称密码那样传输对方的密钥了。这样安全性就大了很多。

### RSA算法

简介：

RSA是目前最有影响力的公钥加密算法，该算法基于一个十分简单的数论事实：将两个大素数相乘十分容易，但想要对其乘积进行因式
分解却极其困难，因此可以将乘积公开作为加密密钥，即公钥，而两个大素数组合成私钥。公钥是可发布的供任何人使用，私钥则为自己
所有，供解密之用。

工作流程：

A要把信息发给B为例，确定角色：A为加密者，B为解密者。首先由B随机确定一个KEY，称之为私钥，将这个KEY始终保存在机器
B中而不发出来；然后，由这个KEY计算出另一个KEY，称之为公钥。这个公钥的特性是几乎不可能通过它自身计算出生成它的私钥。
接下来通过网络把这个公钥传给A，A收到公钥后，利用公钥对信息加密，并把密文通过网络发送到B，最后B利用已知的私钥，就能对
密文进行解码了。以上就是RSA算法的工作流程。

运算速度：

由于进行的都是大数计算，使得RSA最快的情况也比DES慢上好几倍，无论是软件还是硬件实现。速度一直是RSA的缺陷。一般来说
只用于少量数据加密。RSA的速度是对应同样安全级别的对称密码算法的1/1000左右。
比起DES和其它对称算法来说，RSA要慢得多。实际上一般使用一种对称算法来加密信息，然后用RSA来加密比较短的公钥，然后将
用RSA加密的公钥和用对称算法加密的消息发送给接收方。
这样一来对随机数的要求就更高了，尤其对产生对称密码的要求非常高，否则的话可以越过RSA来直接攻击对称密码。

## 在实际项目中可以增加的加密方法

先生成一对非对称的公钥和私钥，私钥保存在服务器端，同时使用AES密钥，用公钥将AES密钥加密成密文来发送到服务端，服务端解密后得到AES密钥进行对称的加密数据传输，在保证传输效率的同时增加了安全性。
