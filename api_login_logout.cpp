#include <iostream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

#include "json.hpp"

void logout(std::string& access_token)
{
    curlpp::Cleanup cleaner;
    curlpp::Easy request;
    std::list<std::string> header;

    header.push_back("Content-Type: application/json");
    header.push_back("Authorization: Bearer " + access_token);
    request.setOpt(new curlpp::options::Url("https://api.sendplex.io/logout"));
    request.setOpt(new curlpp::options::Verbose(true));
    request.setOpt(new curlpp::options::HttpHeader(header));
    request.perform();
}

long unsigned int get_access_token(char* ptr, long unsigned int size, long unsigned int nmemb)
{
    nlohmann::json json_doc;
    std::string access_token;

    json_doc = nlohmann::json::parse(ptr);

    access_token = json_doc["access_token"].get<std::string>();

    logout(access_token);

    return size * nmemb;
};

void login()
{
    curlpp::Cleanup cleaner;
    curlpp::Easy request;
    curlpp::types::WriteFunctionFunctor functor(get_access_token);
    curlpp::options::WriteFunction* call_back = new curlpp::options::WriteFunction(functor);
    std::list<std::string> header;
    std::string email;
    std::string password;
    std::string email_password;

    std::cout << "email: ";
    std::cin >> email;
    std::cout << "password:";
    std::cin >> password;

    email_password = "email=" + email + "&password=" + password;

    header.push_back("Content-Type: application/x-www-form-urlencoded");
    request.setOpt(new curlpp::options::Url("https://api.sendplex.io/login"));
    request.setOpt(new curlpp::options::Verbose(true));
    request.setOpt(new curlpp::options::HttpHeader(header));
    request.setOpt(new curlpp::options::PostFields(email_password));
    request.setOpt(new curlpp::options::PostFieldSize(email_password.length()));
    request.setOpt(call_back);
    request.perform();
}

int main() 
{
    try
    {
        login();
    }
    catch (curlpp::LogicError& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (curlpp::RuntimeError& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
