#pragma once

#include <string>
#include <array>
#include <type_traits>
#include <typeinfo>
#include <cxxabi.h>
#include <toml++/impl/forward_declarations.hpp>
#include <toml++/toml.hpp>
#include <easylogging++.h>

namespace robo {
namespace util {
void mergeConfig(toml::table& user_config, toml::table& default_config);

const toml::table &getTable(const toml::table &table, const std::string &key);

template <typename T>
T getValue(const toml::table &table, const std::string &key) {
    auto value = table[key].value<T>();
    if (!!value) {
        LOG(INFO) << "True key: " + key;
        return value.value();
    } else {
        LOG(ERROR) << R"(An error occurred when got a value named ")" + key + R"(".)";
        throw std::runtime_error("Wrong configuration file.");
        return *new T;
    }
}

template <typename T, std::size_t n>
std::array<T, n> getArray(const toml::table &table, const std::string &key) {
    std::array<T, n> arr;
    auto tarr = table[key].as_array();

    if (!tarr) {
        LOG(ERROR) << R"(An error occurred when got an array named ")" + key + R"(".)";
        throw std::runtime_error("Wrong configuration file.");
        return arr;
    }

    if (auto tarr_size = tarr->size(); tarr_size!= n) {
        LOG(ERROR) << R"(Array named ")" + key + R"(" required )" << n << " elements, but you gave " << tarr_size << ".";
        throw std::runtime_error("Wrong configuration file.");
        return arr;
    }

    int i = 0;
    for (auto &&elem: *tarr) {
        int status;
        bool is_vaild = elem.visit([&arr, i](auto &&el) noexcept -> bool {
            if constexpr (toml::is_number<decltype(el)> && (std::is_same<T, int>() || std::is_same<T, float>())) {
                arr[i] = static_cast<T>(*el);
            } else if constexpr (toml::is_string<decltype(el)> && (std::is_same<T, std::string>())) {
                arr[i] = static_cast<T>(*el);
            } else if constexpr (toml::is_boolean<decltype(el)> && (std::is_same<T, bool>())) {
                arr[i] = static_cast<T>(*el);
            } else {
                return false;
            }
            return true;
        });

        if (!is_vaild) {
            LOG(ERROR) << R"(Array named ")" + key + R"(" required ")"
                       << abi::__cxa_demangle(typeid(T).name(), 0, 0, &status)
                       << R"(" type data, but you gave ")"
                       << abi::__cxa_demangle(typeid(elem).name(), 0, 0, &status)
                       << R"(".)";
            throw std::runtime_error("Wrong configuration file.");
            return arr;
        }
    
        i++;
    }

    LOG(INFO) << "True key: " + key;
    return arr;
}
}
}
