/* 
 * File:   GlobalDebug.h
 * Author: sriram
 *
 * Created on 20 April, 2014, 12:24 AM
 */

#ifndef GLOBALDEBUG_H
#define	GLOBALDEBUG_H
#include <stdio.h>
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// Console output/Debug operations that are useful across the project  ////
///////////////////////////////////////////////////////////////////////////
// Used for development purpose debugging
#define COMMON_DEBUG true
// Development purpose debug only related to parsing
#define PARSER_DEBUG true
// Simple debugging messages also may be visible to users
#define SIMPLE_DEBUG true
// Warning messages from all across the project
#define WARNING_DEBUG true
// Error messages from all across the project
#define ERROR_DEBUG true
// Debugging into File -- Usually formatted output
// for the developer to validate and verify large output
#define FILE_DEBUG true
// For checking the status of both physical and external memory
// Just a simple pause function for current status validation
#define MEMORY_DEBUG false

/// ofstream defined for the purpose of file debug
/// The file is created in the directory of project executable
std::ofstream outputStream;

#define DEBUG(x) \
if(COMMON_DEBUG == true){ \
        std::cout <<"COMMON MSG- File:"<< __FILE__ <<",Line:" <<__LINE__ <<",msg:"<< x << std::endl;\
        std::cout.flush();\
}

#define DEBUG_SIMPLE(x) \
if(SIMPLE_DEBUG == true){ \
        std::cout << "*** " << x << " ***"<<std::endl;\
        std::cout.flush();\
}

#define DEBUG_PARSER(x) \
if(PARSER_DEBUG == true){ \
	std::cout <<"PARSER MSG- File:"<< __FILE__ <<",Line:" <<__LINE__ <<",msg:"<< x << std::endl;\
        std::cout.flush();\
}

#define DEBUG_MEMORY(x) \
if(MEMORY_DEBUG == true){ \
        std::cout <<"MEMORY INFO- File:"<< __FILE__ <<",Line:" <<__LINE__ <<",msg:"<< x << std::endl;\
        std::cout <<"Press any key to continue"<<std::endl;\
        getchar();\
        std::cout <<"Please Wait!!!"<<std::endl;\
        std::cout.flush();\
}

#define DEBUG_ERROR(x) \
if(ERROR_DEBUG == true){ \
        std::cout <<"GENERAL ERR- File:"<< __FILE__ <<",Line:" <<__LINE__ <<",msg:"<< x << std::endl;\
        std::cout.flush();\
}

#define DEBUG_WARNING(x) \
if(WARNING_DEBUG == true){ \
        std::cout <<"GENERAL WRN- File:"<< __FILE__ <<",Line:" <<__LINE__ <<",msg:"<< x << std::endl;\
        std::cout.flush();\
}


/*        outputStream<<__FILE__ <<",Line:" <<__LINE__ <<",msg:"<< x << std::endl;\*/
#define DEBUG_FILE(x) \
if(FILE_DEBUG == true){ \
        if(!outputStream.is_open()){ \
                outputStream.open("debug_log.txt", std::ios::app);\
        }\
        outputStream<<x<<std::endl;\
        outputStream.flush();\
}
char* get_current_time_stamp() {
    // current date/time based on current system
    time_t now = time(0);
    // convert now to string form
    char* dt = ctime(&now);
    dt[strlen(dt)-1] = '\0';
    return dt;
}

#define DEBUG_FILE_WITH_TIMESTAMP(x) \
if(FILE_DEBUG == true){ \
        if(!outputStream.is_open()){ \
                outputStream.open("debug_log.txt", std::ios::app);\
        }\
        outputStream << "[ " << get_current_time_stamp() << " ]: " << x << std::endl;\
        outputStream.flush();\
}


#define CONSOLE_PRINT(x) \
std::cout << x << std::endl;\
std::cout.flush();

#define CONSOLE_PRINTLN(x) \
std::cout << x << std::endl;\
std::cout.flush();


#endif	/* GLOBALDEBUG_H */