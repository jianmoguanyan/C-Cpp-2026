#include <iostream>
#include <vector>
#include <string>

using namespace std;

//动物基类
class Animal {
public:
    // 纯虚函数：要求所有继承 Animal 的类都必须实现自己的叫声
    virtual void makeSound() const = 0;

    //具体要求一的动物
    class Dog : public Animal {
    public:
        void makeSound() const override {
            cout << "狗叫：汪汪汪！" << endl;
        }
    };

    class Cat : public Animal {
    public:
        void makeSound() const override {
            cout << "猫叫：喵喵喵！" << endl;
        }
    };

    //具体要求二新增的动物
    class Bird : public Animal {
    public:
        void makeSound() const override {
            cout << "鸟鸣：叽叽喳喳！" << endl;
        }
    };

    class WolfDog : public Animal {
    public:
        void makeSound() const override {
            cout << "狼狗嚎叫：嗷呜——！" << endl;
        }
    };

    //动物园类
    class Zoo {
    private:
        // 使用 vector 存储基类指针，实现容纳任意多个
        vector<Animal*> m_animals;

    public:
        // 添加动物
        void addAnimal(Animal* animal) {
            if (animal != nullptr) {
                m_animals.push_back(animal);
            }
        }

        // 要求一.3：让动物园内的动物依次吼叫一次
        void showAllSounds() const {
            if (m_animals.empty()) {
                cout << "动物园里目前没有动物。" << endl;
                return;
            }

            cout << "动物园大合唱开始" << endl;
            // 遍历所有的动物指针，通过多态调用各自重写的 makeSound
            for (Animal* animal : m_animals) {
                animal->makeSound();
            }
            cout << "------------------------" << endl;
        }
    };

    int main() {
        // 1. 建立动物园
        Zoo myZoo;

        // 2. 添加基础动物 (要求一)
        cout << "【正在放入基础动物...】" << endl;
        // 注意：这里使用了 new 创建在堆区，多态必须用指针
        myZoo.addAnimal(new Dog());
        myZoo.addAnimal(new Cat());

        // 3. 动物依次吼叫
        myZoo.showAllSounds();

        //要求二验证
        cout << "\n【正在放入新物种...】" << endl;
        myZoo.addAnimal(new Bird());
        myZoo.addAnimal(new WolfDog());
        myZoo.addAnimal(new Dog()); // 再加一只普通狗

        // 再次让所有动物吼叫
        myZoo.showAllSounds();

        system("pause"); // VS2022 暂停控制台
        return 0;
    }

    //VS C环境