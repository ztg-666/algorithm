#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

// 定义数组的最大长度
const int MAXN = 100001;

// 定义数组来存储输入的元素
int arr[MAXN];
// 定义变量n来存储数组的长度
int n;

// 定义一个哈希表来存储前缀和及其对应的索引
unordered_map<int, int> map;

// 返回无序数组中正数和负数个数相等的最长子数组长度
// 给定一个无序数组arr，其中元素可正、可负、可0
// 求arr所有子数组中正数与负数个数相等的最长子数组的长度
// 计算正数和负数个数相等的最长子数组长度
int compute() {
    // 清空哈希表
    map.clear();
    // 初始化哈希表，前缀和为0时对应的索引为-1
    map[0] = -1;
    // 初始化答案为0
    int ans = 0;
    // 初始化前缀和为0
    for (int i = 0, sum = 0; i < n; i++) {
        // 更新前缀和
        sum += arr[i];
        // 如果当前前缀和已经存在于哈希表中
        if (map.find(sum) != map.end()) {
            // 更新答案为当前索引与哈希表中存储的索引之差的最大值
            ans = max(ans, i - map[sum]);
        } else {
            // 如果当前前缀和不存在于哈希表中，则将其存入哈希表
            map[sum] = i;
        }
    }
    // 返回结果
    return ans;
}

int main() {
    // 关闭同步以提高输入输出效率
    ios::sync_with_stdio(false);
    cin.tie(0);
    // 循环读取输入
    while (cin >> n) {
        // 读取数组元素
        for (int i = 0, num; i < n; i++) {
            cin >> num;
            // 将正数转换为1，负数转换为-1，0保持不变
            arr[i] = num != 0 ? (num > 0 ? 1 : -1) : 0;
        }
        // 计算并输出结果
        cout << compute() << endl;
    }
    // 返回0表示程序正常结束
    return 0;
}
