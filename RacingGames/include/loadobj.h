#ifndef OBJLOAD_H
#define OBJLOAD_H

#ifndef OS_LINUX
#pragma execution_character_set("utf-8")
#endif

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "mymodel.h"


myModel loadObj(const std::string);
#endif
