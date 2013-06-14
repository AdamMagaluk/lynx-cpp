/* 
 * File:   example.cpp
 * Author: adam
 *
 * Created on June 13, 2013, 9:39 PM
 */

#include <cstdlib>
#include <iostream>

#include "StatsDLynx.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    if(argc < 2){
        std::cerr << "Usage: <host>" << std::endl;
        return 1;
    }
    
    StatsDLynx metrics(argv[1]);
    metrics.Debug = true;

    metrics.increment("node_test.int");
    metrics.decrement("node_test.int");
    metrics.timing("node_test.some_service.task.time", 500); // time in ms
    metrics.gauge("gauge.one", 100);
    metrics.set("set.one", 10);
    
    return 0;
}

