///////////////////////////////////////////////////////
// vwatchd
//
// Copyright (c) 2011 Benjamnin Becker
// All Rights Reserved
//
// @brief   Parser
// @author  ben
// @date    11/13/2011
///////////////////////////////////////////////////////

#include "common.hpp"
#include "trace_parser.hpp"
using namespace vwatch;

/// @brief  Default constructor
trace_parser::trace_parser()
{
    
}

/// @brief  Default destructor
trace_parser::~trace_parser()
{
    
}

// #brief   Initialize the parser (before data has been passed)
void trace_parser::initialize() 
{
    
}

/// @brief  Incrementally process an additional buffer
int trace_parser::operator()(const string &buffer)
{
    return 0;
}


/// @brief  Complete the parsing
datapoints_t trace_parser::complete(const int err_code)
{
    datapoints_t all_data;
    return all_data;
}


