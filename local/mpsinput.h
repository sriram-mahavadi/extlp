/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                  This file is part of the class library                   */
/*       SoPlex --- the Sequential object-oriented simPlex.                  */
/*                                                                           */
/*    Copyright (C) 1996-2013 Konrad-Zuse-Zentrum                            */
/*                            fuer Informationstechnik Berlin                */
/*    Copyright (C) 2014      Sriram Mahavadi                                */
/*                            IIIT Bangalore                                 */
/*                                                                           */
/*  SoPlex is distributed under the terms of the ZIB Academic Licence.       */
/*                                                                           */
/*  You should have received a copy of the ZIB Academic License              */
/*  along with SoPlex; see the file COPYING.                                 */
/*  If not email to sriram.mahavadi@gmail.com                                */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**@file  mpsinput.h
 * @brief Read MPS format files.
 */
#ifndef _MPSINPUT_H_
#define _MPSINPUT_H_

#include <iostream>

//#include "spxlp.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>

//#include "spxdefines.h"
#include "mpsinput.h"
#include "GlobalDefines.h"
//#include "spxout.h"


#define PATCH_CHAR    '_'
#define BLANK         ' '
/**@class MPSInput
   
   Reads MPS input files. A right-hand side for the objective function is
   allowed but ignored.
 */
class MPSInput {
public:

    //-----------------------------------
    /**@name Types */
    //@{
    ///
    enum Section {
        NAME, OBJSEN, OBJNAME, ROWS, COLUMNS, RHS, RANGES, BOUNDS, ENDATA
    };
    ///
    enum {
        MAX_LINE_LEN = 256
    };
    //@}

private:

    //-----------------------------------
    /**@name Private data */
    //@{
    ///
    Section m_section;
    /// the input stream from which the file is read
    std::istream& m_input;
    /// line number
    int m_lineno;
    /// objctive sense (maximization or minimization)
    //   SPxLP::SPxSense m_objsense;
    bool m_objsense;
    /// is set to \c true upon a syntax error
    bool m_has_error;
    /// the line buffer
    char m_buf[MAX_LINE_LEN];
    /// first field in a line
    const char* m_f0;
    /// second field in a line
    const char* m_f1;
    /// third field in a line
    const char* m_f2;
    /// fourth field in a line
    const char* m_f3;
    /// fifth field in a line
    const char* m_f4;
    /// sixth field in a line
    const char* m_f5;
    /// problem name
    char m_probname[MAX_LINE_LEN];
    /// objective name
    char m_objname [MAX_LINE_LEN];
    /// rhs column name
    char m_rhsname [MAX_LINE_LEN];
    ///
    bool m_is_integer;
    /// new MPS format?
    bool m_is_new_format;
    /// Number of already ignored entries.
    int m_ignored;
    /// Maximal number ofp ignored entries for which a warning will be issued.
    static const int m_max_ignore = 1000;
    //@}

public:

    //-----------------------------------
    /**@name Construction / destruction */
    //@{
    ///
    explicit
    MPSInput(std::istream& p_input)
    : m_section(NAME)
    , m_input(p_input)
    , m_lineno(0)
    , m_objsense(false/*SPxLP::MINIMIZE*/)
    , m_has_error(false)
    , m_is_integer(false)
    , m_is_new_format(false)
    , m_ignored(0) {
        m_f0 = m_f1 = m_f2 = m_f3 = m_f4 = m_f5 = 0;

        m_buf [0] = '\0';
        m_probname[0] = '\0';
        m_objname [0] = '\0';
    }
    ///
    virtual
    ~MPSInput() {
        // only to signal to flexelint that the pointers do
        // not point to anything that has to be freed.
        m_f0 = m_f1 = m_f2 = m_f3 = m_f4 = m_f5 = 0;
    }
    //@}

    //-----------------------------------
    /**@name Access */
    //@{
    ///
    Section section() const {
        return m_section;
    }
    ///
    int lineno() const {
        return m_lineno;
    }
    ///
    const char* field0() const {
        return m_f0;
    }
    ///
    const char* field1() const {
        return m_f1;
    }
    ///
    const char* field2() const {
        return m_f2;
    }
    ///
    const char* field3() const {
        return m_f3;
    }
    ///
    const char* field4() const {
        return m_f4;
    }
    ///
    const char* field5() const {
        return m_f5;
    }
    ///
    const char* probName() const {
        return m_probname;
    }
    ///
    const char* objName() const {
        return m_objname;
    }
    ///
    bool objSense() const {
        return m_objsense;
    }
    ///
    bool hasError() const {
        return m_has_error;
    }
    ///
    bool isInteger() const {
        return m_is_integer;
    }
    ///
    const char* rhsName() const {
        return m_rhsname;
    }
    //@}

    //-----------------------------------
    /**@name Modification */
    //@{
    ///
    void setSection(Section p_section) {
        m_section = p_section;
    }
    ///
    void setProbName(const char* p_probname) {
        assert(strlen(p_probname) < MAX_LINE_LEN);
        strcpy(m_probname, p_probname);
    }
    ///
    void setObjName(const char* p_objname) {
        assert(strlen(p_objname) < MAX_LINE_LEN);
        strcpy(m_objname, p_objname);
    }
    ///
    void setObjSense(bool sense/*SPxLP::SPxSense sense*/) {
        m_objsense = sense;
    }
    ///
    void setRhsName(const char* p_rhsname) {
        assert(strlen(p_rhsname) < MAX_LINE_LEN);
        strcpy(m_rhsname, p_rhsname);
    }
    //@}

    //-----------------------------------
    /**@name Warnings and Errors */
    //@{
    ///
    void syntaxError() {
        //      MSG_ERROR( spxout << "Syntax error in line " << m_lineno << std::endl; )
        m_section = ENDATA;
        m_has_error = true;
        exit(1);
    }
    ///
    void entryIgnored(
            const char* what, const char* what_name,
            const char* entity, const char* entity_name) {
        if (m_ignored < m_max_ignore) {
            DEBUG_SIMPLE("Warning: line " << m_lineno << ": "
                    << what << " \"" << what_name << "\""
                    << " for " << entity << " \""
                    << entity_name << "\" ignored");
            ++m_ignored;
        }
        if (m_ignored == m_max_ignore) {
            DEBUG_SIMPLE("Warning: This was the " << m_max_ignore << " ignored entry. No further warnings on "
                    << "ignored entries will be given." << std::endl)
        }
    }
    //@}

    //-----------------------------------
    /**@name Helpers */
    //@{

    /// fill the line from \p pos up to column 80 with blanks.
    void clear_from(char* buf, int pos) {
        for (int i = pos; i < 80; i++)
            buf[i] = BLANK;
        buf[80] = '\0';
    }

    /// change all blanks inside a field to #PATCH_CHAR.
    void patch_field(char* buf, int beg, int end) {
        while ((beg <= end) && (buf[end] == BLANK))
            end--;

        while ((beg <= end) && (buf[beg] == BLANK))
            beg++;

        for (int i = beg; i <= end; i++)
            if (buf[i] == BLANK)
                buf[i] = PATCH_CHAR;
    }

    /// read a MPS format data line and parse the fields.
    bool readLine() {
        int len;
        int space;
        char* s;
        bool is_marker = false;

        do {
            m_f0 = m_f1 = m_f2 = m_f3 = m_f4 = m_f5 = 0;
            is_marker = false;

            // Read until we have a not comment line.
            do {
                if (m_input.getline(m_buf, sizeof (m_buf)) == 0)
                    return false;
                m_lineno++;

                //        MSG_DEBUG( spxout << "DMPSIN01 Line " << m_lineno
                //                          << " " << m_buf << std::endl; )
            } while (*m_buf == '*');

            /* Normalize line
             */
            len = int(strlen(m_buf));

            for (int i = 0; i < len; i++)
                if ((m_buf[i] == '\t') || (m_buf[i] == '\n') || (m_buf[i] == '\r'))
                    m_buf[i] = BLANK;

            if (len < 80)
                clear_from(m_buf, len);

            assert(strlen(m_buf) >= 80);

            /* Look for new section
             */
            if (*m_buf != BLANK) {
                m_f0 = strtok(&m_buf[0], " ");

                assert(m_f0 != 0);

                m_f1 = strtok(0, " ");

                return true;
            }

            if (!m_is_new_format) {
                /* Test for fixed format comments
                 */
                if ((m_buf[14] == '$') && (m_buf[13] == ' '))
                    clear_from(m_buf, 14);
                else if ((m_buf[39] == '$') && (m_buf[38] == ' '))
                    clear_from(m_buf, 39);

                /* Test for fixed format
                 */
                space = m_buf[12] | m_buf[13]
                        | m_buf[22] | m_buf[23]
                        | m_buf[36] | m_buf[37] | m_buf[38]
                        | m_buf[47] | m_buf[48]
                        | m_buf[61] | m_buf[62] | m_buf[63];

                if (space == BLANK || len < 13) {
                    /* Now we have space at the right positions.
                     * But are there also the non space where they
                     * should be ?
                     */
                    bool number = isdigit(m_buf[24]) || isdigit(m_buf[25])
                            || isdigit(m_buf[26]) || isdigit(m_buf[27])
                            || isdigit(m_buf[28]) || isdigit(m_buf[29])
                            || isdigit(m_buf[30]) || isdigit(m_buf[31])
                            || isdigit(m_buf[32]) || isdigit(m_buf[33])
                            || isdigit(m_buf[34]) || isdigit(m_buf[35]);

                    /* len < 13 is handle ROW lines with embedded spaces 
                     * in the names correctly
                     */
                    if (number || len < 13) {
                        /* Now we assume fixed format, so we patch possible embedded spaces.
                         */
                        patch_field(m_buf, 4, 12);
                        patch_field(m_buf, 14, 22);
                        patch_field(m_buf, 39, 47);
                    } else {
                        if (m_section == COLUMNS || m_section == RHS
                                || m_section == RANGES || m_section == BOUNDS)
                            m_is_new_format = true;
                    }
                } else {
                    m_is_new_format = true;
                }
            }
            s = &m_buf[1];

            /* At this point it is not clear if we have a indicator field.
             * If there is none (e.g. empty) f1 will be the first name field.
             * If there is one, f2 will be the first name field.
             * 
             * Initially comment marks '$' ar only allowed in the beginning
             * of the 2nd and 3rd name field. We test all fields but the first.
             * This makes no difference, since if the $ is at the start of a value
             * field, the line will be errornous anyway.
             */
            do {
                if (0 == (m_f1 = strtok(s, " ")))
                    break;

                if ((0 == (m_f2 = strtok(0, " "))) || (*m_f2 == '$')) {
                    m_f2 = 0;
                    break;
                }
                if (!strcmp(m_f2, "'MARKER'")) {
                    is_marker = true; // Actually a marker but simply return
                    return true; // Added for simply returning
                }

                if ((0 == (m_f3 = strtok(0, " "))) || (*m_f3 == '$')) {
                    m_f3 = 0;
                    break;
                }
                if (is_marker) {
                    if (!strcmp(m_f3, "'INTORG'"))
                        m_is_integer = true;
                    else if (!strcmp(m_f3, "'INTEND'"))
                        m_is_integer = false;
                    else
                        break; // unknown marker
                }

                if (!strcmp(m_f3, "'MARKER'")) {
                    is_marker = true; // Actually a marker but simply return
                    return true; // Added for simply returning
                }
                if ((0 == (m_f4 = strtok(0, " "))) || (*m_f4 == '$')) {
                    m_f4 = 0;
                    break;
                }
                if (is_marker) {
                    if (!strcmp(m_f4, "'INTORG'"))
                        m_is_integer = true;
                    else if (!strcmp(m_f4, "'INTEND'"))
                        m_is_integer = false;
                    else
                        break; // unknown marker
                }

                if ((0 == (m_f5 = strtok(0, " "))) || (*m_f5 == '$'))
                    m_f5 = 0;
            } while (false);
        } while (is_marker);

        //   MSG_DEBUG(
        //      spxout << "DMPSIN02 -----------------------------------------------" 
        //             << std::endl
        //             << "DMPSIN03 f0=" << ((m_f0 == 0) ? "nil" : m_f0) << std::endl
        //             << "DMPSIN04 f1=" << ((m_f1 == 0) ? "nil" : m_f1) << std::endl
        //             << "DMPSIN05 f2=" << ((m_f2 == 0) ? "nil" : m_f2) << std::endl
        //             << "DMPSIN06 f3=" << ((m_f3 == 0) ? "nil" : m_f3) << std::endl
        //             << "DMPSIN07 f4=" << ((m_f4 == 0) ? "nil" : m_f4) << std::endl
        //             << "DMPSIN08 f5=" << ((m_f5 == 0) ? "nil" : m_f5) << std::endl
        //             << "DMPSIN09 -----------------------------------------------" 
        //             << std::endl;
        //   )

        return true;
    }

    /// Insert \p name as field 1 and shift all other fields up.
    void insertName(const char* name, bool second = false) {
        m_f5 = m_f4;
        m_f4 = m_f3;
        m_f3 = m_f2;

        if (second)
            m_f2 = name;
        else {
            m_f2 = m_f1;
            m_f1 = name;
        }
    }

    //@}
};

#endif // _MPSINPUT_H_