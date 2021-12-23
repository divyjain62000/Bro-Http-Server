#include <iostream>
#include <map>
#include <forward_list>
#include <string.h>
#include <unistd.h>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef linux
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

using namespace std;

//Defining error related macros
#define ERR_CREATING_SOCKET "Unable to create socket"
#define ERR_BINDING_SOCKET_TO_PORT "Unable to bind socket to port : "
#define ERR_ACCEPT_CLIENT_CONNECTION "Unable to accept client connection"

//Defining socket binding and related things macros
#define QUEUE_SIZE 10

//Global Variables
int x;

// Amit [ The Bro Programmer ]

/**
 * @brief Contains all methods related to validation
 * 
 */
class Validator
{
private:
    Validator() {}

public:
    /**
     * @brief To validate MIME Type
     * 
     * @param mimeType 
     * @return true 
     * @return false 
     */
    static bool isValidMIMEType(string &mimeType)
    {
        //right now do nothing
        return true;
    }

    /**
     * @brief To validate path
     * 
     * @param path 
     * @return true 
     * @return false 
     */
    static bool isValidPath(string &path)
    {
        //right now do nothing
        return true;
    }

    /**
     * @brief To validate URL format
     * 
     * @return true 
     * @return false 
     */
    static bool isValidURLFormat(string url)
    {
        //right now do nothing
        return true;
    }
};

/**
 * @brief This class contains all fields and methods related to error
 * 
 */
class Error
{
private:
    string error;

public:
    /**
     * @brief Construct a new Error object
     * 
     * @param error 
     */
    Error(string error)
    {
        this->error = error;
    }

    /**
     * @brief If there will be an error this method return true otherwise it return false
     * 
     * @return true 
     * @return false 
     */
    bool hasError()
    {
        return this->error.length() > 0;
    }

    /**
     * @brief Get the Error message
     * 
     * @return string 
     */
    string getError()
    {
        return this->error;
    }
};

/**
 * @brief This class contains fields and methods related to Request
 * 
 */
class Request
{
};

/**
 * @brief This class contains fields and methods related to Response
 * 
 */
class Response
{
private:
    string contentType;
    forward_list<string> content;
    unsigned long contentLength;
    forward_list<string>::iterator contentIterator;

public:
    /**
     * @brief Construct a new Response object
     * 
     */
    Response()
    {
        this->contentLength = 0;
        this->contentIterator = this->content.before_begin();
    }

    /**
     * @brief Destroy the Response object
     * 
     */
    ~Response()
    {
        //not yet decided
    }

    /**
     * @brief Set the Content Type
     * 
     * @param contentType 
     */
    void setContentType(string contentType)
    {
        if (Validator::isValidMIMEType(contentType))
            this->contentType = contentType;
    }

    /**
     * @brief Here we overload << operator, We will set content by using this operator
     * 
     * @param content 
     * @return Response& 
     */
    Response &operator<<(string content)
    {
        this->contentLength += content.length();
        this->contentIterator = this->content.insert_after(this->contentIterator, content);
        return *this;
    }
};

/**
 * @brief enum of request methods
 * 
 */
enum __request_method__
{
    __GET__,
    __POST__,
    __PUT__,
    __DELETE__,
    __CONNECT__,
    __HEAD__,
    __OPTIONS__,
    __TRACE__
};

/**
 * @brief structure of url mapping
 * 
 */
typedef struct __url__mapping
{
    __request_method__ requestMethod;
    void (*mappedFunction)(Request &, Response &);
} URLMapping;

/**
 * @brief This class contains fields and methods related to our Bro HTTP Server
 * 
 */
class Bro
{
private:
    string staticResourcesFolder;
    map<string, URLMapping> urlMappings;

public:
    /**
     * @brief Construct a new Bro object
     * 
     */
    Bro()
    {
    }

    /**
     * @brief Destroy the Bro object
     * 
     */
    ~Bro()
    {
    }

    /**
     * @brief Set the Static Resources Folder
     * 
     * @param staticResourcesFolder 
     */
    void setStaticResourcesFolder(string staticResourcesFolder)
    {
        if (Validator::isValidPath(staticResourcesFolder))
        {
            this->staticResourcesFolder = staticResourcesFolder;
        }
        else
        {
            //not yet decided
        }
    }

    /**
     * @brief To handle GET Type request with particular urlPattern
     * 
     * @param url
     * @param callBack 
     */
    void get(string url, void (*callBack)(Request &, Response &))
    {
        if (Validator::isValidURLFormat(url))
        {
            urlMappings.insert(pair<string, URLMapping>(url,{__GET__,callBack}));
        }
    }

    /**
     * @brief To set server at particular port number to accept request
     * 
     * @param portNumber 
     * @param callBack 
     */
    void listen(int portNumber, void (*callBack)(Error &))
    {

#ifdef _WIN32
        WSADATA wsaData;
        WORD ver;
        ver = MAKEWORD(1, 1);
        WSAStartup(ver, &wsaData);
#endif

        char requestBuffer[4097]; //Instead of 4096 we make array of 4097 because we have to add \0 by ourself that's why we need one extra space
        int requestLength;

        int serverSocketDescriptor;
        serverSocketDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocketDescriptor < 0)
        {
#ifdef _WIN32
            WSACleanup();
#endif
            Error error(ERR_CREATING_SOCKET);
            callBack(error);
            return;
        }
        struct sockaddr_in serverSocketInformation;
        serverSocketInformation.sin_family = AF_INET;
        serverSocketInformation.sin_port = htons(portNumber);
        serverSocketInformation.sin_addr.s_addr = htonl(INADDR_ANY);
        int successCode = bind(serverSocketDescriptor, (struct sockaddr *)&serverSocketInformation, sizeof(serverSocketInformation));
        if (successCode < 0)
        {
            close(serverSocketDescriptor);
#ifdef _WIN32
            WSACleanup();
#endif
            string err = ERR_BINDING_SOCKET_TO_PORT + to_string(portNumber);
            Error error(err);
            callBack(error);
            return;
        }
        successCode = ::listen(serverSocketDescriptor, QUEUE_SIZE);
        if (successCode < 0)
        {
            close(serverSocketDescriptor);
#ifdef _WIN32
            WSACleanup();
#endif
            Error error(ERR_ACCEPT_CLIENT_CONNECTION);
            callBack(error);
            return;
        }

        /*
            If execution of program reach at that point server is able to go in listen 
            mode So we make error object with empty string so whatever user want to 
            print when connection created successfully will be able to print
        */
        Error error("");
        callBack(error);

        struct sockaddr_in clientSocketInformation;
#ifdef linux
        socklen_t len = sizeof(clientSocketInformation);
#endif
#ifdef _WIN32
        int len = sizeof(clientSocketInformation);
#endif

        int clientSocketDescriptor;
        while (1)
        {
            clientSocketDescriptor = accept(serverSocketDescriptor, (struct sockaddr *)&clientSocketInformation, &len);
            requestLength=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer)-sizeof(char),0);
            if(requestLength==0 || requestLength==-1)
            {
                close(clientSocketDescriptor);
                continue;
            }
            requestBuffer[requestLength]='\0';
            // code to parse the first line of the http request starts here
            // first line should be REQUEST_METHOD SPACE URI SPACE HTTPVERSIONCrlf

            //done done 33:43

            close(clientSocketDescriptor);
            //lot of code go here
        } //infinite loop
#ifdef _WIN32
        WSACleanup();
#endif
    }
};

// Bobby [ The web Application Developer - User of Bro web server ]
int main()
{
    Bro bro;
    bro.setStaticResourcesFolder("whatever");

    /**
     * @brief If request is of GET type and url is / (i.e no resource name has been specified) then this function should get executed
     * 
     */
    bro.get("/", [](Request &request, Response &response) -> void
            {
                const char *html = R""""(
            <!DOCTYOE HTML>
            <html lang='en'>
            <head>
            <meta charset='utf-8'>
            <title>whatever</title>
            </head>
            <body>
            <h1>Welcome</h1>
            <h3>Administrator</h3>
            <a href='getCustomers'>Customers List</a>
            </body>
            </html>
        )"""";
                response.setContentType("text/html"); // Setting MIME type
                response << html;
            });

    /**
     * @brief If request is of GET type and url is /getCustomers (i.e resource name has been specified) then this function should get executed
     * 
     */
    bro.get("/getCustomers", [](Request &request, Response &response) -> void
            {
                const char *html = R""""(
            <!DOCTYPE HTML>
            <html lang='en'>
            <head>
            <meta charset='utf-8'>
            <title>whatever</title>
            </head>
            <body>
            <h1>List Of Customers</h1>
            <ul>
            <li>Ramesh</li>
            <li>Suresh</li>
            <li>Mohan</li>
            </ul>
            <a href='/'>Home</a>
            </body>
            </html>
        )"""";
                response.setContentType("text/html"); // Setting MIME type
                response << html;
            });

    /**
     * @brief Here we specify port number on which Bro HTTP server accept request
     * 
     */
    bro.listen(6060, [](Error &error) -> void
               {
                   if (error.hasError())
                   {
                       cout << error.getError();
                       return;
                   }
                   cout << "Bro HTTP server is ready to accept request on port 6060" << endl;
               });
    return 0;
}