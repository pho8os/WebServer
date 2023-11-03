
#include "ConfigFile.hpp"


void trim(std::string &s) {
  while (s.begin() != s.end() && isspace(*s.begin()))
    s.erase(s.begin());
  while (s.end() != s.begin() && isspace(*s.rbegin()))
    s.pop_back();
}

std::vector<std::string> lineget() {
  std::vector<std::string> file;
  std::string line;
  while (!std::cin.eof()) {
    std::getline(std::cin, line);
    if (line.empty() || line[0] == '#')
      continue;
    (line.find("#") != std::string::npos) && (line = line.substr(0, line.find("#")), 0);
    int ss = line.find_first_of(";{}");
    while(ss != std::string::npos)
    {
     (line[ss] == '{' || line[ss] == '}') && (line.insert(line.begin() + ss, '\n'), ss++);
      line.insert(line.begin() + ss + 1, '\n');
      int a = line.substr(ss + 1, std::string::npos).find_first_of(";{}");
      if(a == std::string::npos)
      {
        char *p = std::strtok((char *)line.c_str(), "\n");
        while(p)
        {
          std::string buff(p);
          trim(buff);
          file.push_back(buff + "\n");
          p = std::strtok(NULL, "\n");
        }
        ss = -1;
        break;
      }
      ss += a + 1;
    }
    if(ss == -1)
      continue;
    trim(line);
    file.push_back(line + "\n");
  }
  return file;
}

int main() {
  if (!std::freopen("file.config", "r", stdin))
    return (1);
  std::vector<std::string> file = lineget();
  for(int i = 0; i < file.size(); i++)
    std::cout << file[i];
  std::vector<Server> vec;

}