#pragma once
#include "stdafx.h"
#include <stdint.h>
#include <stdlib.h>

bool readBmp(const char* path, uint8_t **imageData, int *pWidth, int *pHeight);
