#include "db.h"
#include <muduo/base/Logging.h>

// 数据库配置信息
static const string server = "127.0.0.1";
static const string user = "root";
static const string password = "123456";
static const string dbname = "chat";

// 初始化数据库连接
MySQL::MySQL()
{
    _conn = mysql_init(nullptr);
}

// 释放数据库连接资源
MySQL::~MySQL()
{
    if (_conn != nullptr) {
        mysql_close(_conn);
    }
}

// 连接数据库
bool MySQL::connect()
{
    // 检查连接是否已经初始化
    if (_conn == nullptr) {
        LOG_ERROR << "MySQL connection not initialized";
        return false;
    }
    MYSQL *p = mysql_real_connect(_conn, server.c_str(), user.c_str(),
                                  password.c_str(), dbname.c_str(), 3306, nullptr, 0);
    if (p != nullptr)
    {
        // 设置字符集前检查连接状态
        if (mysql_ping(_conn) != 0) {
            LOG_ERROR << "MySQL connection is not alive after connect";
            return false;
        }
        
        // C和C++代码默认的编码字符是ASCII，如果不设置，从MySQL上拉下来的中文显示？
        if (mysql_query(_conn, "set names utf8mb4") != 0) {
            LOG_ERROR << "set names utf8mb4 failed: " << mysql_error(_conn);
            // 不要立即返回false，连接可能仍然可用
        }
        LOG_INFO << "connect mysql successed!";
        // 验证连接确实工作
        if (mysql_query(_conn, "SELECT 1") != 0) {
            LOG_ERROR << "Test query failed: " << mysql_error(_conn);
            return false;
        }
        
        // 清理测试结果
        MYSQL_RES* result = mysql_store_result(_conn);
        if (result != nullptr) {
            mysql_free_result(result);
        }
    }
    else
    {
        LOG_ERROR << "connect mysql failed: " << mysql_error(_conn);
        LOG_ERROR << "Server: " << server << ", User: " << user 
                  << ", Database: " << dbname;
        return false;
    }

    return p;
}

// 更新操作
bool MySQL::update(string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
                 << sql << "更新失败!";
        return false;
    }

    return true;
}

// 查询操作
MYSQL_RES *MySQL::query(string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
                 << sql << "查询失败!";
        return nullptr;
    }
    
    return mysql_use_result(_conn);
}

// 获取连接
MYSQL* MySQL::getConnection()
{
    return _conn;
}