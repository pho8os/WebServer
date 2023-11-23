
#include <string>
#include <sys/_types/_size_t.h>
#include <vector>
#include <deque>
#include <map>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include<iostream>
#include <cstring>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <utility>

void trim(std::string &s) {
  while (s.begin() != s.end() && isspace(*s.begin()))
    s.erase(s.begin());
  while (s.end() != s.begin() && isspace(*s.rbegin()))
    s.pop_back();
}

void push_multiline(std::deque<std::string> &file, std::string &line,
                    size_t &dd) {
  while (dd != std::string::npos) {
    (line[dd] == '{' || line[dd] == '}') &&
        (line.insert(line.begin() + dd, '\n'), dd++);
    line.insert(line.begin() + dd + 1, '\n');
    size_t a = line.substr(dd + 1, std::string::npos).find_first_of(";{}");
    dd = (a == std::string::npos) * a + (a != std::string::npos) * (a + dd + 1);
  }
  char *p = std::strtok((char *)line.c_str(), "\n");
  while (p) {
    std::string buff(p);
    trim(buff);
    (!buff.empty()) && (file.push_back(buff), 0);
    p = std::strtok(NULL, "\n");
  }
}

void valid_line(std::deque<std::string> &file, std::string &line) {
  trim(line);
  (line.find("#") != std::string::npos) &&
      (line = line.substr(0, line.find("#")), 0);
  if (line.empty())
    return;
  size_t dd = line.find_first_of(";{}");
  if (dd != std::string::npos)
    return (push_multiline(file, line, dd), (void)0);
  trim(line);
  (!line.empty()) && (file.push_back(line), 0);
}

std::deque<std::string> getfile(const std::string &path)
{
  if (!std::freopen(path.c_str(), "r", stdin)) {
    std::cout << "Config file not found" << std::endl;
    std::exit(-1);
  }
  std::deque<std::string> file;
  std::string line;
  while(!std::cin.eof())
  {
    std::getline(std::cin , line);
    valid_line(file, line);
  }
  return file;
}


int main()
{
  std::deque<std::string> file = getfile("./Src/ConfigFile/file.config");
  for(size_t i = 0; i < file.size(); i++)
    std::cout << file[i] << std::endl;
  
}

//./Src/ConfigFile/file.config