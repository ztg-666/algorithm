#include <bits/stdc++.h>
using namespace std;
// 功能：在一个数组中找到出现奇数次的数字，并打印出来
void printOddTimesNum1(vector<int>& arr) {
    int eO = 0; // 初始化一个变量用于存储所有元素的异或结果
    for (int cur : arr) {
        eO ^= cur; // 对数组中的每个元素进行异或操作
    }
    cout << eO << endl; // 打印最终的异或结果，即出现奇数次的数字
}

// 功能：在一个数组中找到两个出现奇数次的数字，并打印出来
void printOddTimesNum2(vector<int>& arr) {
    int eO = 0, eOhasOne = 0; // 初始化两个变量，eO用于存储所有元素的异或结果，eOhasOne用于存储其中一个奇数次出现的数字
    for (int curNum : arr) {
        eO ^= curNum; // 对数组中的每个元素进行异或操作
    }
    int rightOne = eO & (~eO + 1); // 找到eO中最低位的1，用于区分两个奇数次出现的数字
    for (int cur : arr) {
        if ((cur & rightOne) != 0) { // 根据最低位的1将数组分成两部分
            eOhasOne ^= cur; // 对其中一部分进行异或操作，得到其中一个奇数次出现的数字
        }
    }
    cout << eOhasOne << " " << (eO ^ eOhasOne) << endl; // 打印两个奇数次出现的数字
}
void swapexclusive(int& a, int& b) {
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
}


// 更通用的方法
// 已知数组中只有1种数出现次数少于m次，其他数都出现了m次
// 返回出现次数小于m次的那种数
static int find(const std::vector<int>& arr, int m) {
    // cnts[0] : 0位上有多少个1
    // cnts[i] : i位上有多少个1
    // cnts[31] : 31位上有多少个1
    int cnts[32] = { 0 };
    for (int num : arr) {
        for (int i = 0; i < 32; i++) {
            cnts[i] += (num >> i) & 1;
        }
    }
    int ans = 0;
    for (int i = 0; i < 32; i++) {
        if (cnts[i] % m != 0) {
            ans |= 1 << i;
        }
    }
    return ans;
}
static int singleNumber(const std::vector<int>& nums) {
    return find(nums, 3);
}
int main() {
    int a = 5;
    int b = 7;
    swapexclusive(a, b);

    cout << a << endl; // 打印交换后的a的值
    cout << b << endl; // 打印交换后的b的值

    vector<int> arr1 = { 3, 3, 2, 3, 1, 1, 1, 3, 1, 1, 1 };
    printOddTimesNum1(arr1); // 调用函数找到并打印数组arr1中出现奇数次的数字

    vector<int> arr2 = { 4, 3, 4, 2, 2, 2, 4, 1, 1, 1, 3, 3, 1, 1, 1, 4, 2, 2 };
    printOddTimesNum2(arr2); // 调用函数找到并打印数组arr2中两个出现奇数次的数字


    std::vector<int> testData = { 1, 1, 1, 2, 2, 2, 3 };

    // 调用find方法
    int result = find(testData, 3);

    // 输出结果
    std::cout << "The number that appears less than 3 times is: " << result << std::endl;

    return 0;
}
