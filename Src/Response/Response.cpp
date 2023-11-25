#include "Response.hpp"

Response::Response(void) : loc(false) {

}
st_		Response::getRet() {
	return ret;
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
	// if (loc)
	// 	ret += "Location: " + set_.getConfig()[0].location[location].redirect.second + "\r\n";
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
            body += st += "\n";
    Set_Up_Headers( ret, req, body );
    ret += body;
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
	for (int idx = 0; idx < (int)server[0].location.size(); idx++) {
		if (server[0].location[idx].prefix == "/")
			root = idx;
		if (server[0].location[idx].prefix == URI) {
			location = idx;
			return;
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
	return 200;
}
bool	Response::index_file( int i, request &req ) {
	st_	dir_;
	st_	body;
	std::vector < Server > res = set_.getConfig();
	st_ root = res[0].location[location].root;
	if (root[root.length() - 1] == '/') dir_ = root + res[0].location[location].index[i];
	else dir_ = root + "/" + res[0].location[location].index[i];
	std::ifstream	file(dir_);
	if (!file.is_open())
		return false;
	while (std::getline(file, dir_))
		body += dir_ + "\n";
	Set_Up_Headers( ret, req, body );
	ret += body;
	return true;
}
int	Response::Fill_Resp( request &req, st_ root ) {
	st_	body;
	st_	dir;
	struct dirent *directory;
	std::vector < Server > res = set_.getConfig();
	if (res[0].location[location].autoindex) {
		DIR *dir = opendir( root.c_str() );
		body = "<div class=\"container\" style=\"display: flex;justify-content:center;align-items:center;height:100svh;flex-flow:column;\">\n";
		body += "<h1 style=\"font-size:30px;font-family:Arial;\">Directory</h1>\n";
		while ((directory = readdir(dir)))
			body += "<a style=\"color:orange;text-decoration:none;cursor:pointer;\" href=\"" + root + "\">" + directory->d_name + "</a><br>" + "\n";
		body += "</div>\n";
		Set_Up_Headers(ret, req, body);
		ret += body;
		return 1;
	}
	return 0;
}
void	Response::is_file( st_ path, request &req ) {
	st_	body, ret_;
	std::ifstream	file(path);
	if (!file.is_open())
		throw 404;
	while (std::getline(file, ret_))
		body += ret_ + "\n";
	Set_Up_Headers( ret, req, body );
	ret += body;
}
void	Response::is_dir( st_ root, std::vector < Server > res, request &req ) {
	st_	body, dir_;
	if (loc) {
		body = "<meta http-equiv=\"refresh\" content=\"0; URL='" + res[0].location[location].redirect.second + "'\"/>";
		Set_Up_Headers(ret, req, body);
		ret += body;
		return ;
	}
	std::ifstream file(root + "index.html");
	if (!file.is_open()) {
		if (Fill_Resp( req, root )) return ;
		if ((int)res[0].location[location].index.size() == 0)
        	throw 403;
		for (int i = 0; i < (int)res[0].location[location].index.size() && !index_file(i, req); i++);
	}
	else {
		while (std::getline(file, dir_))
			body += dir_ + "\n";
		Set_Up_Headers( ret, req, body );
		ret += body;
		return ;
	}

}
void	Response::GETResource( request &req ) {
	st_	body;
	struct stat stru_t;
	std::vector < Server > res = set_.getConfig();
	st_ root = res[0].location[location].root;
	try {
		if (stat(root.c_str(), &stru_t) == 0) {
			if (S_ISREG(stru_t.st_mode))
				is_file( root, req );
			else if (S_ISDIR(stru_t.st_mode))
				is_dir( root, res, req );
		}
		else
			throw 404;
	}
	catch (int code_) {
		throw code_;
	}
}
Response &Response::RetResponse( request &req ) { // max body size || redirect || location //
	init_TheCont_();
	status_code = 200;
	if (!req.getBoolean())
		return status_code = req.getCode(), getPage(req), *this;
	try {
		isItinConfigFile( req.getURI(), set_.getConfig() );
		checkMethods( req, set_.getConfig(), location );
		if (!req.getMethod_().compare("GET"))
			GETResource(req);
		if (!req.getMethod_().compare("POST")) {
			ob_post.setUpPath( "/Users/mnassi/Desktop/1337/WebServer" );
			ob_post.runPost("./01.png");
		}
	}
	catch (int code) {
		return status_code = code, getPage(req), *this;
	}
	return *this;
}
Response::~Response(void) {

}