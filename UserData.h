#include <string>
using namespace std;
#pragma once

class UserData{
private:
    string name;
    bool isActive;
public:
    UserData() {};
    UserData(string _name);
    string getName();
    void setName(string _name);
    bool getIsActive();
    void switchIsActive();
};