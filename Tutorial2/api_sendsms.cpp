// g++ -o api_sendsms api_sendsms.cpp -lcurl -lcurlpp -Wall

#include <unistd.h>
#include <iostream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

#include "../include/json.hpp"

// Functions prototype
void login();
void logout(std::string& access_token);
void sendsms(std::string& access_token, std::string receiver, std::string& message);
long unsigned int get_access_token(char* ptr, long unsigned int size, long unsigned int nmemb);

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

    std::cout << std::endl << "Attempting to login to Sendplex ..." << std::endl << std::endl;

    header.push_back("Content-Type: application/x-www-form-urlencoded");
    request.setOpt(new curlpp::options::Url("https://api.sendplex.io/login"));
    request.setOpt(new curlpp::options::Verbose(true));
    request.setOpt(new curlpp::options::HttpHeader(header));
    request.setOpt(new curlpp::options::PostFields(email_password));
    request.setOpt(new curlpp::options::PostFieldSize(email_password.length()));
    request.setOpt(call_back);
    request.perform();
}

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

    std::cout << std::endl << std::endl << "Successfully logout of Sendplex!" << std::endl << std::endl;

    access_token = "";
}

void sendsms(std::string& access_token, std::string receiver, std::string& message)
{
    curlpp::Cleanup cleaner;
    curlpp::Easy request;
    std::list<std::string> header;
    std::string body = "to=" + receiver + "&message=" + message;

    header.push_back("Content-Type: application/x-www-form-urlencoded");
    header.push_back("Authorization: Bearer " + access_token);
    request.setOpt(new curlpp::options::Url("https://api.sendplex.io/sendsms"));
    request.setOpt(new curlpp::options::Verbose(true));
    request.setOpt(new curlpp::options::HttpHeader(header));
    request.setOpt(new curlpp::options::PostFields(body));
    request.setOpt(new curlpp::options::PostFieldSize(body.length()));
    request.perform();
}

long unsigned int get_access_token(char* ptr, long unsigned int size, long unsigned int nmemb)
{
    nlohmann::json json_doc;
    std::string access_token = "";
    std::string receiver = "";
    std::string message = "";

    json_doc = nlohmann::json::parse(ptr);

    std::cout << std::endl << json_doc.dump(4) << std::endl;

    access_token = json_doc["access_token"].get<std::string>();

    std::cout << std::endl << "Successfully login to Sendplex!" << std::endl << std::endl;

    std::cout << "receiver phone number: ";
    std::cin >> receiver;
    std::cout << "message: ";
    std::cin >> message;
    std::cout << std::endl;

    sendsms(access_token, receiver, message);

    logout(access_token);

    return size * nmemb;
};
