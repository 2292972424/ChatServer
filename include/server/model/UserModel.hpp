#ifndef USERMODEL_H
#define USERMODEL_H
#include "user.hpp"

//User表的数据操作类
class UserModel
{
private:
    /* data */
public:
    //User表的增加
    bool insert(User &user);
    //User表的查询
    User query(int id);
    //User表的更新
    bool updateState(User user);
    //重置用户的状态信息
    void resetState();
};

#endif // !CH