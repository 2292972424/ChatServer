#include "offlinemessagemodel.hpp"
#include "db.h"
#include <muduo/base/Logging.h>

//存储用户的离线消息
void OfflineMsgModel::insert(int userid,string msg)
{
        MySQL mysql;
    if(mysql.connect())
    {
        // 转义消息内容（如果MySQL类中有escapeString方法）
        //string escapedMsg = mysql.escapeString(msg);
        
        // 1. 组装sql语句
        char sql[1024] = {0};
        sprintf(sql, "insert into offlinemessage values(%d, '%s')", userid, msg.c_str());
        
        // 添加调试日志
        LOG_INFO << "Insert offline message SQL: " << sql;
        
        // 执行更新
        if (!mysql.update(sql)) {
            LOG_ERROR << "Insert offline message failed for user: " << userid;
        } else {
            LOG_INFO << "Insert offline message success for user: " << userid;
        }
    }
    else
    {
        LOG_ERROR << "Database connection failed when inserting offline message";
    }
}
//删除用户的离线消息
void OfflineMsgModel::remove(int userid)
{
    //1.组装sql语句
    char sql[1024]={0};
    sprintf(sql,"delete from offlinemessage where userid=%d",userid);
    
    MySQL mysql;
    if(mysql.connect())
    {
        mysql.update(sql);
    }
}
//查询用户的离线消息
vector<string> OfflineMsgModel::query(int userid)
{
    //1.组装sql语句
    char sql[1024]={0};
    sprintf(sql,"select message from offlinemessage where userid=%d",userid);

    vector<string> vec;
    MySQL mysql;
    if(mysql.connect())
    {
        MYSQL_RES *res=mysql.query(sql);
        if (res!=nullptr)
        {
            MYSQL_ROW row;
            //在循环中获取下一行(不要写在上面，会死循环)
            while((row = mysql_fetch_row(res)) != nullptr)
            {
                if(row[0] != nullptr) {
                vec.push_back(row[0]);
            }
            }
            mysql_free_result(res);
        }
    }
    return vec;
}