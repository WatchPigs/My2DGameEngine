#pragma once

#include <vector>
#include <string>
#include <stdint.h>
#include <Windows.h>
#include <assert.h>

namespace Engine
{
	bool FileExists(const std::string& i_Filename);
	std::vector<uint8_t> LoadFile(const std::string& i_FileName);
}
