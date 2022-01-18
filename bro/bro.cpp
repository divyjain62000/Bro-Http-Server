#include <iostream>
#include <map>
#include <forward_list>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

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
 * @brief Content file system related functionalitites
 * 
 */
class FileSystemUtility
{
private:
    FileSystemUtility() {}

public:
    /**
     * @brief Check wether file exits or not
     * 
     * @param path 
     * @return true 
     * @return false 
     */
    static bool fileExists(const char *path)
    {
        struct stat s;
        int x;
        x = stat(path, &s);
        if (x != 0)
            return false;
        if (s.st_mode & S_IFDIR)
            return false;
        return true;
    }

    /**
     * @brief Check wether directory exists or not
     * 
     * @param path 
     * @return true 
     * @return false 
     */
    static bool directoryExists(const char *path)
    {
        struct stat s;
        int x;
        x = stat(path, &s);
        if (x != 0)
            return false;
        if (s.st_mode & S_IFDIR)
            return true;
        return false;
    }
};

/**
 * @brief Contains string operations related functionality
 * 
 */
class StringUtility
{
private:
    StringUtility();

public:
    /**
 * @brief Convert each character of string to lower case
 * 
 * @param str 
 */
    static void toLowerCase(char *str)
    {
        if (str == NULL)
            return;
        for (; *str; str++)
            if (*str >= 65 && *str <= 90)
                *str += 32;
    }
};

/**
 * @brief Contains Http error status related functionality
 * 
 */
class HttpErrorStatusUtility
{
private:
    HttpErrorStatusUtility();

public:
    static void sendBadRequestError(int clientSocketDescriptor)
    {
        //will complete later on
    }
    static void sendHttpVersionNotSupportedError(int clientSocketDescriptor, char *version)
    {
        //will complete later on
    }
    static void sendNotFoundError(int clientSocketDescriptor, char *requestURI)
    {
        //we will optimize this code later on
        char content[1000];
        char header[200];
        char response[1200];
        sprintf(content, "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>404 Error</title></head><body>Requested Resource [%s] Not Found</body></html>", requestURI);
        int contentLength = strlen(content);
        sprintf(header, "HTTP/1.1 404 Not Found\r\nContent-Type:text/html\nContent-Length:%d\nConnection: close\r\n\r\n", contentLength);
        strcpy(response, header);
        strcat(response, content);
        send(clientSocketDescriptor, response, strlen(response), 0);
    }
    static void sendMethodNotAllowedError(int clientSocketDescriptor, char *method, char *requestURI)
    {
        //will complete later on
    }
};

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
        return FileSystemUtility::directoryExists(path.c_str());
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
private:
    char *method;
    char *requestURI;
    char *httpVersion;
    Request(char *method, char *requestURI, char *httpVersion)
    {
        this->method = method;
        this->requestURI = requestURI;
        this->httpVersion = httpVersion;
    }
    friend class Bro;
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
    friend class HttpResponseUtility;
};

/**
 * @brief Contains method that send http responses
 * 
 */
class HttpResponseUtility
{
private:
    HttpResponseUtility();

public:
    void static sendResponse(int clientSocketDescriptor, Response &response)
    {
        char header[200];
        int contentLength = response.contentLength;
        sprintf(header, "HTTP/1.1 404 Not Found\r\nContent-Type:text/html\nContent-Length:%d\nConnection: close\r\n\r\n", contentLength);
        send(clientSocketDescriptor, header, strlen(header), 0);
        auto contentIterator = response.content.begin();
        while (contentIterator != response.content.end())
        {
            string str = *contentIterator;
            send(clientSocketDescriptor, str.c_str(), str.length(), 0);
            ++contentIterator;
        }
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
            string exception = string("Invalid static resource folder path : ") + staticResourcesFolder;
            throw exception;
        }
    }


    bool serveStaticResource(int clientSocketDescriptor,const char *requestURI)
    {
        if(this->staticResourcesFolder.length()==0) return false;
        if(!FileSystemUtility::directoryExists(this->staticResourcesFolder.c_str())) return false;
        string resourcePath=this->staticResourcesFolder+string(requestURI);
        cout<<"Static resource path is "<<resourcePath<<endl;
        if(!FileSystemUtility::fileExists(resourcePath.c_str())) return false;
        FILE *file=fopen(resourcePath.c_str(),"rb");
        if(file==NULL) return false;
        long fileSize;
        fseek(file,0,SEEK_END);
        fileSize=ftell(file);
        if(fileSize==0)
        {
            fclose(file);
            return false;
        }
        rewind(file); //to move the internal file pointer to the start of the file

        char header[200];
        sprintf(header,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %ld\r\nConnection: close\r\n\r\n",fileSize);
        send(clientSocketDescriptor,header,strlen(header),0);
        
        long bytesLeftToRead;
        int bytesToRead;
        char buffer[4096];
        bytesToRead=4096;
        bytesLeftToRead=fileSize;
        while(bytesLeftToRead>0)
        {
            if(bytesLeftToRead<bytesToRead) bytesToRead=bytesLeftToRead;
            fread(buffer,bytesToRead,1,file);
            if(feof(file)) break; //this won't happen in our case
            send(clientSocketDescriptor,buffer,bytesToRead,0);
            bytesLeftToRead=bytesLeftToRead-bytesToRead;
        }
        fclose(file);
        return true;
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
            urlMappings.insert(pair<string, URLMapping>(url, {__GET__, callBack}));
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
            requestLength = recv(clientSocketDescriptor, requestBuffer, sizeof(requestBuffer) - sizeof(char), 0);
            if (requestLength == 0 || requestLength == -1)
            {
                close(clientSocketDescriptor);
                continue;
            }
            int i;
            char *method, *requestURI, *httpVersion;
            requestBuffer[requestLength] = '\0';
            // code to parse the first line of the http request starts here
            // first line should be REQUEST_METHOD SPACE URI SPACE HTTPVersionCRLF
            method = requestBuffer;
            i = 0;
            while (requestBuffer[i] && requestBuffer[i] != ' ')
                i++;
            //If there is BAD Request
            if (requestBuffer[i] == '\0')
            {
                HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
                close(clientSocketDescriptor);
                continue;
            }
            requestBuffer[i] = '\0';
            i++;
            if (requestBuffer[i] == ' ' || requestBuffer[i] == '\0')
            {
                HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
                close(clientSocketDescriptor);
                continue;
            }
            StringUtility::toLowerCase(method);

            if (!(strcmp(method, "get") == 0 || strcmp(method, "post") == 0 || strcmp(method, "put") == 0 ||
                  strcmp(method, "delete") == 0 || strcmp(method, "head") == 0 || strcmp(method, "options") == 0 ||
                  strcmp(method, "trace") == 0 || strcmp(method, "connect") == 0))
            {
                HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
                close(clientSocketDescriptor);
                continue;
            }
            requestURI = requestBuffer + i;
            while (requestBuffer[i] && requestBuffer[i] != ' ')
                i++;
            //If there is BAD Request
            if (requestBuffer[i] == '\0')
            {
                HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
                close(clientSocketDescriptor);
                continue;
            }
            requestBuffer[i] = '\0';
            i++;
            //If there is BAD Request
            if (requestBuffer[i] == '\0')
            {
                HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
                close(clientSocketDescriptor);
                continue;
            }
            httpVersion = requestBuffer + i;
            while (requestBuffer[i] && requestBuffer[i] != '\r' && requestBuffer[i] != '\n')
                i++;
            //If there is BAD Request
            if (requestBuffer[i] == '\0')
            {
                HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
                close(clientSocketDescriptor);
                continue;
            }
            if (requestBuffer[i] == '\r' && requestBuffer[i + 1] != '\n')
            {
                HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
                close(clientSocketDescriptor);
                continue;
            }
            if (requestBuffer[i] == '\r')
            {
                requestBuffer[i] = '\0';
                i = i + 2;
            }
            else
            {
                requestBuffer[i] = '\0';
                i = i + 1;
            }
            StringUtility::toLowerCase(httpVersion);
            if (strcmp(httpVersion, "http/1.1") != 0)
            {
                HttpErrorStatusUtility::sendHttpVersionNotSupportedError(clientSocketDescriptor, httpVersion);
                close(clientSocketDescriptor);
                continue;
            }
            cout << "Request arrived, uri is " << requestURI << endl;
            auto urlMappingsIterator = urlMappings.find(requestURI);
            if (urlMappingsIterator == urlMappings.end())
            {
                if (!serveStaticResource(clientSocketDescriptor, requestURI))
                {
                    HttpErrorStatusUtility::sendNotFoundError(clientSocketDescriptor, requestURI);
                }
                close(clientSocketDescriptor);
                continue;
            }
            URLMapping urlMapping = urlMappingsIterator->second;
            if (urlMapping.requestMethod == __GET__ and strcmp(method, "get") != 0)
            {
                HttpErrorStatusUtility::sendMethodNotAllowedError(clientSocketDescriptor, method, requestURI);
                close(clientSocketDescriptor);
                continue;
            }
            // code to parse the header and then the payload if exists starts here
            // code to parse the header and then the payload if exists ends here
            Request request(method, requestURI, httpVersion);
            Response response;
            urlMapping.mappedFunction(request, response);
            HttpResponseUtility::sendResponse(clientSocketDescriptor, response);

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
    try
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
    }
    catch (string exception)
    {
        cout << exception << endl;
    }
    return 0;
}