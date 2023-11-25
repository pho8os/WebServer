#include <iostream>
#include <map>
#include <utility>

#include "../Request.hpp"
class post
{
private:
    st_ _upPath;
    st_ _boundary;
    st_ _endBoundary;
    std::vector<st_> _fields;
    std::vector<std::pair<st_, st_> > _urlFields;
    std::vector<std::pair<st_, st_> > _dataFields;
    std::vector<st_> _binFiles;
    std::vector<st_> _binFileNames;

public:
request req;
    post();
    ~post();
    
    void setUpPath(const st_ &str);
    
    std::vector<st_> ft_split(const st_ req, const st_ del);
    st_ fileToStr(const st_ path);
    void detectBoundry();
    bool isValidReq();
    void detectFields();
    void detectUrlFields();
    void detectDataFields();
    void detectBinaryFiles();
    void makeResponse(int code);
    void makeFiles();
    void parseFiles();


    void runPost(st_ path);
};