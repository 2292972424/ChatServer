#ifndef __GROUP_USER_H__
#define __GROUP_USER_H__

#include "user.hpp"
#include <string>

//群组用户，多了一个role角色信息
class GroupUser
{
public:
    GroupUser() = default;
    GroupUser(const User& user, const std::string& role = "") 
        : _user(user), _role(role) {}

    // 委托方法 - 访问User信息
    void setUser(const User& user) { _user = user; }
    User& getUser() { return _user; }
    const User& getUser() const { return _user; }

    // 直接访问User属性的便捷方法
    void setId(int id) { _user.setId(id); }
    int getId() { return _user.getId(); }
    
    void setName(const std::string& name) { _user.setName(name); }
    std::string getName() { return _user.getName(); }
    
    void setPwd(const std::string& pwd) { _user.setPwd(pwd); }
    std::string getPwd() { return _user.getPwd(); }
    
    void setState(const std::string& state) { _user.setState(state); }
    std::string getState() { return _user.getState(); }

    // 角色相关方法
    void setRole(const std::string& role) { _role = role; }
    std::string getRole() const { return _role; }

private:
    User _user;
    std::string _role;
};

#endif // __GROUP_USER_H__