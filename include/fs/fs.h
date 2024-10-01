#pragma once

#ifndef _FS_
#define _FS_

#include "libs/vector/vector.h"
#include "libs/map/map.h"
#include "utils/typedef.h"

namespace SimpleOS {

	class FileSystem {
		
	public:
		struct File {
			char* name = nullptr;
			size_t size = 0;
			vector<uint32_t> sectors;

		};

		static bool create_file(const char* name, const char* data = nullptr);

		static bool delete_file(const char* name);

		static void write_file(const char* name, const char* data);

		static void read_file(const char* name, char* buffer);

		static bool append_to_file(const char* name, const char* data);

	private:
		static ssize_t free_sector(size_t size);

		static vector<File> files;
		static vector<size_t> taken_sectors;
	};

}

#endif // _FS_