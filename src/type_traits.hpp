#pragma once
#include <functional>

namespace sref
{
    template <typename T>
    struct remove_reference
    {
    };

    template <typename T>
    struct remove_reference<T &>
    {
        using type = T;
    };

    template <typename T>
    struct remove_reference<T &&>
    {
        using type = T;
    };

    template <typename T>
    struct remove_reference<const T &>
    {
        using type = const T;
    };

    template <typename T>
    struct remove_reference<const T &&>
    {
        using type = const T;
    };

    template <typename T>
    using remove_reference_t = typename remove_reference<T>::type;

    template <typename T>
    struct remove_const
    {
    };

    template <typename T>
    struct remove_const<const T>
    {
        using type = T;
    };

    template <typename T>
    struct remove_const<const T &>
    {
        using type = T &;
    };

    template <typename T>
    struct remove_const<const T &&>
    {
        using type = T &&;
    };

    template <typename T>
    struct remove_const<const T *>
    {
        using type = T *;
    };

    template <typename T>
    using remove_const_t = typename remove_const<T>::type;

    template <typename T>
    struct remove_pointer
    {
    };

    template <typename T>
    struct remove_pointer<T *>
    {
        using type = T;
    };

    template <typename T>
    struct remove_pointer<const T *>
    {
        using type = const T;
    };

    struct true_type
    {
        static constexpr bool value = true;
    };

    struct false_type
    {
        static constexpr bool value = false;
    };

    template <typename T>
    using remove_pointer_t = typename remove_pointer<T>::type;

    template <typename T>
    struct is_function_pointer : public false_type
    {
    };

    template <typename R, typename... Args>
    struct is_function_pointer<R (*)(Args...)> : public true_type
    {
    };

    template <typename R, typename... Args>
    struct is_function_pointer<std::function<R(Args...)>> : public true_type
    {
    };

    template <typename R, typename... Args>
    struct is_function_pointer<R(Args...)> : public false_type
    {
    };

    template <typename T>
    static constexpr bool is_function_pointer_v = is_function_pointer<T>::value;

    template <typename T>
    struct is_function : public false_type
    {
    };

    template <typename R, typename... Args>
    struct is_function<R(Args...)> : public true_type
    {
    };

    template <typename T>
    static constexpr bool is_function_v = is_function<T>::value;

    template <typename T>
    struct is_member_function_pointer
    {
        static constexpr bool value = false;
    };

    template <typename R, typename C, typename... Args>
    struct is_member_function_pointer<R (C::*)(Args...)>
    {
        static constexpr bool value = true;
    };

    template <typename R, typename C, typename... Args>
    struct is_member_function_pointer<R (C::*)(Args...) const>
    {
        static constexpr bool value = true;
    };

    template <typename T>
    static constexpr bool is_member_function_pointer_v = is_member_function_pointer<T>::value;

    template <typename T>
    struct is_const_member_function_pointer
    {
        static constexpr bool value = false;
    };

    template <typename R, typename C, typename... Args>
    struct is_const_member_function_pointer<R (C::*)(Args...) const>
    {
        static constexpr bool value = true;
    };

    template <typename T>
    static constexpr bool is_const_member_function_pointer_v = is_const_member_function_pointer<T>::value;

    template <typename T>
    struct is_function_object
    {
        static constexpr bool value = !is_function_pointer_v<T> && !is_member_function_pointer_v<T>;
    };

    template <typename T>
    static constexpr bool is_function_object_v = is_function_object<T>::value;

    template <typename T>
    struct function_traits_base
    {
        static constexpr bool is_function_pointer = is_function_pointer_v<T>;
        static constexpr bool is_member_function_pointer = is_member_function_pointer_v<T>;
        static constexpr bool is_const_member_function_pointer = is_const_member_function_pointer_v<T>;
    };

    template <typename T>
    struct function_traits : public function_traits_base<T>
    {
    };

    template <typename R, typename... Args>
    struct function_traits<R (*)(Args...)> : public function_traits_base<R (*)(Args...)>
    {
        using return_type = R;
        static constexpr std::size_t arity = sizeof...(Args);
        template <std::size_t N>
        struct argument
        {
            static_assert(N < arity, "error: invalid parameter index.");
            using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
        };
    };

    template <typename R, typename C, typename... Args>
    struct function_traits<R (C::*)(Args...)> : public function_traits_base<R (C::*)(Args...)>
    {
        using return_type = R;
        using class_type = C;
        static constexpr std::size_t arity = sizeof...(Args);

        template <std::size_t N>
        struct argument
        {
            static_assert(N < arity, "error: invalid parameter index.");
            using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
        };
    };

    template <typename R, typename C, typename... Args>
    struct function_traits<R (C::*)(Args...) const> : public function_traits_base<R (C::*)(Args...) const>
    {
        using return_type = R;
        using class_type = C;
        static constexpr std::size_t arity = sizeof...(Args);

        template <std::size_t N>
        struct argument
        {
            static_assert(N < arity, "error: invalid parameter index.");
            using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
        };
    };

    template <typename R, typename... Args>
    struct function_traits<std::function<R(Args...)>> : public function_traits_base<std::function<R(Args...)>>
    {
        using return_type = R;
        static constexpr std::size_t arity = sizeof...(Args);

        template <std::size_t N>
        struct argument
        {
            static_assert(N < arity, "error: invalid parameter index.");
            using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
        };
    };

    template <typename R, typename... Args>
    auto function_pointer_type(R (*)(Args...)) -> R (*)(Args...);

    template <typename R, typename C, typename... Args>
    auto function_pointer_type(R (C::*)(Args...)) -> R (C::*)(Args...);

    template <typename R, typename C, typename... Args>
    auto function_pointer_type(R (C::*)(Args...) const) -> R (C::*)(Args...) const;

    template <typename R, typename... Args>
    auto function_pointer_type(std::function<R(Args...)>) -> std::function<R(Args...)>;

    template <auto T>
    using function_pointer_type_t = decltype(function_pointer_type(T));

    template <auto T>
    using function_traits_t = function_traits<function_pointer_type_t<T>>;
}
