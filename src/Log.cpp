/*
 * Log.cpp
 *
 *  Created on: 7 Jul 2020
 *      Author: julianporter
 */

#include "Log.hpp"

static std::fstream stream("/Users/julianporter/Rack/log.txt",std::ios_base::app) ;

std::iostream & log() { return stream; }
