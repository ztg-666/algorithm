#include <iostream>
#include <unordered_map>
#include <sstream>

using namespace std;

// 使用 unordered_map 存储键值对，其中键是整数，值是一个 pair，包含实际值和插入/更新的时间戳
unordered_map<int, pair<int, int>> map;
// setAllValue 用于存储 setAll 操作设置的全局值
int setAllValue;
// setAllTime 用于存储最后一次 setAll 操作的时间戳
int setAllTime;
// cnt 用于生成时间戳，每次操作后递增
int cnt;

// 向 map 中插入或更新键值对
void put(int k, int v) {
    // 如果键 k 已经存在，则更新其值和时间戳
    if (map.find(k) != map.end()) {
        map[k] = {v, cnt++};
    } else {
        // 如果键 k 不存在，则插入新的键值对和时间戳
        map[k] = {v, cnt++};
    }
}

// 设置所有键的值为 v，并更新 setAllTime
void setAll(int v) {
    setAllValue = v;
    setAllTime = cnt++;
}

// 获取键 k 的值
int get(int k) {
    // 如果键 k 不存在，返回 -1
    if (map.find(k) == map.end()) {
        return -1;
    }
    auto value = map[k];
    // 如果键 k 的时间戳大于 setAllTime，返回其实际值
    if (value.second > setAllTime) {
        return value.first;
    } else {
        // 否则返回 setAllValue
        return setAllValue;
    }
}

int main() {
    string line;
    // 从标准输入读取多行数据
    while (getline(cin, line)) {
        // 清空 map 和相关变量，准备处理新的一组操作
        map.clear();
        setAllValue = 0;
        setAllTime = -1;
        cnt = 0;
        istringstream iss(line);
        int n, op, a, b;
        // 读取操作数量
        iss >> n;
        for (int i = 0; i < n; i++) {
            // 读取操作类型
            iss >> op;
            if (op == 1) {
                // 读取键和值，执行 put 操作
                iss >> a >> b;
                put(a, b);
            } else if (op == 2) {
                // 读取键，执行 get 操作并输出结果
                iss >> a;
                cout << get(a) << endl;
            } else {
                // 读取值，执行 setAll 操作
                iss >> a;
                setAll(a);
            }
        }
    }
    return 0;
}
