#include <iostream>
#include "item.h"

std::string Item::onInspect() const {
    return description;  // default — visa bara description
};

std::string ReadableItem::onInspect() const {
    return description + "\nYou read: " + text;  // visa description + text
};