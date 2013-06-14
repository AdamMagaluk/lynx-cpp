/* 
 * File:   Lynx.h
 * Author: Adam Magaluk <AdamMagaluk@gmail.com>
 *
 * Created on June 13, 2013, 9:28 PM
 */

#ifndef STATSD_LYNX_H
#define	STATSD_LYNX_H

#include <string>


#include <netdb.h>

#define DEFAULT_STATSD_PORT "8125"

class StatsDLynx {
public:
    StatsDLynx(const std::string& host,const std::string& port = DEFAULT_STATSD_PORT);
    virtual ~StatsDLynx();
    
    int increment(const std::string& metric);
    int decrement(const std::string& metric);
    int timing(const std::string& metric,const unsigned int time);
    int gauge(const std::string& metric,const int val);
    int set(const std::string& metric,const int val);
    
    bool Debug;
    
protected:
    StatsDLynx(const StatsDLynx& orig);
    
    // StatsD server
    const std::string m_host;
    
    // StatsD Port
    const std::string m_port;
    
    
    struct {
        int fd;
        struct addrinfo hints, *servinfo, *p;
        bool init;
    }m_io;
    
    int initSocket();
    
    bool validMetric(const std::string& metric);
    
    int writeString(const std::string& str);
    
};

#endif	/* LYNX_H */

