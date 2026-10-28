#include <iostream>
#include <string>
#include <cstdlib> // 用于 exit()

using namespace std;

// 要求三：实现类型无关的安全数组（使用模板）
// 这里直接一步到位，包含了要求一和二的功能
template <typename T>
class SafeArray {
private:
    T* m_data;      // 指向动态分配的数组
    int m_size;     // 数组的大小

public:
    // 构造函数
    SafeArray(int size) {
        if (size <= 0) {
            cerr << "初始化错误：数组大小必须大于0" << endl;
            exit(1); // 暴力退出
        }
        m_size = size;
        m_data = new T[m_size];
    }

    // 析构函数：释放内存
    ~SafeArray() {
        delete[] m_data;
    }

    // 要求一：实现基本功能，当数组下标越界时报错
    T get(int index) const {
        if (index < 0 || index >= m_size) {
            cerr << "错误：数组下标越界！(get方法)" << endl;
            exit(1);
        }
        return m_data[index];
    }

    void set(int index, T value) {
        if (index < 0 || index >= m_size) {
            cerr << "错误：数组下标越界！(set方法)" << endl;
            exit(1);
        }
        m_data[index] = value;
    }

    // 要求二：实现下标运算操作
    T& operator[](int index) {
        if (index < 0 || index >= m_size) {
            cerr << "错误：数组下标越界！(operator[]运算)" << endl;
            exit(1);
        }
        return m_data[index];
    }

    // 辅助测试方法：打印整个数组
    void print() const {
        for (int i = 0; i < m_size; ++i) {
            cout << m_data[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    // 1. 测试 int 类型（要求一、二）
    cout << "--- 测试 int 数组 ---" << endl;
    SafeArray<int> intArr(5);
    for (int i = 0; i < 5; ++i) {
        intArr[i] = (i + 1) * 10; // 测试 operator[]
    }
    intArr.print();
    cout << "获取索引2的值: " << intArr.get(2) << endl; // 测试 get

    // 2. 测试 string 类型（要求三）
    cout << "\n--- 测试 string 数组 ---" << endl;
    SafeArray<string> strArr(3);
    strArr[0] = "Hello";
    strArr[1] = "C++";
    strArr[2] = "VS2022";
    strArr.print();

    system("pause");
    return 0;
}

//VS C环境