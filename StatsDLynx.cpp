/* 
 * File:   Lynx.cpp
 * Author: adam
 * 
 * Created on June 13, 2013, 9:28 PM
 */

#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sstream>

#include "StatsDLynx.h"

StatsDLynx::StatsDLynx(const std::string& host,const std::string& port) :
        Debug(false),
        m_host(host),
        m_port(port)
{
    
    initSocket();
    
}

StatsDLynx::StatsDLynx(const StatsDLynx& orig) {
}

StatsDLynx::~StatsDLynx() {
    close(m_io.fd);
    if(m_io.servinfo != NULL)
        freeaddrinfo(m_io.servinfo);
    
}

int StatsDLynx::initSocket(){
    int rv;
    memset(&m_io.hints, 0, sizeof m_io.hints);
    m_io.hints.ai_family = AF_UNSPEC;
    m_io.hints.ai_socktype = SOCK_DGRAM;
    m_io.servinfo = NULL;

    if ((rv = getaddrinfo(m_host.c_str(), m_port.c_str() , &m_io.hints, &m_io.servinfo)) != 0) {
        std::cerr << "StatsDLynx::initSocket: " << gai_strerror(rv) << std::endl;
        return 1;
    }

    for(m_io.p = m_io.servinfo; m_io.p != NULL; m_io.p = m_io.p->ai_next) {
        if ((m_io.fd = socket(m_io.p->ai_family, m_io.p->ai_socktype,
                m_io.p->ai_protocol)) == -1) {
            perror("StatsDLynx::initSocket: socket");
            continue;
        }
        break;
    }

    if (m_io.p == NULL) {
        std::cerr << "StatsDLynx::initSocket: failed to bind socket" << std::endl;
        return 2;
    }
    
    return 0;
}

int StatsDLynx::writeString(const std::string& str){
    
    if(Debug){
        std::cout << "StatsDLynx::writeString " << str << std::endl;
    }
    
    if ((sendto(m_io.fd, str.c_str(), str.size(), 0, m_io.p->ai_addr, m_io.p->ai_addrlen)) == -1) {
        return 1;
        perror("StatsDLynx::initSocket: sendto");
    }
    return 0;
}

bool StatsDLynx::validMetric(const std::string& metric){
    if(metric.size() == 0)
        return false;
    
    return true;
}


int StatsDLynx::increment(const std::string& metric) {
    if(!validMetric(metric))
        return 1;
    
    std::stringstream ss;
    ss << metric << ":1|c";
    return writeString(ss.str());
}

int StatsDLynx::decrement(const std::string& metric) {
    if(!validMetric(metric))
        return 1;
    
    std::stringstream ss;
    ss << metric << ":-1|c";
    return writeString(ss.str());
}

int StatsDLynx::timing(const std::string& metric, const unsigned int time) {
    if(!validMetric(metric))
        return 1;
    
    std::stringstream ss;
    ss << metric << ":" << time << "|ms";
    return writeString(ss.str());
}

int StatsDLynx::gauge(const std::string& metric, const int val) {
    if(!validMetric(metric))
        return 1;
    
    std::stringstream ss;
    ss << metric << ":" << val << "|g";
    return writeString(ss.str());
}

int StatsDLynx::set(const std::string& metric, const int val) {
    if(!validMetric(metric))
        return 1;
    
    std::stringstream ss;
    ss << metric << ":" << val << "|s";
    return writeString(ss.str());
}
