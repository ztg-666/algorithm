#include <bits/stdc++.h>
using namespace std;

// 快速读取整数的函数
inline int read() {
    int s = 0, w = 1; // s 用于存储读取的数字，w 用于处理负数
    char ch = getchar(); // 读取一个字符
    // 跳过所有非数字字符，处理负号
    while(ch < '0' || ch > '9') {
        if(ch == '-') w *= -1; // 如果读取到负号，将 w 置为 -1
        ch = getchar(); // 继续读取下一个字符
    }
    // 读取数字字符并转换为整数
    while(ch >= '0' && ch <= '9') {
        s = (s << 3) + (s << 1) + (ch ^ 48); // 等价于 s = s * 10 + (ch - '0')
        ch = getchar(); // 继续读取下一个字符
    }
    return s * w; // 返回读取到的整数，考虑正负号
}