#include "Response.hpp"

Response::Response(void) : loc(false) , headersent(false) {

}
st_		Response::getRet() {
	return ret;
}
void  Response::setRet( st_ &ret ) {
	this->ret = ret;
}
void	Response::Set_Up_Headers( st_ &ret, request &req, st_ body ) {
	Map	sto_ = req.getVector();
	st_ conn = "Connection";
	st_ ctype = "Content-Type";
	st_ serv = "Server";
	std::time_t curr_time = std::time(0);
	st_ Date = std::ctime(&curr_time);
	ret = req.getVersion() + " " + std::to_string(status_code) + " " + error_codes[status_code] + "\r\n";
	ret += "Date: " + Date.substr(0, Date.length() - 1) + " GMT " + "\r\n";
	if (!conn.empty() && !sto_[conn].empty()) ret += conn + ": " + sto_[conn] + "\r\n";
	else ret += conn + ": " + "closed\r\n";
	ret += serv + ": " + SERVER + "\r\n";
	ret += "Content-Length: " + std::to_string(body.length()) + "\r\n";
	if (!ctype.empty() && !sto_[ctype].empty()) ret += ctype + ": " + sto_[ctype] + "\r\n\r\n";
	else ret += ctype + ": text/html; charset=iso-8859-1\r\n\r\n";
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

void Response::isItinConfigFile( st_ URI, std::vector < Server > server ) {
	int root = -1;
	std::vector < std::string > prefix;
	std::vector <Location> locations = server[0].location;
	for (int idx = 0; idx < (int)locations.size(); idx++)
		prefix.push_back(locations[idx].prefix);
	std::sort(prefix.begin(), prefix.end());
	for (int idx = prefix.size() - 1; idx >= 0; idx--) {
		if (prefix[idx] == "/")
			root = idx;
		else if (prefix[idx] + "/" == URI.substr(0, prefix[idx].length() + 1)) {
			std::cout << prefix[idx] << std::endl;
			for (int i = 0; i < (int)locations.size(); i++)
				if (locations[i].prefix == prefix[idx])
					location = i;
			return ;
		}
	}
	if (root != -1)
		location = root;
	else
		throw 404;
}
int	Response::checkMethods( request &req, std::vector < Server > server, int idx ) {
	if (!server[0].location[idx].redirect.second.empty()) status_code = server[0].location[idx].redirect.first, loc = true;
	if ((!server[0].location[idx].allow.Get && req.getMethod_() == "GET")
		|| (!server[0].location[idx].allow.Post && req.getMethod_() == "POST")
			|| (!server[0].location[idx].allow.Delete && req.getMethod_() == "DELETE")) throw 405;
	// if ((req.getURI().find(".py") != std::string::npos || req.getURI().find(".php") != std::string::npos)
	// 	&& (!server[0].location[location].cgi["py"].empty() || !server[0].location[location].cgi["php"].empty())) throw Cgi();
	return 200;
}
bool	Response::index_file( request &req, st_ path ) {
	st_	body;
	st_	dir_;
	std::ifstream file(path);
	std::cout << path << std::endl;
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
	std::vector < Server > res = set_.getConfig();
	st_		href = req.getURI().substr(res[0].location[location].prefix.length());
	std::cout << href << std::endl;
	if (res[0].location[location].autoindex) {
		DIR *dir = opendir( root.c_str() );
		if (!dir)
			throw 404;
		body = "<div class=\"container\" style=\"display: flex;justify-content:center;align-items:center;height:100svh;flex-flow:column;\">\n";
		body += "<h1 style=\"font-size:30px;font-family:Arial;\">Directory</h1>\n";
		while ((directory = readdir(dir))) {
			if (href[href.length() - 1] != '/')
				href += '/';
			 body += "<a style=\"color:orange;text-decoration:none;cursor:pointer;\" href=\"" + href + directory->d_name + "\">" + directory->d_name + "</a><br>" + "\n";
		}
		body += "</div>\n";
		Set_Up_Headers(ret, req, body);
		ret += body;
		closedir(dir);
		return 1;
	}
	return 0;
}
void	Response::is_file( st_ path, request &req ) {
	struct stat fd_;
	fd = open(path.c_str(), O_RDONLY, 0777);
	if (stat(path.c_str(), &fd_) == -1)
		throw 404;;
	Map	sto_ = req.getVector();
	st_ conn = "Connection";
	st_ ctype = "Content-Type";
	st_ serv = "Server";
	std::time_t curr_time = std::time(0);
	st_ Date = std::ctime(&curr_time);
	ret = req.getVersion() + " " + std::to_string(status_code) + " " + error_codes[status_code] + "\r\n";
	ret += "Date: " + Date.substr(0, Date.length() - 1) + " GMT " + "\r\n";
	if (!conn.empty() && !sto_[conn].empty()) ret += conn + ": " + sto_[conn] + "\r\n";
	else ret += conn + ": " + "closed\r\n";
	ret += serv + ": " + SERVER + "\r\n";
	ret += "Content-Length: " + std::to_string(fd_.st_size) + "\r\n";
	ret += ctype + ": " + text_types[req.getURI().substr(req.getURI().find(".") + 1)] + "\r\n\r\n";
}
void	Response::is_dir( st_ root, std::vector < Server > res, request &req ) {
	int	i;
	st_	body, dir_;
	if (root[root.length() - 1] != '/')
		root += "/";
	if (loc) {
		body = "<meta http-equiv=\"refresh\" content=\"0; URL='" + res[0].location[location].redirect.second + "'\"/>";
		Set_Up_Headers( ret, req, body );
		ret += body;
		return ;
	}
	std::ifstream file(root + "index.html");
	if (!file.is_open()) {
		if (Fill_Resp( req, root )) return ;
		if ( (int)res[0].location[location].index.size() == 0 )
        	throw 403;
		for (i = 0; i < (int)res[0].location[location].index.size(); i++)
			if (index_file( req, root + res[0].location[location].index[i]) )
				return ;
		if ( i == (int)res[0].location[location].index.size() )
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
	struct stat stru_t;
	std::vector < Server > res = set_.getConfig();
	st_ root = res[0].location[location].root;
	if (res[0].location[location].prefix == "/")
		root += "/";
	path = root + req.getURI().substr(res[0].location[location].prefix.length());
	try {
		if (stat(path.c_str(), &stru_t) == 0) {
			if (S_ISREG(stru_t.st_mode))
				is_file( path, req );
			else if (S_ISDIR(stru_t.st_mode))
				is_dir( path, res, req );
		}
		else
			throw 404;
	}
	catch (int code_) {
		throw code_;
	}
}
void    Response::deleteFile( st_ path,  request &req, struct stat &stru_t ) {
	(void)req;
    mode_t permission = stru_t.st_mode;
    std::vector < Server > conf = set_.getConfig();
    if (!conf[0].location[location].cgi.empty())
        // cgi call
    if (permission & S_IWUSR) {
        remove(path.c_str());
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
        inf.permission = inf.stru_t.st_mode;
        if (S_ISREG(inf.stru_t.st_mode) && (inf.permission & S_IWUSR))
            inf.files.push_back(file_or_dir);
        else if (S_ISDIR(inf.stru_t.st_mode))
            inf.directories.push_back(file_or_dir);
        else
            throw 403;
    }

    for (std::vector < st_ >::iterator it_ = inf.files.begin(); it_ != inf.files.end(); it_++)
        remove((*it_).c_str());
    while (idx < (int)inf.directories.size())
        openDir((inf.directories[idx++] + "/").c_str(), req);
	while (idx--)
		remove(inf.directories[idx].c_str());
	remove(inf.first_path.c_str());
    closedir(inf.dir);
    bzero(&inf, sizeof(inf));
    throw 204;
}
void	Response::DeleteContent( request &req, st_ path ) {
	struct stat stru_t;
	inf.first_path = path;
	if (stat(path.c_str(), &stru_t) == 0) {
		if (S_ISREG(stru_t.st_mode))
			deleteFile( path, req, stru_t );
		else if (S_ISDIR(stru_t.st_mode))
			openDir( path, req );
		return ;
	}
	if (path[path.length() - 1] != '/')
		throw 409;
	else
		throw 404;
}
void	Response::DELResource( request &req ) {
	st_	body;
	std::vector < Server > res = set_.getConfig();
	st_ root = res[0].location[location].root;
	if (res[0].location[location].prefix == "/")
		root += "/";
	st_	path = root + req.getURI().substr(res[0].location[location].prefix.length());
	try {
		DeleteContent( req, path );
	}
	catch (int code_) {
		std::cout << code_ << std::endl;
		throw code_;
	}
}
Response &Response::RetResponse( request &req ) { // redirect || location // error pages // max body size // length
	init_TheCont_();
	content_types();
	status_code = 200;
	buffer = new char [4096];
	if (!req.getBoolean())
		return status_code = req.getCode(), getPage(req), *this;
	try {
		isItinConfigFile( req.getURI(), set_.getConfig() );
		checkMethods( req, set_.getConfig(), location );
		if (!req.getMethod_().compare("GET"))
			GETResource( req );
		if (!req.getMethod_().compare("DELETE"))
			DELResource( req );
	}
	catch (int code) {
		return status_code = code, getPage(req), *this;
	}
	// catch (Cgi &e) {
	// 	st_ root = set_.getConfig()[0].location[location].root + req.getURI().substr(set_.getConfig()[0].location[location].prefix.length());
		// if (access(root.c_str(), F_OK) == 0)
			// e.run( req.getMethod_(), root );
	// }
	return *this;
}
Response::~Response(void) {

}
 