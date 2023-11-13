#include "Response.hpp"

Response::Response(void) {
	set_.init_vars__();
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
	return 1;
}
Response &Response::RetResponse( request &req ) {
	int error = isItinConfigFile( req.getURI(), set_.getVector() );
	if (error == 404 || error == 301)
		return status_code = error, *this;
	if ((error = checkMethods( req, set_.getVector(), error )) != 1)
		return std::cout << error << std::endl, status_code = error, *this;
	return (status_code = 1, *this);
}
Response::~Response(void) {

}