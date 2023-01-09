/**
 * @file test.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2023-01-06
 */

#include <memory>

#include "serenity.h"

int main() {
    auto app = std::make_unique<serenity::Serenity>();
    app->Loop();
    return 0;
}