#include "Response.hpp"

Response::Response(void) {

}
bool Response::isItinConfigFile( st_ URI, std::vector < Server > server ) const {
	for (int idx = 0; idx < (int)server[0].location.size(); idx++)
		if (server[0].location[idx].prefix == URI)
			return true;
	return false;
}
Response &Response::RetResponse( request &req ) {
	if (!isItinConfigFile( req.getURI(), set_.getVector() ))
		std::cout << "404 Not Found\n";
	return (*this);
}
Response::~Response(void) {

}