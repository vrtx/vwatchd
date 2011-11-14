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
#include "abstract_parser.hpp"
using namespace vwatch;

/// @brief  Default constructor
abstract_parser::abstract_parser()
{
    
}

/// @brief  Default destructor
abstract_parser::~abstract_parser()
{
    
}

// @brief   Initialize the parser (before data has been passed)
void abstract_parser::initialize() 
{
    
}

/// @brief  Incrementally process an additional buffer
int abstract_parser::operator()(const string &buffer)
{
    return 0;
}

/// @brief  Complete the parsing
datapoints_t abstract_parser::complete(const int err_code)
{
    datapoints_t all_data;
    return all_data;
}


