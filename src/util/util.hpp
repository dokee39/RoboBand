#pragma once

#include <toml++/toml.hpp>
#include <easylogging++.h>

namespace robo {
namespace util {
void merge_config(toml::table& user_config, toml::table& default_config);
}
}
