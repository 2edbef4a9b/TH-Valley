#ifndef ILLUSTRATEDGUIDE_H_
#define ILLUSTRATEDGUIDE_H_
#include <string>
#include <unordered_map>

class Illustratedguide {
public:

    Illustratedguide();
    std::string read(std::string key);
    static std::unordered_map<std::string, std::string> Getguide();
    void loadguide();
    void showdesignated(std::string key);
    void showtotal();
 private:
    std::unordered_map<std::string, std::string> guide;  // ∂®“Â
    int totalnum;
};
#endif  // ILLUSTRATEDGUIDE_H_
