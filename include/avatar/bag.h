#ifndef BAG_H_
#define BAG_H_
#include <string>
#include <unordered_map>

class Bag {
public:
    bool isempty() const;
    bool isfull() const;
    void addobj(std::string key, int addnum);
    int  count(std::string key);
    void reduce(std::string key);
    void openbag();
    Bag();
private:
    std::unordered_map<std::string, int> bag;
    int currentnum;
    int maxnum;
};
#endif  // BAG_H_
