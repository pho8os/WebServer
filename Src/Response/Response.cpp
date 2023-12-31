#include "Response.hpp"
#include <string>
#include <sys/stat.h>
#include <vector>

Response::Response(void) : sending(true),loc(false) , headersent(false) {

}
int		Response::getFd() {
	return fd;
}
st_		&Response::getRet() {
	return ret;
}
void  Response::setRet( st_ &ret ) {
	this->ret = ret;
}
struct fd_	&Response::getStruct() {
	return setting;
}
void	Response::clear_Obj() {
	fd = -1;
	ret = "";
	status_code = 200;
}
void	Response::Set_Up_Headers( st_ &ret, request &req, st_ body ) {
	Map	sto_ = req.getVector();
	st_ conn = "Connection";
	st_ ctype = "Content-Type";
	st_ serv = "Server";
	std::time_t curr_time = std::time(0);
	st_ Date = std::ctime(&curr_time);
	ret = req.getVersion() + " " + std::to_string(status_code) + " " + error_codes[status_code] + "\r\n";
	ret += "Date: " + Date.substr(0, Date.length() - 1) + " GMT\r\n";
	if (!sto_["connection"].empty()) ret += conn + ": " + sto_["connection"] + "\r\n";
	else ret += conn + ": " + "closed\r\n";
	ret += serv + ": " + SERVER + "\r\n";
	ret += "Content-Length: " + std::to_string(body.length()) + "\r\n";
	size_t p = req.getURI().rfind(".");
	std::cout << text_types[req.getURI().substr(p + 1)] << "\n";
	if (p != std::string::npos && !text_types[req.getURI().substr(p + 1)].empty()) ret += ctype + ": " + text_types[req.getURI().substr(p + 1)] + "\r\n\r\n";
	else ret += ctype + ": text/html\r\n\r\n";
}
void    Response::getPage( request &req ) {
    st_        body;
    st_        st;
    Map    sto_ = req.getVector();
    std::map < int, st_ > conf_ = set_.getConfig()[0].error_page;
    std::ifstream    html(conf_[(int)status_code]);
    if (!html.is_open())
        body = Create_DefPage();
    else
        while (std::getline(html, st))
            body += st += "\r\n";
    Set_Up_Headers( ret, req, body );
    ret += body;
}
void	Response::content_types() {
	text_types["html"] = "text/html";
    text_types["css"] = "text/css";
    text_types["js"] = "application/javascript";
    text_types["jpg"] = "image/jpeg";
    text_types["jpeg"] = "image/jpeg";
    text_types["png"] = "image/png";
	text_types["svg"] = "image/svg+xml";
    text_types["mp4"] = "video/mp4";
    text_types["mp3"] = "audio/mpeg";
    text_types["json"] = "application/json";
    text_types["pdf"] = "application/pdf";
    text_types["txt"] = "text/plain";
    text_types["xml"] = "application/xml";
    text_types["zip"] = "application/zip";
    text_types["rar"] = "application/x-rar-compressed";
    text_types["gz"] = "application/gzip";
    text_types["tar"] = "application/x-tar";
    text_types["doc"] = "application/msword";
    text_types["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    text_types["xls"] = "application/vnd.ms-excel";
    text_types["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    text_types["ppt"] = "application/vnd.ms-powerpoint";
    text_types["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    text_types["odt"] = "application/vnd.oasis.opendocument.text";
    text_types["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    text_types["odp"] = "application/vnd.oasis.opendocument.presentation";
    text_types["odg"] = "application/vnd.oasis.opendocument.graphics";
    text_types["odc"] = "application/vnd.oasis.opendocument.chart";
    text_types["odf"] = "application/vnd.oasis.opendocument.formula";
    text_types["odg"] = "application/vnd.oasis.opendocument.graphics";
    text_types["odp"] = "application/vnd.oasis.opendocument.presentation";
    text_types["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    text_types["odt"] = "application/vnd.oasis.opendocument.text";
    text_types["rtf"] = "application/rtf";
    text_types["wma"] = "audio/x-ms-wma";
    text_types["wmv"] = "video/x-ms-wmv";
    text_types["webm"] = "video/webm";
    text_types["flv"] = "video/x-flv";
    text_types["avi"] = "video/x-msvideo";
    text_types["swf"] = "application/x-shockwave-flash";
    text_types["exe"] = "application/x-msdownload";
    text_types["msi"] = "application/x-msdownload";
    text_types["cab"] = "application/vnd.ms-cab-compressed";
    text_types["deb"] = "application/x-debian-package";
    text_types["gz"] = "application/x-gzip";
    text_types["rar"] = "application/x-rar-compressed";
    text_types["iso"] = "application/x-iso9660-image";
}
void	Response::init_TheCont_() {
	error_codes[200] = "OK";
	error_codes[201] = "CREATED";
	error_codes[202] = "ACCEPTED";
	error_codes[204] = "NO_CONTENT";
	error_codes[301] = "MOVED_PERMANENTLY";
	error_codes[302] = "FOUND";
	error_codes[303] = "SEE_OTHER";
	error_codes[304] = "NOT_MODIFIED";
	error_codes[307] = "TEMPORARY_REDIRECT";
	error_codes[400] = "BAD_REQUEST";
	error_codes[401] = "UNAUTHORIZED";
	error_codes[403] = "FORBIDDEN";
	error_codes[404] = "NOT_FOUND";
	error_codes[405] = "METHOD_NOT_ALLOWED";
	error_codes[408] = "REQUEST_TIMEOUT";
	error_codes[409] = "Conflict";
	error_codes[500] = "INTERNAL_SERVER_ERROR";
	error_codes[501] = "NOT_IMPLEMENTED";
	error_codes[502] = "BAD_GATEWAY";
	error_codes[503] = "SERVICE_UNAVAILABLE";
	error_codes[504] = "GATEWAY_TIMEOUT";
	error_codes[505] = "HTTP_VERSION_NOT_SUPPORTED";
	error_codes[413] = "REQUEST_ENTITY_TOO_LARGE";
}
st_	Response::Create_DefPage() {
	return "<div style=\"display: flex;font-size: 70px;letter-spacing: 5px;font-family: Arial, Helvetica, sans-serif;height: 100svh;justify-content: center;flex-flow: column;align-items: center;\">\n<h1>" + std::to_string(status_code) + "</h1>\n" + "<h3 style=\"font-size:20px;\">" + error_codes[status_code] + "</h3>\n</div>\n";
}
void Response::isItinConfigFile(st_ URI) {
  int root = -1;
  std::vector<std::string> prefix;
  std::vector<Location> locations = srv.location;
  for (int idx = 0; idx < (int)locations.size(); idx++) {
    if (locations[idx].prefix == "/")
      root = idx;
    prefix.push_back(locations[idx].prefix);
  }
  std::sort(prefix.begin(), prefix.end());
  for (int idx = prefix.size() - 1; idx >= 0; idx--) {
    if (prefix[idx] + "/" == URI.substr(0, prefix[idx].length() + 1)) {
      for (int i = 0; i < (int)locations.size(); i++)
        if (locations[i].prefix == prefix[idx])
          location = i;
      return;
    }
  }
  if (root != -1)
    location = root;
  else
    throw 404;
}
int	Response::checkMethods( request &req ) {
	if (!srv.location[location].redirect.second.empty()) status_code = srv.location[location].redirect.first, loc = true;
	if ((!srv.location[location].allow.Get && req.getMethod_() == "GET")
		|| (!srv.location[location].allow.Post && req.getMethod_() == "POST")
			|| (!srv.location[location].allow.Delete && req.getMethod_() == "DELETE")) throw 405;
	return 200;
}
bool	Response::index_file( request &req, st_ path ) {
	st_	body;
	st_	dir_;
	std::ifstream file(path);
	// std::cout << path << std::endl;
	if ( !file.is_open() )
		return false;
	while (std::getline(file, dir_))
		body += dir_ + "\r\n";
	Set_Up_Headers( ret, req, body );
	ret += body;
	return true;
}
int	Response::Fill_Resp( request &req, st_ root ) {
	st_	body;
	struct dirent *directory;
	st_ href = req.getURI();
	if (href[href.length() - 1] != '/')
		href += "/";
	if (srv.location[location].autoindex) {
		DIR *dir = opendir( root.c_str() );
		if (!dir)
			throw 404;
		body = "<div class=\"container\" style=\"display: flex;justify-content:center;align-items:center;height:100svh;flex-flow:column;\">\n";
		body += "<h1 style=\"font-size:30px;font-family:Arial;\">Directory</h1>\n";
		while ((directory = readdir(dir)))
			 body += "<a style=\"color:orange;text-decoration:none;cursor:pointer;\" href=\"" + href + directory->d_name + "\">" + directory->d_name + "</a><br>" + "\n";
		body += "</div>\n";
		Set_Up_Headers(ret, req, body);
		ret += body;
		closedir(dir);
		return 1;
	}
	return 0;
}
void    Response::is_file( st_ path, request &req ) {
    fd = open(path.c_str(), O_RDONLY, 0777);

	setting = fd_( fd );
    setting.offset = 0;
    setting.len = setting.statbuf.st_size;

    Map    sto_ = req.getVector();
    st_ conn = "Connection"; //check this
    st_ ctype = "Content-Type";
    st_ serv = "Server";
    std::time_t curr_time = std::time(0);
    st_ Date = std::ctime(&curr_time);
    ret = req.getVersion() + " " + std::to_string(status_code) + " " + error_codes[status_code] + "\r\n";
    ret += "Date: " + Date.substr(0, Date.length() - 1) + " GMT " + "\r\n";
    if (!conn.empty() && !sto_["connection"].empty()) ret += conn + ": " + sto_["connection"] + "\r\n";
    else ret += conn + ": " + "closed\r\n";
    ret += serv + ": " + SERVER + "\r\n";
    ret += "Content-Length: " + std::to_string(setting.statbuf.st_size) + "\r\n";
    if (text_types[req.getURI().substr(req.getURI().find(".") + 1)].empty())
		ret += ctype + ": text/plain\r\n\r\n";
	else
    	ret += ctype + ": " + text_types[req.getURI().substr(req.getURI().find(".") + 1)] + "\r\n\r\n";
}
void	Response::is_dir( st_ root, request &req ) {
	int	i;
	st_	body, dir_;
	if (root[root.length() - 1] != '/')
		root += "/";
	std::ifstream file(root + "index.html");
	if (loc) {
		body = "<meta http-equiv=\"refresh\" content=\"0; URL='" + srv.location[location].redirect.second + "'\"/>";
		Set_Up_Headers( ret, req, body );
		ret += body;
	}
	else if (!file.is_open()) {
		if (Fill_Resp( req, root )) return ;
		for (i = 0; i < (int)srv.location[location].index.size(); i++)
			if (index_file( req, root + srv.location[location].index[i]) )
				return ;
		if ( i == (int)srv.location[location].index.size() || (int)srv.location[location].index.size() == 0 )
        	throw 403;
	}
	else {
		while (std::getline(file, dir_))
			body += dir_ + "\r\n";
		Set_Up_Headers( ret, req, body );
		ret += body;
	}
}
void	Response::GETResource( request &req ) {
	st_	body;
	st_	path;
	size_t pos;
	struct stat stru_t;
	st_ root = srv.location[location].root;
	if (srv.location[location].prefix != "/")
		path = root + req.getURI().substr(srv.location[location].prefix.length());
	if (srv.location[location].prefix != "/" && req.getURI().substr(srv.location[location].prefix.length()) == "/")
		path = root + req.getURI().substr(srv.location[location].prefix.length() + 1);
	else
		path = root + req.getURI();
	if ((pos = req.getURI().find("?")) != std::string::npos)
        path = root + req.getURI().substr(0, req.getURI().find("?"));
	try {
		std::cout << path << std::endl;
		if (stat(path.c_str(), &stru_t) == 0) {
			if (S_ISREG(stru_t.st_mode))
				is_file( path, req );
			else if (S_ISDIR(stru_t.st_mode))
				is_dir( path, req );
		}
		else
			throw 404;
	}
	catch (int code_) {
		throw code_;
	}
}
void    Response::deleteFile( request &req ) {
    // if (!srv.location[location].cgi.empty())
	// 	throw 502;
	if (access(inf.first_path.c_str(), W_OK) == 0) {
        remove(inf.first_path.c_str());
        throw 204;
    }
    throw 403;
}
void	Response::openDir( st_ path, request &req ) {
	inf.path = path;
	inf.dir = opendir(inf.path.c_str());
	if (inf.dir)
		deleteDir( req );
	else
		throw 404;
}
void    Response::deleteDir( request &req ) {
    st_ file_or_dir;
    static int idx;
    while ((inf.directory = readdir(inf.dir)) != NULL) {
        if ((inf.directory->d_name[0] == '.' && !inf.directory->d_name[1])
            || (inf.directory->d_name[0] == '.' && inf.directory->d_name[1] == '.' && !inf.directory->d_name[2]))
            continue;
        file_or_dir = inf.path + inf.directory->d_name;
        if (stat(file_or_dir.c_str(), &inf.stru_t) == -1)
            throw 404;
        if (S_ISREG(inf.stru_t.st_mode) && (access(inf.first_path.c_str(), W_OK) == 0))
            inf.files.push_back(file_or_dir);
        else if (S_ISDIR(inf.stru_t.st_mode))
            inf.directories.push_back(file_or_dir);
        else
			throw 403;
    }
    for (std::vector < st_ >::iterator it_ = inf.files.begin(); it_ != inf.files.end(); it_++)
        if (remove((*it_).c_str()) == -1)
			throw 500;
	inf.files.clear();
    while (idx < (int)inf.directories.size())
        openDir((inf.directories[idx++] + "/").c_str(), req);
	for (std::vector < st_ >::iterator it_ = inf.directories.end() - 1; inf.directories.size() > 0 && it_ >= inf.directories.begin(); it_--)
		if (remove((*it_).c_str()) == -1)
			throw 500;
	if (remove(inf.first_path.c_str()) == -1)
		throw 500;
	idx = 0;
    closedir(inf.dir);
    bzero(&inf, sizeof(inf));
    throw 204;
}
void	Response::DeleteContent( request &req, st_ path ) {
	inf.first_path = path;
	// std::cout << path << std::endl;
	if (stat(path.c_str(), &inf.stru_t) == -1)
		throw 404;
	if (S_ISREG(inf.stru_t.st_mode))
		deleteFile( req );
	else if (S_ISDIR(inf.stru_t.st_mode))
		openDir( path, req );
}
void	Response::DELResource( request &req ) {
	st_	body;
	st_ root = srv.location[location].root;
	if (srv.location[location].prefix == "/")
		root += "/";
	st_	path = root + req.getURI().substr(srv.location[location].prefix.length());
	try {
		DeleteContent( req, path );
	}
	catch (int code_) {
		throw code_;
	}
}
void	Response::countCgiBody( request req ) {
	st_	body, app;
	int	length;
	std::time_t curr_time = std::time(0);
	st_ Date = std::ctime(&curr_time);
	std::fstream	file(req.cgiResult);
	if (!file.is_open())
		perror("could not open file\n");
	while (std::getline(file, body))
		app += body + "\n";
	st_ headers = req.getVersion() + " " + std::to_string(status_code) + " " + error_codes[status_code] + "\r\n";
	if (app.find("\r\n\r\n") == st_::npos)
	{
		headers += "Content-Type : text/html; charset=utf-8\r\n";
		body = app;
	}
	else {
		headers += app.substr(0, app.find("\r\n\r\n")) + "\r\n";
		length = app.substr(app.find("\r\n\r\n") + 4, app.length()).length();
		body = app.substr(app.find("\r\n\r\n") + 4, app.length());
	}
	headers += "Content-Length: " + std::to_string(body.length()) + "\r\n";
	headers += "Date: " + Date.substr(0, Date.length() - 1) + " GMT\r\n\r\n";
	ret = headers + body;
	// std::cout << ret << std::endl;
}
Response &Response::RetResponse( request &req ) { // max body size || server || trim headers
	fd = -1;
	// std::cout << req.getCode();
	// std::cout << "code > \n" << req.getCode() << std::endl;
	// std::cout << "-> method \n" << req.getMethod_() << std::endl;
	init_TheCont_();
	content_types();
	status_code = 200;
	this->srv = req.Serv;
	buffer = new char [4096];
	if (!req.getBoolean())
		return status_code = req.getCode(), getPage(req), *this;
	try {
		isItinConfigFile( req.getURI() );
		checkMethods( req );
		if (!req.getMethod_().compare("DELETE"))
			DELResource( req );
		else if (req.cgi)
			countCgiBody(req);
		else if (!req.getMethod_().compare("GET"))
			GETResource( req );
	}
	catch (int code) {
		return status_code = code, getPage(req), *this;
	}
	return *this;
}
Response::~Response(void) {

}