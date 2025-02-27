#pragma once
#include "core/common.h"
#include <functional>
#include <memory>
#include <type_traits>

namespace infini {

template <typename T> using Ref = std::shared_ptr<T>;
template <typename T> using WRef = std::weak_ptr<T>;

template <typename T> struct is_ref : std::false_type {};
template <typename T> struct is_ref<Ref<T>> : std::true_type {};
template <typename T> struct is_ref<WRef<T>> : std::true_type {};

// 用任意数量的构造函数参数，并使用 std::make_shared 来构造对象
template <typename T, typename... Params> Ref<T> make_ref(Params &&...params) {
    static_assert(is_ref<T>::value == false, "Ref should not be nested");
    return std::make_shared<T>(std::forward<Params>(params)...);
}

// 用于将 Ref<U> 转化为 Ref<T>
template <class T, class U,
          typename std::enable_if_t<std::is_base_of_v<U, T>> * = nullptr>
Ref<T> as(const Ref<U> &ref) {
    return std::dynamic_pointer_cast<T>(ref);
}

// 将传入的 Ref数组 转化为 WRef 数组
template <typename T>
std::vector<WRef<T>> refs_to_wrefs(const std::vector<Ref<T>> &refs) {
    std::vector<WRef<T>> wrefs;
    for (const auto &ref : refs)
        wrefs.emplace_back(ref);
    return wrefs;
}

// 将传入的 WRef数组 转化为 Ref 数组
template <typename T>
std::vector<Ref<T>> wrefs_to_refs(const std::vector<WRef<T>> &wrefs) {
    std::vector<Ref<T>> refs;
    for (const auto &wref : wrefs)
        refs.emplace_back(wref);
    return refs;
}

} // namespace infini
