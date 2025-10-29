#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <muduo/net/TcpServer.h>
#include <unordered_map>
#include <functional>
#include <mutex>
#include "UserModel.hpp"
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "group_model.hpp"
#include "json.hpp"
#include "redis.hpp"
using namespace std;
using namespace muduo;
using namespace muduo::net;
using json=nlohmann::json;
//处理消息的事件回调方法类型
using MsgHandler=std::function<void(const TcpConnectionPtr &conn,json &js,Timestamp)>;

//聊天服务器业务类
class ChatService
{
private:
    //存储消息id和其他对应的业务处理方法
    unordered_map<int,MsgHandler> _msgHandlerMap;
    ChatService();
    ChatService(const ChatService&) = delete;
    ChatService& operator=(const ChatService&) = delete;
    //存储在线用户的通信连接
    unordered_map<int,TcpConnectionPtr> _userConnMap;
    //数据操作类对象
    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;
    //定义互斥锁，保证_userConnMap的线程安全
    mutex _connMutex;
    //redis 操作对象
    Redis _redis;

public:
    //获取单例对象的接口函数
    static ChatService *instance();
    //处理登录业务
    void login(const TcpConnectionPtr&conn,json &js,Timestamp time);
    //处理注册业务
    void reg(const TcpConnectionPtr&conn,json &js,Timestamp time);
    //获取消息对应的处理器
    MsgHandler gethandler(int msgid);
    //处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);
    //处理服务器异常，业务重置方法
    void reset();
    //一对一聊天业务
    void oneChat(const TcpConnectionPtr&conn,json &js,Timestamp time);
    //添加好友业务
    void addFriend(const TcpConnectionPtr&conn,json &js,Timestamp time);
    //创建群组业务
    void createGroup(const TcpConnectionPtr&conn,json &js,Timestamp time);
    //加入群组业务
    void addGroup(const TcpConnectionPtr&conn,json &js,Timestamp time);
    //群组聊天业务
    void groupChat(const TcpConnectionPtr&conn,json &js,Timestamp time);
    //处理注销业务
    void loginout(const TcpConnectionPtr&conn,json &js,Timestamp time);
    //redis订阅消息触发的回调函数
    void redis_subscribe_message_handler(int channel, string message);
};

#endif // !CH