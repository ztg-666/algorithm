#include <vector>
#include <string>
#include<iostream>
using namespace std;

const int MAXN = 301;
int father[MAXN];
int sets;

// 初始化并查集
void build(int n) {
    for (int i = 0; i < n; ++i) {
        father[i] = i; // 每个节点的父节点初始化为其自身
    }
    sets = n; // 初始时，每个节点都是一个独立的集合
}

// 查找节点 i 的根节点，并进行路径压缩
int find(int i) {
    if (father[i] != i) {
        father[i] = find(father[i]); // 路径压缩
    }
    return father[i]; // 返回根节点
}

// 合并两个集合
void unionSet(int x, int y) {
    int fx = find(x); // 找到 x 的根节点
    int fy = find(y); // 找到 y 的根节点
    if (fx != fy) {
        father[fx] = fy; // 合并两个集合
        sets--;          // 连通分量数量减一
    }
}

// 计算相似字符串组的数量
int numSimilarGroups(vector<string>& strs) {
    int n = strs.size(); // 字符串的数量
    int m = strs[0].size(); // 每个字符串的长度
    build(n); // 初始化并查集

    // 遍历每一对字符串
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (find(i) != find(j)) { // 如果两个字符串不在同一个集合中
                int diff = 0; // 记录字符差异的数量
                // 比较两个字符串的每个字符
                for (int k = 0; k < m && diff < 3; ++k) {
                    if (strs[i][k] != strs[j][k]) {
                        ++diff; // 如果字符不同，差异计数加一
                    }
                }
                // 如果字符差异为 0 或 2，则两个字符串相似
                if (diff == 0 || diff == 2) {
                    unionSet(i, j); // 合并这两个字符串所在的集合
                }
            }
        }
    }
    return sets; // 返回连通分量的数量，即相似字符串组的数量
}
// 相似字符串组
// 如果交换字符串 X 中的两个不同位置的字母，使得它和字符串 Y 相等
// 那么称 X 和 Y 两个字符串相似
// 如果这两个字符串本身是相等的，那它们也是相似的
// 例如，"tars" 和 "rats" 是相似的 (交换 0 与 2 的位置)；
// "rats" 和 "arts" 也是相似的，但是 "star" 不与 "tars"，"rats"，或 "arts" 相似
// 总之，它们通过相似性形成了两个关联组：{"tars", "rats", "arts"} 和 {"star"}
// 注意，"tars" 和 "arts" 是在同一组中，即使它们并不相似
// 形式上，对每个组而言，要确定一个单词在组中，只需要这个词和该组中至少一个单词相似。
// 给你一个字符串列表 strs列表中的每个字符串都是 strs 中其它所有字符串的一个字母异位词。
// 返回 strs 中有多少字符串组
// 测试链接 : https://leetcode.cn/problems/similar-string-groups/
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    vector<string> test1 = {"tars","rats","arts","star"};
    cout << numSimilarGroups(test1) << endl; // 输出 2

    vector<string> test2 = {"omv","ovm"};
    cout << numSimilarGroups(test2) << endl; // 输出 1

    return 0;
}