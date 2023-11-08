// #include "ConfigFile/ConfigFile.hpp"
#include "ConfigFile/ConfigFile.hpp"
#include <exception>
#include <iostream>
#include <vector>

int main() {
  Config a;
  std::vector<Server> b = a.getConfig();
}