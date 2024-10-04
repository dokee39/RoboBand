#pragma once

#include <string>
#include <toml++/toml.hpp>
#include <easylogging++.h>

namespace robo {
namespace util {
void mergeConfig(toml::table& user_config, toml::table& default_config);

const toml::table &getTable(const toml::table &table, const std::string key);

template <typename T>
T getValue(const toml::table &table, const std::string key) {
    auto value = table[key].value<T>();
    if (value) {
        /*LOG(INFO) << "true key:" + key;*/
        return value.value();
    } else {
        /*LOG(INFO) << "error key:" + key;*/
        throw std::runtime_error(R"(An error occurred when got a value named ")" + key + R"(".)");
        return *new T;
    }
}
}
}
