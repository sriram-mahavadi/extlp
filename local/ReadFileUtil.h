/* 
 * File:   ReadFileUtil.h
 * Author: sriram
 *
 * Created on 6 April, 2014, 1:34 AM
 */

#ifndef READFILEUTIL_H
#define	READFILEUTIL_H
#include "ReadMPSUtil.h"
#include "ReadLPUtil.h"
#include "GlobalDefines.h"
/// Util class for reading input LP Problem file (accepts both LP and mps format)
class ReadFileUtil {
    /// Identifies the input file type and performs it's 
    /// corresponding read
public:
    bool readFile(const char *filename) {
        std::ifstream file(filename);
        DEBUG_SIMPLE( "Reading File: "<<filename );
        if (!file) {
            DEBUG_PARSER( "File Not present" );
            return false;
        }
        char c;

        file.get(c);
        file.putback(c);
        std::vector<std::string> rowNames, colNames;
        /* MPS starts either with a comment mark '*' or with the keyword
         * 'NAME' at the first column.
         * LPF starts either with blanks, a comment mark '\' or with
         * the keyword "MAX" or "MIN" in upper or lower case.
         * There is no possible valid LPF file starting with a '*' or 'N'.
         */
        ReadMPSUtil mpsReader;
        ReadLPUtil lpReader;
        bool ok = ((c == '*') || (c == 'N'))
                ? mpsReader.readMPS(file, rowNames, colNames)
                : lpReader.readLPF(file, rowNames, colNames);
        return ok;
    }
};

#endif	/* READFILEUTIL_H */

