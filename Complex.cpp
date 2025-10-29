#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <cmath>
#include <chrono>
#include <string>
using namespace std;

class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    double getReal() const { return real; }
    double getImag() const { return imag; }
    double getModulus() const { return sqrt(real * real + imag * imag); }

    void setReal(double r) { real = r; }
    void setImag(double i) { imag = i; }

    friend ostream& operator<<(ostream& os, const Complex& c) {
        os << "(" << c.real;
        if (c.imag >= 0)
            os << "+" << c.imag << "i)";
        else
            os << c.imag << "i)";
        return os;
    }

    bool operator<(const Complex& other) const {
        double mod1 = this->getModulus();
        double mod2 = other.getModulus();
        if (std::abs(mod1 - mod2) > 1e-6)
            return mod1 < mod2;
        if (std::abs(real - other.real) > 1e-6)
            return real < other.real;
        return imag < other.imag;
    }

    bool operator==(const Complex& other) const {
        return std::abs(real - other.real) < 1e-6 && 
               std::abs(imag - other.imag) < 1e-6;
    }

    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }
};

// 将向量操作函数改为全局函数

// 唯一化：去除重复元素
void uniqueVector(std::vector<Complex>& vec) {
    auto it = std::unique(vec.begin(), vec.end());
    vec.erase(it, vec.end());
}

// 查找元素
int findComplex(const std::vector<Complex>& vec, const Complex& target) {
    auto it = std::find(vec.begin(), vec.end(), target);
    if (it != vec.end())
        return std::distance(vec.begin(), it);
    return -1;
}

// 冒泡排序
void bubbleSort(std::vector<Complex>& vec) {
    int n = vec.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (vec[j + 1] < vec[j]) {
                std::swap(vec[j], vec[j + 1]);
            }
        }
    }
}

// 归并排序的合并函数
void merge(std::vector<Complex>& vec, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<Complex> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = vec[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = vec[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] < R[j]) {
            vec[k++] = L[i++];
        } else {
            vec[k++] = R[j++];
        }
    }

    while (i < n1) {
        vec[k++] = L[i++];
    }
    while (j < n2) {
        vec[k++] = R[j++];
    }
}

// 归并排序
void mergeSort(std::vector<Complex>& vec, int left, int right) {
    if (left >= right) return;
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(vec, left, mid);
        mergeSort(vec, mid + 1, right);
        merge(vec, left, mid, right);
    }
}

void mergeSort(std::vector<Complex>& vec) {
    if (vec.empty()) return;
    mergeSort(vec, 0, vec.size() - 1);
}

// 区间查找
std::vector<Complex> rangeSearch(const std::vector<Complex>& vec, double m1, double m2) {
    std::vector<Complex> result;
    for (const auto& c : vec) {
        double mod = c.getModulus();
        if (mod >= m1 && mod <= m2) {
            result.push_back(c);
        }
    }
    return result;
}

void printVector(const std::vector<Complex>& vec, const std::string& title = "") {
    if (!title.empty()) {
        std::cout << title << ":" << std::endl;
    }
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
        if ((i + 1) % 5 == 0 && i != vec.size() - 1)
            std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-10.0, 10.0);

    std::vector<Complex> complexVector;
    int numComplex = 15;

    std::cout << "生成 " << numComplex << " 随机复数..." << std::endl;

    for (int i = 0; i < numComplex; ++i) {
        double real = dis(gen);
        double imag = dis(gen);
        complexVector.push_back(Complex(real, imag));

        if (i % 3 == 0 && i > 0) {
            complexVector.push_back(complexVector[i - 1]);
        }
    }

    printVector(complexVector, "原始随机复数");

    std::cout << "(1) 基础操作" << std::endl;

    // 置乱
    std::shuffle(complexVector.begin(), complexVector.end(), gen);
    printVector(complexVector, "打乱后的复数");

    // 查找
    if (!complexVector.empty()) {
        Complex target = complexVector[complexVector.size() / 2];
        int index = findComplex(complexVector, target); 
        std::cout << "查找 " << target << ": Index = " << index << std::endl << std::endl;
    }

    // 插入
    Complex newComplex(dis(gen), dis(gen));
    complexVector.insert(complexVector.begin() + 3, newComplex);
    std::cout << "插入后 " << newComplex << " at index 3" << std::endl;
    printVector(complexVector);

    // 删除
    if (complexVector.size() > 5) {
        complexVector.erase(complexVector.begin() + 2);
        printVector(complexVector, "删除后元素");
    }

    // 唯一化
    uniqueVector(complexVector);
    printVector(complexVector, "唯一化后元素");

    return 0;
}