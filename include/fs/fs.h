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

		struct Directory {
			map<string, File> files;
			map<string, Directory> directories;
			Directory* parent = nullptr;
		};

		struct Superblock {
			uint32_t file_table_start;
			uint32_t total_files;   
			uint32_t total_sectors;   
			uint32_t free_sectors;
		};

		static bool create_file(const string& name, const string& data = nullptr);

		static bool delete_file(const string& name);

		static bool write_to_file(const string& name, const string& data);

		static bool append_to_file(const string& name, const string& data);

		static bool read_file(const string& name, string& buffer);

		static bool format();

		static void file_info(const string& name);

		static bool file_exist(const string& name);

		static bool __check_exist(const string& name);
		static bool __check_dir_exist(const string& name);

		static bool create_dir(const string& name);

		static bool delete_dir(const string& name);

		static bool cd(const string& name);

		static bool cd_up();

		static void tree(const Directory& dir, ssize_t level = 0);

		static string get_current_path();

		static Directory* get_current_directory();

		static Directory get_root();

		static void open_file(const string& name);

		static const string& get_opened_file();

		static void close_file();

	private:
		static void print_tree(const Directory& dir, ssize_t level);

		static ssize_t free_sector();

		static bool distr_to_sectors(File& file, const char* data);

		static Directory root;
		static Directory* current_directory;
		static vector<string> current_path;

		//static map<string, File> files;
		static vector<size_t> taken_sectors;
        static Superblock block;

		static string opened_file;
	};

}

#endif // _FS_