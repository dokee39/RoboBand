#include "util/util.hpp"

namespace robo {
namespace util {
void merge_config(toml::table& user_config, toml::table& default_config) {
    for (const auto& [key, value] : user_config) {
        if (auto* user_table = value.as_table()) {
            if (auto* default_table = default_config[key].as_table()) {
                merge_config(*default_table, *user_table);
            } else {
                default_config.insert_or_assign(key, value);
            }
        } else {
            default_config.insert_or_assign(key, value);
        }
    }
}

}
}
