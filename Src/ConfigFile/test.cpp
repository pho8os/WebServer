
#include "ConfigFile.hpp"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

void trim(std::string &s) {
  while (s.begin() != s.end() && isspace(*s.begin()))
    s.erase(s.begin());
  while (s.end() != s.begin() && isspace(*s.rbegin()))
    s.pop_back();
}


int ourfind(std::string s)
{
  int a;
  a = s.find(";");
  return a;
}

std::string lineget() {
  std::string file;
  std::string line;
  while (!std::cin.eof()) {
    std::getline(std::cin, line);
    int ss = line.find_first_of("{}");
    while(ss != std::string::npos)
    {
      line.insert(line.begin() + ss, '\n');
      ss++;
      line.insert(line.begin() + ss + 1, '\n');
      int a = line.substr(ss + 1, std::string::npos).find_first_of("{}");
      if(a == std::string::npos)
        break;
      ss += a + 1;
    }
    int dd = line.find(";");
    while (dd != std::string::npos && line[dd + 1])
    {

      line.insert(line.begin() + dd + 1, '\n');
      int a = line.substr(dd + 1, std::string::npos).find(";");
      if(a == std::string::npos)
        break;
      dd += a + 1;
    }
    
    trim(line);
    if (line.empty() || line[0] == '#')
      continue;
    if (line.find("#") != std::string::npos)
      line = line.substr(0, line.find("#"));
    file += line + "\n";
  }
  return file;
}

Server parseserv(std::string &file) {
  Server ret;
  std::vector<Location> vec;
  int check = 0;
  int pos = file.find("server");
  if (pos)
    throw std::runtime_error("tabon mok 1");
  file = file.substr(6, file.size() - 6);
  trim(file);
  // std::cout << file;
  if (file[0] != '{')
    throw std::runtime_error("tabon mok 2");
  check++;
  std::cout << check << std::endl;
  int i = 0;
  while (++i < (int)file.size() && check > 0) {
    if (file[i] == '{')
      check++;
    else if (file[i] == '}')
      check--;
  }
  if (i == (int)file.size())
    throw std::runtime_error("tabon mok 3");
  std::string serv = file.substr(0, i);
  trim(file);
  file = file.substr(i + 1, file.size() - i);
  std::cout << serv << "\n------------\n";
  std::cout << check << std::endl;
  return ret;
}

int main() {
  if (!std::freopen("file.config", "r", stdin))
    return (1);
  std::string file = lineget();
  std::cout << file;
  std::vector<Server> vec;
  // while (file.size())
  //   parseserv(file);
}