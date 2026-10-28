#include <iostream>
#include <stdexcept> // 用于抛出标准异常 std::out_of_range
#include <string>

// 模板类定义，满足功能要求（三）：实现类型无关的安全数组
template <typename T>
class SafeArray {
private:
    T* m_pData;     // 指向动态分配的数组内存
    int m_nSize;    // 数组的大小

public:
    // 构造函数：初始化数组大小并分配内存
    SafeArray(int size = 10) {
        if (size <= 0) {
            // 边界处理：如果传入非法的数组大小，抛出异常
            throw std::invalid_argument("数组大小必须大于0！");
        }
        m_nSize = size;
        m_pData = new T[m_nSize](); // 值初始化，基本类型置0，类类型调用默认构造
    }

    // 析构函数：释放动态分配的内存，防止内存泄漏
    ~SafeArray() {
        if (m_pData != nullptr) {
            delete[] m_pData;
            m_pData = nullptr;
        }
    }

    // 拷贝构造函数：实现深拷贝（防止浅拷贝导致的重复释放崩溃）
    SafeArray(const SafeArray& other) {
        m_nSize = other.m_nSize;
        m_pData = new T[m_nSize];
        for (int i = 0; i < m_nSize; ++i) {
            m_pData[i] = other.m_pData[i];
        }
    }

    // 赋值运算符重载：实现深拷贝
    SafeArray& operator=(const SafeArray& other) {
        if (this != &other) { // 防止自赋值
            delete[] m_pData; // 先释放原有内存
            m_nSize = other.m_nSize;
            m_pData = new T[m_nSize];
            for (int i = 0; i < m_nSize; ++i) {
                m_pData[i] = other.m_pData[i];
            }
        }
        return *this;
    }

    // 功能要求（二）：实现下标运算操作（非const版本，支持读写）
    T& operator[](int index) {
        // 边界处理：功能要求（一）当数组下标越界时报错
        if (index < 0 || index >= m_nSize) {
            throw std::out_of_range("数组下标越界！越界索引: " + std::to_string(index));
        }
        return m_pData[index];
    }

    // 功能要求（二）：实现下标运算操作（const版本，仅支持只读，用于const对象）
    const T& operator[](int index) const {
        // 边界处理：当数组下标越界时报错
        if (index < 0 || index >= m_nSize) {
            throw std::out_of_range("数组下标越界！越界索引: " + std::to_string(index));
        }
        return m_pData[index];
    }

    // 辅助功能：获取数组大小
    int getSize() const {
        return m_nSize;
    }
};

// 主函数：用于测试代码功能
int main() {
    std::cout << "========== 测试 SafeArray (int 类型) ==========" << std::endl;

    // 测试功能要求（一）：int型数组基本功能
    SafeArray<int> intArray(5);
    for (int i = 0; i < intArray.getSize(); ++i) {
        intArray[i] = (i + 1) * 10; // 利用重载的[]赋值
    }

    // 输出正常数据
    std::cout << "正常读取数组元素: ";
    for (int i = 0; i < intArray.getSize(); ++i) {
        std::cout << intArray[i] << " ";
    }
    std::cout << std::endl;

    // 测试功能要求（一）：越界报错处理
    std::cout << "\n开始测试边界处理（越界访问）：" << std::endl;
    try {
        // 尝试访问索引为 5 的元素 (有效索引为 0-4)
        int value = intArray[5];
        std::cout << "意外成功读取了越界元素: " << value << std::endl;
    }
    catch (const std::out_of_range& e) {
        // 捕获越界异常
        std::cout << "捕获到异常: " << e.what() << std::endl;
    }

    // 测试负数索引越界
    try {
        int value = intArray[-1];
    }
    catch (const std::out_of_range& e) {
        std::cout << "捕获到异常: " << e.what() << std::endl;
    }

    std::cout << "\n========== 测试 SafeArray (string 类型) ==========" << std::endl;
    // 测试功能要求（三）：类型无关的安全数组
    SafeArray<std::string> strArray(3);
    strArray[0] = "Hello";
    strArray[1] = "VS2022";
    strArray[2] = "C++";

    std::cout << "字符串数组元素: ";
    for (int i = 0; i < strArray.getSize(); ++i) {
        std::cout << strArray[i] << " ";
    }
    std::cout << std::endl;

    // 测试深拷贝
    SafeArray<int> copyArray = intArray;
    copyArray[0] = 999; // 修改拷贝后的数组
    std::cout << "\n深拷贝测试 - 原数组首个元素: " << intArray[0] << "，拷贝数组首个元素: " << copyArray[0] << std::endl;

    system("pause"); // VS2022 环境下防止控制台闪退
    return 0;
}