#include "File.h"

namespace Engine
{
	bool FileExists(const std::string& i_Filename)
	{
		return !((GetFileAttributesA(i_Filename.c_str()) == INVALID_FILE_ATTRIBUTES) && (GetLastError() == ERROR_FILE_NOT_FOUND));
	}

	std::vector<uint8_t> LoadFile(const std::string& i_Filename)
	{
		std::vector<uint8_t> Contents;

		if (!i_Filename.empty())
		{
			FILE* pFile = nullptr;

			errno_t fopenError = fopen_s(&pFile, i_Filename.c_str(), "rb");
			if (fopenError == 0)
			{
				assert(pFile != nullptr);

				int FileIOError = fseek(pFile, 0, SEEK_END);
				assert(FileIOError == 0);

				long FileSize = ftell(pFile);
				assert(FileSize >= 0);

				FileIOError = fseek(pFile, 0, SEEK_SET);
				assert(FileIOError == 0);

				Contents.reserve(FileSize);
				Contents.resize(FileSize);

				size_t FileRead = fread(&Contents[0], 1, FileSize, pFile);
				assert(FileRead == FileSize);

				fclose(pFile);
			}
		}

		return Contents;
	}

} // namespace Engine
