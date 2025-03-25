#include <iostream>
#include <string>

class TowerOfHanoi {
public:
    static void hanoi(int n) {
        if (n > 0) {
            f(n, "1", "0", "-1");
        }
    }

private:
    static void f(int i, const std::string& from, const std::string& to, const std::string& other) {
        if (i == 1) {
            std::cout << "from " << from << " to " << to << std::endl;
        } else {
            f(i - 1, from, other, to);
            std::cout << " " << i << " from " << from << " to " << to << std::endl;
            f(i - 1, other, to, from);
        }
    }
};

int main() {
    int n = 3;
    TowerOfHanoi::hanoi(n);
    return 0;
}
