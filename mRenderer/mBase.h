#pragma once
#include <cstdio>
#include <cstdlib>
#include <type_traits>

// DEBUG
#include <iostream>
using std::cout;
//

using uchar     = unsigned char;
using uint      = unsigned int;
using rawColor  = uint;             // 4字节

//#define mNDebug

#ifndef mNDebug
#define mAssert(exp)    do { if(!(exp)) {fprintf(stderr, " - AF: %s; %s:%d\n", #exp, __FILE__, __LINE__); }} while(0)
#define mError(info)    do { fprintf(stderr, " - %s:%d  Info:%s\n", __FILE__, __LINE__, info); abort();} while(0)
#else
#define mAssert(exp)    (void)(0)
#define mError(info)    (void)(0)
#endif

#define UNUSED_VAR(x)   ((void)(x))
#define LINE_SIZE       1024

//template<bool B, class T = void>
//struct enable_if {};
//template<class T>
//struct enable_if<true, T> { typedef T type; };

// 常用类型判断 (SFINE
//  _ 用于函数的声明和实现分离的情况
#define _If_Type(...)   typename std::enable_if<(bool)(__VA_ARGS__), int>::type
#define If_Type(...)    _If_Type(__VA_ARGS__) = 0

template <typename T1, typename T2> 
constexpr bool is_same() { return std::is_same<T1, T2>::value; }

template <typename T> 
constexpr bool is_int() { return is_same<std::decay<T>::type, int>(); }

template <typename T> 
constexpr bool is_char() { return is_same<std::decay<T>::type, char>(); }

template <typename T> 
constexpr bool is_uchar() { return is_same<std::decay<T>::type, uchar>(); }

template <typename T> 
constexpr bool is_float() { return is_same<std::decay<T>::type, float>(); }

template <typename T> 
constexpr bool is_double() { return is_same<std::decay<T>::type, double>(); }

template <typename T> 
constexpr bool is_rawColor() { return is_same<std::decay<T>::type, rawColor>(); }




