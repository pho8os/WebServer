#include "Response.hpp"

Response::Response(void) {
	set_.init_vars__();
}
void	Response::getPage( request &req ) {
	Vect	sto_ = req.getVector();
	st_		body;
	st_		st;
	std::ofstream	file("error_file");
	std::ifstream	html("./html/404.html");
	while (std::getline(html, st)) {
		body += st;
		body += "\n";
	}
	file << req.getVersion() << " " << status_code << " " << error_codes[status_code] << "\n\n";
	file << "Date: Sun, 18 Oct 2023 10:21:30 GMT" << "\r\n";
	file << "Server: Apache/2.2.14 (Win32)" << "\r\n";
	file << "Content-Length: " << body.length()  << "\r\n";
	file << "Connection: " << "Closed" << "\r\n";
	file << "Content-Type: " << "text/html; charset=ios-8859-1" << "\r\n";
	file << "\r\n" << body;
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
		if (!server[0].location[idx].redirect.empty()) return 301;
		if (server[0].location[idx].prefix == URI) return idx;
		else return 404;
	}
	return false;
}
int	Response::checkMethods( request &req, std::vector < Server > server, int idx ) {
	if ((!server[0].location[idx].allow.Get && req.getMethod_() == "GET")
		|| (!server[0].location[idx].allow.Post && req.getMethod_() == "POST")
			|| (!server[0].location[idx].allow.Delete && req.getMethod_() == "DELETE")) return 405;
	return 200;
}
Response &Response::RetResponse( request &req ) {
	init_TheCont_();
	int error = isItinConfigFile( req.getURI(), set_.getVector() );
	if (error == 404 || error == 301) return status_code = error, getPage( req ), *this;
	else location = error;
	if ((error = checkMethods( req, set_.getVector(), location )))
		return status_code = error, getPage( req ), *this;
	return status_code = 200, getPage( req ), *this;
}
Response::~Response(void) {

}