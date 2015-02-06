/*
  ALWAYS include parser.h instead of y.tab.h

  Here is why:

    Since the union created by bison contains several gpl objects,
    a forward class declaration, or including the .h file must be 
    done before the union is created.

    Bison generates y.tab.h where the union is created. 
    I can't find a way to get bison to generate a y.tab.h that includes 
    forward class declarations and #include's, so I created this file.


*/

#ifndef PARSER_H
#define PARSER_H

//#define _DEBUG

// for each type used in the union, you will need to 
// put the appropriate include here
// For example, the union must hold a C++ string, so <string> is included
//   since it is bad programming practive to include "using namespace std"
//   in a .h file, you will have to put "std::string" in the union instead
//   of "string"

#include <string>
#include "gpl_type.h"
#include "expression.h"
#include "value.h"

#include "y.tab.h"

#include <iostream>


#ifdef _DEBUG
#define TRACE_VERBOSE(msg)\
	std::cout << "TRACE_VERBOSE: " << msg << " (file: " << __FILE__ \
		<< ", line #" << __LINE__ << ")" <<   std::endl;

#define TRACE_ERROR(msg)\
	std::cerr << "TRACE_ERROR: " << msg << " (file: " << __FILE__ \
		<< ", line #" << __LINE__ << ")" << std::endl;
#else
#define TRACE_VERBOSE(msg) {;}
#define TRACE_ERROR(msg) {;}
#endif


#endif // #ifndef PARSER_H
