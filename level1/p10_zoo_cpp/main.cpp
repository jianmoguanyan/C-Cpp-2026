#include <iostream>
#include <vector>
#include <string>

using namespace std;

//动物基类
class Animal {
public:
    //虚析构函数：极其重要！当使用基类指针 delete 派生类对象时，保证派生类析构函数被正确调用，防止内存泄漏
    virtual ~Animal() {}

    //纯虚函数：要求所有继承 Animal 的类都必须实现自己的叫声
    virtual void makeSound() const = 0;
};

//具体动物类

//狗类
class Dog : public Animal {
public:
    void makeSound() const override {
        cout << "狗叫：汪汪汪！" << endl;
    }
};

//猫类
class Cat : public Animal {
public:
    void makeSound() const override {
        cout << "猫叫：喵喵喵！" << endl;
    }
};

//要求二新增：鸟类
class Bird : public Animal {
public:
    void makeSound() const override {
        cout << "鸟鸣：叽叽喳喳！" << endl;
    }
};

//要求二新增：狼狗类
class WolfDog : public Animal {
public:
    void makeSound() const override {
        cout << "狼狗嚎叫：嗷呜——！" << endl;
    }
};

//动物园类
class Zoo {
private:
    //使用 vector 存储基类指针，利用多态实现容纳任意多个不同种类的动物
    vector<Animal*> m_animals;

public:
    //默认构造函数
    Zoo() = default;

    //析构函数：边界处理核心得分点！释放所有 new 出来的动物对象，防止内存泄漏
    ~Zoo() {
        for (Animal* animal : m_animals) {
            delete animal; //因为基类有虚析构函数，这里能安全释放派生类资源
        }
        m_animals.clear(); //清空容器
    }

    //禁用拷贝构造和赋值运算符：防止多个 Zoo 对象指向同一块内存导致重复释放（Double Free）崩溃
    Zoo(const Zoo&) = delete;
    Zoo& operator=(const Zoo&) = delete;

    //要求一.1：添加动物
    void addAnimal(Animal* animal) {
        //边界处理：防止传入空指针导致程序崩溃
        if (animal != nullptr) {
            m_animals.push_back(animal);
        }
        else {
            cout << "[警告] 无法添加空指针动物！" << endl;
        }
    }

    //要求一.3：让动物园内的动物依次吼叫一次
    void showAllSounds() const {
        //边界处理：动物园为空的情况
        if (m_animals.empty()) {
            cout << "动物园里目前没有动物。" << endl;
            return;
        }

        cout << "--- 动物园大合唱开始 ---" << endl;
        //遍历所有的动物指针，通过多态动态绑定调用各自重写的 makeSound
        for (Animal* animal : m_animals) {
            animal->makeSound();
        }
        cout << "------------------------" << endl;
    }
};

int main() {
    //1. 建立动物园
    Zoo myZoo;

    //2. 添加基础动物 (要求一)
    cout << "【正在放入基础动物...】" << endl;
    myZoo.addAnimal(new Dog());
    myZoo.addAnimal(new Cat());

    //边界测试：尝试添加空指针
    myZoo.addAnimal(nullptr);

    //3. 动物依次吼叫
    myZoo.showAllSounds();

    //4. 验证要求二（不修改 Zoo 代码，直接扩展新物种）
    cout << "\n【正在放入新物种...】" << endl;
    myZoo.addAnimal(new Bird());
    myZoo.addAnimal(new WolfDog());
    myZoo.addAnimal(new Dog()); // 再加一只普通狗

    //再次让所有动物吼叫
    myZoo.showAllSounds();

    system("pause");
    return 0;
}