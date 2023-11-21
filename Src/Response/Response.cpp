#include "Response.hpp"

Response::Response(void) {
	set_.init_vars__();
}
st_		Response::getRet() {
	return ret;
}
void	Response::Set_Up_Headers( st_ &ret, request &req ) {
	ret = req.getVersion() + " " + std::to_string(status_code) + " " + error_codes[status_code] + "\r\n";
	ret += "Host: localhost:8080\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\n\r\n";
}
void	Response::getPage( request &req ) {
	// std::ofstream	file("error_file");
	std::ifstream	html("./html/" + std::to_string(status_code) + ".html");
	Map	sto_ = req.getVector();
	st_		body;
	st_		st;

	while (std::getline(html, st))
		body += st += "\n";
	// file << req.getVersion() << " " << status_code << " " << error_codes[status_code] << "\n\n";
	// file << "Date: Sun, 18 Oct 2023 10:21:30 GMT" << "\r\n";
	// file << "Server: " << SERVER << "\r\n";
	// file << "Content-Length: " << body.length()  << "\r\n";
	// file << "Connection: " << sto_["Connection"] << "\r\n";
	// if (!sto_["Content-Type"].empty())
	// 	file << "Content-Type: " << sto_["Content-Type"] << "\r\n";
	// else
	// 	file << "Content-Type: text/html" << "\r\n";
	// file << "\r\n" << body;
	Set_Up_Headers( ret, req );
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
int Response::isItinConfigFile( st_ URI, std::vector < Server > server ) const {
	for (int idx = 0; idx < (int)server[0].location.size(); idx++) {
		if (!server[0].location[idx].redirect.empty()) throw 301;
		if (server[0].location[idx].prefix == URI) return idx;
		else throw 404;
	}
	return 200;
}
int	Response::checkMethods( request &req, std::vector < Server > server, int idx ) {
	if ((!server[0].location[idx].allow.Get && req.getMethod_() == "GET")
		|| (!server[0].location[idx].allow.Post && req.getMethod_() == "POST")
			|| (!server[0].location[idx].allow.Delete && req.getMethod_() == "DELETE")) throw 405;
	return 200;
}
void	Response::index_file( int i, request &req ) {
	st_	dir_;
	st_	body;
	std::vector < Server > res = set_.getVector();
	st_ root = res[0].location[location].root;
	if (root[root.length() - 1] == '/') dir_ = root + res[0].location[location].index[i];
	else dir_ = root + "/" + res[0].location[location].index[i];
	std::ifstream	file(dir_);
	Set_Up_Headers( ret, req );
	while (std::getline(file, dir_))
		body += dir_ + "\n";
	ret += body;
}
int	Response::GETResource( request &req ) {
	struct dirent *directory;
	std::vector < Server > res = set_.getVector();
	st_ root = res[0].location[location].root;
	if (res[0].location[location].autoindex) {
		ret = "HTTP/1.1 404 NOT_FOUND\r\n\r\n";
		ret += "<h1>Directory</h1>\n";
		DIR *dir = opendir( root.c_str() );
		while ((directory = readdir(dir)))
			ret += "<a href=\"" + root + "\">" + directory->d_name + "</a><br>" + "\n";
		return 200;
	}
	for (int i = 0; i < (int)res[0].location[location].index.size() - 1; i++) {
		if (res[0].location[location].index[i] == "index.html")
			index_file(i, req);
		else
			index_file(0, req);
	}
	return 200;
}
Response &Response::RetResponse( request &req ) {
	init_TheCont_();
	if (!req.getBoolean())
		return status_code = req.getCode(), getPage(req), *this;
	try {
		location = isItinConfigFile( req.getURI(), set_.getVector() );
		checkMethods( req, set_.getVector(), location );
		if (!req.getMethod_().compare("GET"))
			GETResource(req);
		// else if (!req.getMethod_().compare("POST"))
		// 	error = POSTResource();
		// else if (!req.getMethod_().compare("DELETE"))
		// 	error = DELETEResource();
	}
	catch (int code) {
		return status_code = code, getPage(req), *this;
	}
	return *this;
}
Response::~Response(void) {

}