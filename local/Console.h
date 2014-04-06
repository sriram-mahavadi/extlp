/* 
 * File:   Console.h
 * Author: sriram
 *
 * Created on 21 March, 2014, 10:05 PM
 */

#ifndef CONSOLE_H
#define	CONSOLE_H
// Performs the console input and output operations.
class Console {
public:

    static void println(std::string x) {
        std::cout << x << "\r\n";
    }

    static void print(std::string x) {
        std::cout << x;
    }
    
    // Prints the simple version and license information
    static void printVersionInfo() {
        const char* banner1 =
                "************************************************************************\n"
                "*                                                                      *\n"
                "*       Extlp --- LP Solver using External Memory.                     *\n"
                ;

        const char* banner2 =
                "*                                                                      *\n"
                "*    Copyright (C) 2013-2014 M V Sai Sriram                            *\n"
                "*                            IIIT Bangalore                            *\n"
                "*                                                                      *\n"
                "*  Extlp is distributed under the terms of the opensource Licence.     *\n"
                "*  You may make any changes and redistribute                           *\n"
                "*  along with Extlp; If not email to sriram.mahavadi@gmail.com         *\n"
                "*                                                                      *\n"
                "************************************************************************\n"
                ;

        Console::println(banner1);
        Console::println(banner2);
    }

    // Prints the usage of console - command line format
    // and exits the solver after display.

    static void printUsage(const char* const argv[]) {
        const char* usage =
                "[options] LPfile [Basfile]\n\n"
                "          LPfile can be either in MPS or LPF format\n\n"
                "options:  (*) indicates default\n"
                "          (!) indicates experimental features which may give wrong results\n"
                " -e        select entering algorithm (default is leaving)\n"
                " -r        select row wise representation (default is column)\n"
                " -i        select Eta-update (default is Forest-Tomlin)\n"
                " -x        output solution vector\n"
                " -y        output dual multipliers\n"
                " -q        display solution quality\n"
                " -br       read file with starting basis from Basfile\n"
                " -bw       write file with optimal basis to Basfile\n"
                " -l        set time limit in seconds\n"
                " -L        set iteration limit\n"
                " -f        set primal feasibility tolerance\n"
                " -o        set optimality, i.e., dual feasibility tolerance\n"
                " -d        set primal and dual feasibility tolerance to same value\n"
                " -R        set threshold for tolerances below which iterative refinement is applied\n"
                " -zz       set general zero tolerance\n"
                " -zf       set factorization zero tolerance\n"
                " -zu       set update zero tolerance\n"
                " -v        set verbosity Level: from 0 (ERROR) to 5 (INFO3), default 3 (INFO1)\n"
                " -V        show program version\n"
                " -C        check mode (for check scripts)\n"
                " -h        show this help\n\n"
                "Simplifier:  Scaler:           Starter:    Pricer:        Ratiotester:\n"
                " -s0 none     -g0 none          -c0 none*   -p0 Textbook   -t0 Textbook\n"
                " -s1 Main*    -g1 uni-Equi      -c1 Weight  -p1 ParMult    -t1 Harris\n"
                "              -g2 bi-Equi*      -c2 Sum     -p2 Devex      -t2 Fast*\n"
                "              -g3 bi-Equi+Geo1  -c3 Vector  -p3 Hybrid!    -t3 Bound Flipping\n"
                "              -g4 bi-Equi+Geo8              -p4 Steep*\n"
                "                                            -p5 Weight\n"
                "                                            -p6 SteepExactSetup\n"
                ;

        std::cerr << "usage: " << argv[0] << " " << usage << std::endl;
        exit(0);
    }
    //    static void println(char* x){
    //        std::cout<<x<<"\r\n";
    //    }
    //    static void print(char* x){
    //        std::cout<<x;
    //    }
};

#endif	/* CONSOLE_H */

