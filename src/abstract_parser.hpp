///////////////////////////////////////////////////////
// vwatchd
//
// Copyright (c) 2011 Benjamnin Becker
// All Rights Reserved
//
// @brief   Trace parsing code
// @author  ben
// @date    11/13/2011
///////////////////////////////////////////////////////

#ifndef ABSTRACT_PARSER_HPP_VWATCHD
#define ABSTRACT_PARSER_HPP_VWATCHD

#include "common.hpp"
#include <boost/any.hpp>

namespace vwatch {

    typedef map <string, boost::any> datapoints_t;

    /// @class   Abstract base functor for implementing a subprocess,
    ///          database or file parser.
    /// @param   buffer  Buffer to parse
    /// @retrun  number of bytes handled
    ///          0 if all data has been processed
    ///          -1 if there was a fatal parsing error
    class abstract_parser {
    public:
        abstract_parser();
        virtual ~abstract_parser();
        virtual void initialize();                          // initialize the parser before any data has been read
        virtual datapoints_t complete(const int err_code);  // complete the parsing
        virtual int operator()(const string &buffer);       // process the next buffer
    private:
        // hidden methods
        abstract_parser(const abstract_parser &rhs);              // copy ctor
        // abstract_parser &operator=(const abstract_parser &rhs);   // assignment operator
    };


}

#endif
