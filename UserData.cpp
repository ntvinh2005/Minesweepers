#include "UserData.h"

UserData::UserData(string _name) {
    name = _name;
    isActive = true;
}

string UserData::getName() {
    return name;
}

void UserData::setName(string _name) {
    name = _name;
}

bool UserData::getIsActive() {
    return isActive;
}

void UserData::switchIsActive() {
    if (isActive) isActive = false;
    else isActive = true;
}