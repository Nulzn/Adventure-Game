#pragma once
#include <string>

class Item {
public:
    std::string id;
    std::string displayName;
    std::string commandName;
    std::string description;
    virtual std::string onInspect() const;
    virtual ~Item() = default;
};

class KeyItem : public Item {
public:
    std::string unlocksRoomId;  // vilken dörr den låser upp
};

class ReadableItem : public Item {
public:
    std::string text;  // texten som visas när man läser
    std::string onInspect() const override;
};

