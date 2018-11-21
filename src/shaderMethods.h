#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>

GLuint loadShaderProg(const char *vertexFilePath, const char *fragmentFilePath, const char *geometryFilePath = nullptr);