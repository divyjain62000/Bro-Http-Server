#include <iostream>
#include<map>
#include<forward_list>
using namespace std;

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
public:
    /**
     * @brief If there will be an error this method return true otherwise it return false
     * 
     * @return true 
     * @return false 
     */
    bool hasError()
    {
        return false;
    }

    /**
     * @brief Get the Error message
     * 
     * @return string 
     */
    string getError()
    {
        return "";
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
        this->contentLength=0;
        this->contentIterator=this->content.before_begin();
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
        this->contentLength+=content.length();
        this->contentIterator=this->content.insert_after(this->contentIterator,content);
        return *this;
    }
};

/**
 * @brief This class contains fields and methods related to our Bro HTTP Server
 * 
 */
class Bro
{
private:
    string staticResourcesFolder;
    map<string,void (*)(Request &,Response &)> urlMappings;
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
        if(Validator::isValidURLFormat(url))
        {
            urlMappings.insert(pair<string,void (*)(Request &,Response &)>(url,callBack));
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
        //do nothing right now
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