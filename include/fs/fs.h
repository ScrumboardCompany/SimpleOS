#pragma once

#ifndef _FS_
#define _FS_

#include "libs/vector/vector.h"
#include "libs/map/map.h"
#include "utils/typedef.h"
#include "libs/string/class.h"

#define _INVALID_SECTOR -1

namespace SimpleOS {

	class FileSystem {
		
	public:

		struct File {
			size_t size = 0;
			vector<uint32_t> sectors;

		};

		struct Superblock {
			uint32_t file_table_start;
			uint32_t total_files;   
			uint32_t total_sectors;   
			uint32_t free_sectors;
		};

		static void init_fs();

		static bool create_file(const char* name, const char* data = nullptr);

		static bool delete_file(const char* name);

		static bool write_to_file(const char* name, const char* data);

		static bool append_to_file(const char* name, const char* data);

		static bool read_superblock();

		static bool write_superblock();

		static bool read_file(const char* name, string& buffer);

		static bool format();

		static void file_info(const char* name);

		static bool exist(const char* name);

	private:
		static ssize_t free_sector(size_t size);
		static ssize_t free_sector();

		static bool distr_to_sectors(File& file, const char* data);

		static bool __check_exist(const char* name);

		static map<string, File> files;
		static vector<size_t> taken_sectors;
        static Superblock block;
	};

}

#endif // _FS_