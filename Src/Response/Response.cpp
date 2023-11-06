#include "Response.hpp"

Response::Response() {

}
bool Response::isItinConfigFile( st_ URI, std::vector < Server > server ) const {
	for (std::vector < Server >::iterator it_ = server.begin(); it_ != server.end(); it_++)
		if ()
	return true;
}
Response &Response::RetResponse( request &req ) {
	if (!isItinConfigFile( req.getURI(), set_.getVector() ))
		std::cout << "asfasdf\n";
	return (*this);
}
Response::~Response() {

}