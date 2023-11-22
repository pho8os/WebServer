/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:15:16 by mnassi            #+#    #+#             */
/*   Updated: 2023/11/22 15:42:46 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request/Request.hpp"
#include "Response/Response.hpp"
#include "Request/post/post.hpp"
#include "Server/Server.hpp"
#include "Server/Server.hpp"
#include "cgi/Cgi.hpp"

int main(int ac, char **av)
{
    // server part
    //  try
    // {
    //     if (ac != 2)
    //         throw std::runtime_error("Bad args for testing");
    //     MServer serv1(std::atoi(av[1]));
    //     serv1.initServer();
    // }
    // catch(std::exception &e)
    // {
    //     std::cout << e.what() << '\n';
    // }

    // post part
    (void)ac;
    (void)av;
    // try
    // {
    //     post p;
    //     p.runPost();
    //     // p.SplitReq(st);
    // }
    // catch (std::exception &e)
    // {
    //     std::cerr << e.what() << std::endl;
    // }

    Cgi part;
    std::map<st_, st_> env;
    env["DOCUMENT_ROOT"] = "/var/www/html";
    env["HTTP_COOKIE"] = "user=JohnDoe; sessionid=123456789";
    env["HTTP_HOST"] = "localhost:9000";
    env["HTTP_REFERER"] = "http://localhost:9000/www/previous_page.php";
    env["HTTP_USER_AGENT"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36";
    env["HTTPS"] = "";
    env["PATH"] = "/usr/local/bin:/usr/bin:/bin";
    env["QUERY_STRING"] = "param1=value1&param2=value2";
    env["REMOTE_ADDR"] = "192.168.1.1";
    env["REMOTE_HOST"] = "client.example.com";
    env["REMOTE_PORT"] = "8080";
    env["REMOTE_USER"] = "zmakhkha";
    env["REQUEST_METHOD"] = "GET";
    env["REQUEST_URI"] = "/www/page01.php";
    env["SCRIPT_FILENAME"] = "/var/www/html/www/page01.php";
    env["SCRIPT_NAME"] = "www/page01.php";
    env["SERVER_ADMIN"] = "zmakhkha@gmail.com";
    env["SERVER_NAME"] = "SA3DYA";
    env["SERVER_PORT"] = "9000";
    env["SERVER_SOFTWARE"] = "AMN/1.1 (AKA SA3DYA)";
    Cgi a;
    a.setEnvVars(env);
}