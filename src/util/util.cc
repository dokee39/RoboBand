#include <stdexcept>
#include "util/util.hpp"

namespace robo {
namespace util {
void mergeConfig(toml::table& user_config, toml::table& default_config) {
    for (const auto& [key, value] : user_config) {
        if (auto* user_table = value.as_table()) {
            if (auto* default_table = default_config[key].as_table()) {
                mergeConfig(*default_table, *user_table);
            } else {
                default_config.insert_or_assign(key, value);
            }
        } else {
            default_config.insert_or_assign(key, value);
        }
    }
}

const toml::table &getTable(const toml::table &table, const std::string key) {
    auto new_table = table[key].as_table();
    if (new_table) {
        /*LOG(INFO) << "true key:" + key;*/
        return *new_table;
    } else {
        /*LOG(INFO) << "error key:" + key;*/
        throw std::runtime_error(R"(An error occurred when took out a subtable named ")" + key + R"(".)");
        return table;
    }
}
}
}
