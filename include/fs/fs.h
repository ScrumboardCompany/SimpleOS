#pragma once

#ifndef _FS_
#define _FS_

#include "libs/vector/vector.h"
#include "libs/map/map.h"
#include "utils/typedef.h"
#include "libs/string/class.h"

#define _INVALID_SECTOR -1
#define PERCENTAGE_FOR_METADATA 0.2f

namespace SimpleOS {

	class FileSystem {
		
	public:

		struct File {
			size_t size = 0;
			vector<uint32_t> sectors;

			string serialize() const;
			void deserialize(const string& input);
		};

		struct Directory {
			map<string, File> files;
			map<string, Directory> directories;
			Directory* parent = nullptr;

			string serialize() const;
			void deserialize(const string& input);
		};

		static void init();

		static void set_metadata_of_files();

		static void get_metadata_of_files();

		static bool create_file(const string& path, const string& data = nullptr);

		static bool copy_file(const string& path, const string& result_path);

		static bool delete_file(const string& path);

		static bool write_to_file(const string& path, const string& data);

		static bool append_to_file(const string& path, const string& data);

		static bool read_file(const string& path, string& buffer);

		static bool format();

		static bool file_exist(const string& path);
		static bool dir_exist(const string& path);

		static bool __check_exist(const string& path);
		static bool __check_dir_exist(const string& path);

		static bool create_dir(const string& path);

		static bool copy_dir(const string& path, const string& result_path);

		static bool delete_dir(const string& path);

		static bool cd(const string& path);

		static bool cd(const vector<string>& path);

		static void cd();

		static bool cd_up();

		static void cd_down(const string& name);

		static void tree(const Directory& dir, ssize_t level = 0);

		static void set_current_path(const vector<string>& path);

		static string get_current_path();

		static Directory* get_current_directory();

		static const Directory& get_root();

		static void open_file(const string& path);

		static const string& get_opened_file();

		static void close_file();

	private:

		static void print_tree(const Directory& dir, ssize_t level);
		static bool file_exist(const Directory& dir, const string& name);
		static bool dir_exist(const Directory& dir, const string& name);

		static ssize_t free_sector();
		static bool check_is_metadata_of_files_sector(size_t num);

		static bool __cd(const string& __path, string& buffer);

		static bool distr_to_sectors(File& file, const char* data);

		static bool distr_files_metadata_to_sectors(File& file, const char* data);

		static Directory root;
		static Directory* current_directory;
		static vector<string> current_path;

		static vector<size_t> taken_sectors;

		static string opened_file;
	};

}

#endif // _FS_