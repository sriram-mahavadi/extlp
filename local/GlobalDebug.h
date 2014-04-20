/* 
 * File:   GlobalDebug.h
 * Author: sriram
 *
 * Created on 20 April, 2014, 12:24 AM
 */

#ifndef GLOBALDEBUG_H
#define	GLOBALDEBUG_H

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

#define DEBUG_FILE(x) \
if(FILE_DEBUG == true){ \
        if(!outputStream.is_open()){ \
                outputStream.open("debug_log.txt", ios::app);\
        }\
        outputStream<<__FILE__ <<",Line:" <<__LINE__ <<",msg:"<< x << std::endl;\
        outputStream.flush();\
}
#endif	/* GLOBALDEBUG_H */

