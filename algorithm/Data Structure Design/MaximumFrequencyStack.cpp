#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>

class FreqStack {
private:
    // 记录当前栈中元素出现的最大次数
    int topTimes;

    // 按照出现次数分组存储元素，key为出现次数，value为对应的元素列表
    std::unordered_map<int, std::vector<int>> cntValues;

    // 记录每个元素出现的次数，key为元素值，value为出现次数
    std::unordered_map<int, int> valueTimes;

public:
    // 构造函数，初始化最大出现次数为0
    FreqStack() : topTimes(0) {}

    // 将一个元素压入栈中
    void push(int val) {
        // 更新该元素的出现次数
        valueTimes[val]++;

        // 获取当前元素的出现次数
        int curTopTimes = valueTimes[val];

        // 如果当前出现次数的分组不存在，则创建一个新的分组
        if (cntValues.find(curTopTimes) == cntValues.end()) {
            cntValues[curTopTimes] = std::vector<int>();
        }

        // 将元素加入到对应出现次数的分组中
        cntValues[curTopTimes].push_back(val);

        // 更新最大出现次数
        topTimes = std::max(topTimes, curTopTimes);
    }

    // 从栈中弹出一个元素
    int pop() {
        // 获取当前最大出现次数对应的元素列表
        std::vector<int>& topTimeValues = cntValues[topTimes];

        // 获取并移除最后一个元素（即最近插入的元素）
        int ans = topTimeValues.back();
        topTimeValues.pop_back();

        // 如果当前最大出现次数对应的分组为空，则删除该分组，并减少最大出现次数
        if (topTimeValues.empty()) {
            cntValues.erase(topTimes);
            topTimes--;
        }

        // 更新被移除元素的出现次数
        if (--valueTimes[ans] == 0) {
            // 如果该元素只出现了一次，则从valueTimes中移除
            valueTimes.erase(ans);
        }

        // 返回被移除的元素
        return ans;
    }
};


int main() {
    FreqStack stack;

    // 测试 push 操作
    stack.push(5);
    stack.push(7);
    stack.push(5);
    stack.push(7);
    stack.push(4);
    stack.push(5);

    // 测试 pop 操作
    std::cout << "Popped: " << stack.pop() << std::endl; // 应该输出 5
    std::cout << "Popped: " << stack.pop() << std::endl; // 应该输出 7
    std::cout << "Popped: " << stack.pop() << std::endl; // 应该输出 5
    std::cout << "Popped: " << stack.pop() << std::endl; // 应该输出 4

    return 0;
}