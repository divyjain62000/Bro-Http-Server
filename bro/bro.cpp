#include <iostream>
using namespace std;

// Amit [ The Bro Programmer ]

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
public:
    /**
     * @brief Set the Content Type
     * 
     * @param contentType 
     */
    void setContentType(string contentType)
    {
        //do nothing right now
    }

    /**
     * @brief Here we overload << operator, We will set content by using this operator
     * 
     * @param content 
     * @return Response& 
     */
    Response &operator<<(string content)
    {
        //right now do nothing
        return *this;
    }
};

/**
 * @brief This class contains fields and methods related to our Bro HTTP Server
 * 
 */
class Bro
{
public:
    /**
     * @brief Set the Static Resources Folder
     * 
     * @param staticResourcesFolder 
     */
    void setStaticResourcesFolder(string staticResourcesFolder)
    {
        //do nothing right now
    }

    /**
     * @brief To handle GET Type request with particular urlPattern
     * 
     * @param urlPattern 
     * @param callBack 
     */
    void get(string urlPattern, void (*callBack)(Request &, Response &))
    {
        //do nothing right now
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
    bro.get("/", [](Request &request, Response &response) void
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
    bro.get("/getCustomers", [](Request &request, Response &response) void
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
    bro.listen(6060, [](Error &error) void
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