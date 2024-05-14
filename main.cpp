#include <iostream>
#include "src/sref.hpp"
#include <any>

void func(int &i, std::string name)
{
    std::cout << "lvalue" << std::endl;
}

class A{
public:
    void func(int &i, std::string name)
    {
        std::cout << "lvalue" << std::endl;
    }
};

int main()
{
    sref::remove_reference_t<int&> i = 0;
    using type2 = sref::remove_const<const int&>::type;
    using type3 = sref::remove_pointer<const int*>::type;
    using type3 = sref::remove_reference<const int&>::type;

    auto func2 = func;
    std::any a = 1;
    using type4 = sref::function_traits<decltype(&A::func)>::return_type;
    using arg = sref::function_traits<decltype(&A::func)>::argument<0>::type;
    using arg2 = sref::function_traits<decltype(&A::func)>::argument<1>::type;
    constexpr bool arg3 = sref::function_traits<decltype(&A::func)>::is_const_member_function_pointer;
    using type5 = sref::function_pointer_type_t<&A::func>;
    using type = sref::function_traits_t<&A::func>;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}