#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

int largestRectangleArea(vector<int>& heights) {
    if (heights.empty()) return 0;
    
    vector<int> extendedHeights;
    extendedHeights.push_back(0);
    extendedHeights.insert(extendedHeights.end(), heights.begin(), heights.end());
    extendedHeights.push_back(0);
    
    stack<int> st;  
    int maxArea = 0;
    int n = extendedHeights.size();
    
    for (int i = 0; i < n; i++) {
        while (!st.empty() && extendedHeights[i] < extendedHeights[st.top()]) {
            int height = extendedHeights[st.top()];
            st.pop();
            
            int width;
            if (st.empty()) {
                width = i;
            } else {
                width = i - st.top() - 1;
            }
            
            maxArea = max(maxArea, height * width);
        }
        st.push(i);
    }
    
    return maxArea;
}

void testExamples() {

    vector<int> heights1 = {2, 1, 5, 6, 2, 3};
    cout << "输入: [2,1,5,6,2,3], 输出: " << largestRectangleArea(heights1) << endl;  // 应该输出10
    

    vector<int> heights2 = {2, 4};
    cout << "输入: [2,4], 输出: " << largestRectangleArea(heights2) << endl;  // 应该输出4
}

// 生成随机测试用例
vector<vector<int>> generateTestCases(int numCases = 10) {
    vector<vector<int>> testCases;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> lengthDist(1, 20);  // 数组长度
    uniform_int_distribution<> heightDist(0, 100); // 柱子高度
    
    for (int i = 0; i < numCases; i++) {
        int length = lengthDist(gen);
        vector<int> heights;
        for (int j = 0; j < length; j++) {
            heights.push_back(heightDist(gen));
        }
        testCases.push_back(heights);
    }
    
    return testCases;
}

// 运行测试
void runTests() {
    auto testCases = generateTestCases(10);
    
    cout << "==================================================" << endl;
    cout << "柱状图最大矩形面积测试结果" << endl;
    cout << "==================================================" << endl;
    
    for (int i = 0; i < testCases.size(); i++) {
        auto start = chrono::high_resolution_clock::now();
        int result = largestRectangleArea(testCases[i]);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        
        cout << "测试用例 " << i + 1 << ":" << endl;
        cout << "  输入数组: [";
        for (int j = 0; j < testCases[i].size(); j++) {
            cout << testCases[i][j];
            if (j < testCases[i].size() - 1) cout << ", ";
        }
        cout << "]" << endl;
        cout << "  数组长度: " << testCases[i].size() << endl;
        cout << "  最大面积: " << result << endl;
        cout << "  计算时间: " << duration.count() << " 微秒" << endl;
        cout << "----------------------------------------------" << endl;
    }
}

void testEdgeCases() {
    cout << "\n边界情况测试:" << endl;
    
    vector<int> empty;
    cout << "空数组: " << largestRectangleArea(empty) << endl;

    vector<int> single = {5};
    cout << "单个元素[5]: " << largestRectangleArea(single) << endl;

    vector<int> same = {3, 3, 3, 3};
    cout << "全部相同[3,3,3,3]: " << largestRectangleArea(same) << endl;
    
    vector<int> increasing = {1, 2, 3, 4, 5};
    cout << "递增序列[1,2,3,4,5]: " << largestRectangleArea(increasing) << endl;
    
    vector<int> decreasing = {5, 4, 3, 2, 1};
    cout << "递减序列[5,4,3,2,1]: " << largestRectangleArea(decreasing) << endl;
}

int main() {
    testExamples();
    runTests();
    testEdgeCases();
    return 0;
}