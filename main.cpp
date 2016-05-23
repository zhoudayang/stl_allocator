#include<new>
#include<climits>
#include<iostream>
#include<vector>
/*
 * my first alloctor!
 */
using namespace std;
namespace zz {
    template<class T>
    /*
     *  构造器
     *  size 需要构造的元素个数
     *  T 需要构造的元素类型
     *  返回值 size 个 T* 数据块的开头对应的指针
     */
    inline T *_allocate(ptrdiff_t size, T *) {
        T *tmp = (T *) (::operator new((size_t) (size * sizeof(T))));
        if (tmp == 0) {
            std::cerr << "out of memory!" << std::endl;
            exit(1);
        }
        return tmp;
    }
    /*
     * 析构器,释放地址buffer对应的数据空间
     */
    template<class T>
    inline void _deallocate(T *buffer) {
        ::operator delete(buffer);
    }

    template<class T1, class T2>
    inline void _construct(T1 *p, const T2 &value) {
        //构造器,构造指针和值
        new(p) T1(value);//新建对象,使用value的值来初始化
    }
    //调用单个T类型元素的析构函数
    template<class T>
    inline void _destroy(T *ptr) {
        //调用析构函数
        ptr->~T();
    }

    template<class T>
    class allocator {
    public:
        typedef T value_type;
        typedef T *pointer;
        typedef const T *const_pointer;
        typedef T &reference;
        typedef const T &const_reference;
        //能够容纳最大地址空间
        typedef size_t size_type;
        //存储指针的间隔,可以为正也可以为负
        typedef ptrdiff_t difference_type;
        //再绑定
        template<class U>
        struct rebind {
            typedef allocator<U> other;
        };
        //配置空间,用来存储Tn个T对象

        pointer allocate(size_type n, const void *hint = 0) {
            return _allocate((difference_type) n, (pointer) 0);
        }

        //调用析构器
        //归还先前配置的空间
        void deallocate(pointer p, size_type n) {
            _deallocate(p);
        }
        //等价于 new((void*)p) T(value)
        void construct(pointer p, const T &value) {
            _construct(p, value);
        }

        void destroy(pointer p) {
            _destroy(p);
        }
        //返回对象的地址
        pointer address(reference x) {
            return (pointer) &x;
        }
        //返回对象的const地址
        const_pointer const_address(const_reference x) {
            return (const_pointer) &x;
        }
        //系统支持的存储T类型元素的最大个数
        size_type max_size() const {
            return size_type(UINT_MAX / sizeof(T));
        }
    };

}

int main() {
    int ia[5] = {1, 2, 3, 4, 5};
    //调用自定义的构造器
    std::vector<int, zz::allocator<int>> iv(ia, ia + 5);
    for (int i = 0; i < iv.size(); i++)
        std::cout << iv[i] << " ";
    std::cout << std::endl;
}