#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
using namespace std;

// 递归函数f1，生成字符串的所有子序列并去重
// s: 输入字符串
// i: 当前处理的字符索引
// path: 当前构建的子序列
// set: 用于存储所有不重复的子序列
void f1(const string& s, int i, string& path, unordered_set<string>& set) {
    // 如果已经处理完所有字符，将当前路径加入集合
    if (i == s.length()) {
        set.insert(path);
    } else {
        // 选择当前字符
        path.push_back(s[i]);
        f1(s, i + 1, path, set);
        path.pop_back(); // 回溯，移除当前字符

        // 不选择当前字符
        f1(s, i + 1, path, set);
    }
}

// 递归函数f2，生成字符串的所有子序列并去重
// s: 输入字符串
// i: 当前处理的字符索引
// path: 当前构建的子序列（预分配内存）
// size: 当前路径的实际长度
// set: 用于存储所有不重复的子序列
void f2(const string& s, int i, string& path, int size, unordered_set<string>& set) {
    // 如果已经处理完所有字符，将当前路径加入集合
    if (i == s.length()) {
        set.insert(path.substr(0, size));
    } else {
        // 选择当前字符
        if (size < path.length()) {
            path[size] = s[i];
        } else {
            path.push_back(s[i]);
        }
        f2(s, i + 1, path, size + 1, set);

        // 不选择当前字符
        f2(s, i + 1, path, size, set);
    }
}

// 生成字符串的所有不重复子序列（方法1）
// str: 输入字符串
vector<string> generatePermutation1(string str) {
    unordered_set<string> set;
    string path;
    f1(str, 0, path, set);
    // 将集合转换为向量并返回
    return vector<string>(set.begin(), set.end());
}

// 生成字符串的所有不重复子序列（方法2）
// str: 输入字符串
vector<string> generatePermutation2(string str) {
    unordered_set<string> set;
    // 预分配内存空间
    string path(str.length(), '\0');
    f2(str, 0, path, 0, set);
    // 将集合转换为向量并返回
    return vector<string>(set.begin(), set.end());
}

int main() {
    string input;
    cout << "0: ";
    cin >> input;

    // 使用方法1生成子序列
    vector<string> result1 = generatePermutation1(input);
    cout << "1: " << endl;
    for (const string& subseq : result1) {
        cout << subseq << endl;
    }

    // 使用方法2生成子序列
    vector<string> result2 = generatePermutation2(input);
    cout << "\n2: " << endl;
    for (const string& subseq : result2) {
        cout << subseq << endl;
    }

    return 0;
}