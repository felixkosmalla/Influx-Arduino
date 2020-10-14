#include <asyncHTTPrequest.h>
#include "InfluxArduino.h"


InfluxArduino::InfluxArduino()
{
     if( ! request) request = new asyncHTTPrequest;
      request->setTimeout(5);
      request->setDebug(false);
      

}

InfluxArduino::~InfluxArduino()
{
}

void InfluxArduino::configure(const char database[], const char host[], const uint16_t port)
{
    //copy these strings to private class pointers for future use

    _database = new char[strlen(database) + 1];
    strcpy(_database, database); //strncpy fails for some reason
    _host = new char[strlen(host) + 1];
    strcpy(_host, host);
    _port = port;
}

void InfluxArduino::addCertificate(const char cert[])
{
    //copy these strings to private class pointers for future use

    _cert = new char[strlen(cert) + 1];
    strcpy(_cert, cert);
    _isSecure = true;
}

void InfluxArduino::authorize(const char username[], const char password[])
{
    //copy these strings to private class pointers for future use

    _username = new char[strlen(username) + 1];
    strcpy(_username, username);
    _password = new char[strlen(password) + 1];
    strcpy(_password, password);
    _isAuthorised = true;
}

bool InfluxArduino::write(const char *measurement, const char *fieldString)
{
    return write(measurement, "", fieldString);
}

bool InfluxArduino::write(const char *measurement, const char *tagString, const char *fieldString)
{
   

    if (request->readyState() == 0 || request->readyState() == 4)
    {

        char uri[60];
        sprintf(uri, "/write?db=%s&u=%s&p=%s", _database, _username, _password);

        String url = String("http://") +String(_host) + String(":") + String(_port) + String(uri);

        char writeBuf[512]; // ¯\_(ツ)_/¯
        if (strlen(tagString) > 0)
        {
            sprintf(writeBuf, "%s,%s %s", measurement, tagString, fieldString); //no comma between tags and fields
        }

        else
        {
            //no tags
            sprintf(writeBuf, "%s %s", measurement, fieldString); //no comma between tags and fields
        }

        char url_str[100];
        url.toCharArray(url_str, 100);

        if(request->open("POST", url_str)){
            bool res = request->send(writeBuf);
        
        }
        
    }
    else
    {
        return false;
    }


    return true;
}

int InfluxArduino::getResponse()
{
    return _latestResponse;
}

bool InfluxArduino::isSecure()
{
    return _isSecure;
}