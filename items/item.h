#pragma once
#include <string>
#include <memory>
#include <map>

class Item {
public:
    std::string id;
    std::string displayName;
    std::string commandName;
    std::string description;
    std::string type;
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

class ContainerItem : public Item {
public:
    bool locked = true;
    std::string code;
    std::map<std::string, std::shared_ptr<Item>> contents;
};

class ConsumableItem : public Item {
public:
    bool consumed = false;
    std::string effect;
};