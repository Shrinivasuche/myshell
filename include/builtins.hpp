#ifndef BUILTINS_HPP
#define BUILTINS_HPP

#include <vector>
#include <string>

void builtin_cd(const std::vector<std::string>& tokens);
void builtin_exit();
void builtin_help();

#endif
