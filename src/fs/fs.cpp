#include "fs/fs.h"
#include "libs/string/string.h"
#include "libs/string/class.h"
#include "fs/disk.h"
#include "terminal/terminal.h"

using namespace SimpleOS;

vector<size_t> FileSystem::taken_sectors;
FileSystem::Superblock FileSystem::block;

FileSystem::Directory FileSystem::root;
FileSystem::Directory* FileSystem::current_directory = &root;
vector<string> FileSystem::current_path;

string FileSystem::opened_file = "";

//void FileSystem::init_fs() {
//	Superblock temp_block;
//
//	if (read_superblock()) {
//		block = temp_block;
//
//		current_directory = &root;
//		uint32_t current_sector = block.file_table_start;
//
//		for (size_t i = 0; i < block.total_files; ++i) {
//			string buffer(512, '\0');
//			ata_read_sector(current_sector, buffer);
//
//			bool is_directory;
//			memcpy(&is_directory, buffer, sizeof(bool));
//
//			if (is_directory) {
//				Directory dir;
//				string dirname(buffer + sizeof(bool), 32); 
//				dir.parent = current_directory;
//				current_directory->directories.insert(dirname, dir);
//
//			}
//			else {
//				File file;
//				string filename(buffer + sizeof(bool), 32); 
//				memcpy(&file.size, buffer + sizeof(bool) + 32, sizeof(size_t));
//				size_t sector_count;
//				memcpy(&sector_count, buffer + sizeof(bool) + 32 + sizeof(size_t), sizeof(size_t));
//
//				file.sectors.resize(sector_count);
//				memcpy(file.sectors.data(), buffer + sizeof(bool) + 32 + sizeof(size_t) * 2, sector_count * sizeof(uint32_t));
//
//				current_directory->files.insert(filename, file);
//			}
//
//			current_sector++;
//		}
//	}
//	else {
//		format();
//	}
//
//}

bool FileSystem::create_file(const string& path, const string& data) {

	Directory* last_directory = current_directory;
	vector<string> last_current_path = current_path;
	
	string key;
	if (!__cd(path, key)) return false;

	if (current_directory->files.has(key)) {
		Terminal::lnprint(key + " already exist");
		return false;
	}

	File new_file;

	new_file.size = data.size();

	ssize_t freesector = free_sector();

	if (freesector == _INVALID_SECTOR) {
		Terminal::lnprint("No free space");
		return false;
	}

	new_file.sectors.push(freesector);

	if (data.c_str()) {
		distr_to_sectors(new_file, data.c_str());
	}	

	current_directory->files.insert(key, new_file);

	current_directory = last_directory;
	current_path = last_current_path;

	return true;
}

bool FileSystem::copy_file(const string& path, const string& result_path) {

	Directory* last_directory = current_directory;
	vector<string> last_current_path = current_path;

	string key;
	if (!__cd(path, key)) return false;

	if (!current_directory->files.has(key)) {
		Terminal::lnprint("Source file not found");
		return false;
	}

	string file_data;
	read_file(path, file_data);

	current_directory = last_directory;
	current_path = last_current_path;

	return create_file(result_path, file_data);
}


bool FileSystem::delete_file(const string& path) {

	Directory* last_directory = current_directory;
	vector<string> last_current_path = current_path;

	string key;
	if (!__cd(path, key)) return false;
	
	if (!__check_exist(key)) return false;

	File file = current_directory->files[key];

	for (size_t i = 0, size = file.sectors.size(); i < size; ++i) {
		ata_delete_from_sector(file.sectors[i]);

		if (taken_sectors.has(file.sectors[i]))
			taken_sectors.pop(taken_sectors.find(file.sectors[i]));

	}
	current_directory->files.erase(key);

	//write_superblock();

	current_directory = last_directory;
	current_path = last_current_path;

	return true;
}

bool FileSystem::write_to_file(const string& path, const string& data) {

	Directory* last_directory = current_directory;
	vector<string> last_current_path = current_path;

	string key;
	if (!__cd(path, key)) return false;

	bool result = true;

	if (!__check_exist(key)) result = false;

	else if (!delete_file(key)) result = false;

	else if (!create_file(key, data)) result = false;

	current_directory = last_directory;
	current_path = last_current_path;

	return result;
}

bool FileSystem::append_to_file(const string& path, const string& data) {

	Directory* last_directory = current_directory;
	vector<string> last_current_path = current_path;

	string key;
	if (!__cd(path, key)) return false;

	bool result = true;

	if (!__check_exist(key)) result = false;

	current_directory->files[key].size += data.size();

	if (!distr_to_sectors(current_directory->files[key], data.c_str())) result = false;

	//write_superblock();

	current_directory = last_directory;
	current_path = last_current_path;

	return result;
}

bool FileSystem::read_file(const string& path, string& buffer) {

	Directory* last_directory = current_directory;
	vector<string> last_current_path = current_path;

	string key;
	if (!__cd(path, key)) return false;

	if (!__check_exist(key)) return false;

	File file = current_directory->files[key];
	size_t size = file.sectors.size();

	for (size_t i = 0; i < size; i++) {
		string data;
		ata_read_sector(file.sectors[i], data);
		buffer = buffer + data;
	}
	current_directory = last_directory;
	current_path = last_current_path;

	return true;
}

bool FileSystem::format() {

	bool has_error = false;
	current_directory->files.forEach([&has_error](const string& key, File) {
		if (!delete_file(key))
			has_error = true;
		});
	ata_delete_from_sector(0);

	return !has_error;
}

bool FileSystem::file_exist(const string& path) {
	Directory* last_directory = current_directory;
	vector<string> last_current_path = current_path;

	string key;
	if (!__cd(path, key)) return false;

	bool result = current_directory->files.has(key);

	current_directory = last_directory;
	current_path = last_current_path;

	return result;
}

ssize_t FileSystem::free_sector() {

	for (uint32_t i = 1, sector_count = ata_get_sector_count(); i < sector_count; i++) {
		if (!taken_sectors.has(i)) {
			taken_sectors.push(i);
			return i;
		}
	}
	return _INVALID_SECTOR;
}

bool FileSystem::distr_to_sectors(File& file, const char* data) {
	size_t free_space = ata_get_free_space_in_sector(file.sectors.back());
	if (free_space) {
		char buffer[free_space];

		strncpy(buffer, data, free_space - 1);

		buffer[free_space - 1] = '\0';

		ata_append_to_sector(file.sectors.back(), buffer);

		data += free_space;
	}

	while (strlen(data) > 0) {

		ssize_t freesector = free_sector();
		if (freesector == _INVALID_SECTOR) {
			Terminal::lnprint("No free space");
			return false;
		}

		file.sectors.push(freesector);

		char buffer[512];
		size_t data_to_write = min(strlen(data), size_t(512));
		memcpy(buffer, data, data_to_write);

		if (data_to_write < 512) {
			buffer[data_to_write] = '\0';
		}
		else {
			buffer[511] = '\0';
		}

		ata_write_to_sector(freesector, buffer);

		data += data_to_write;
	}
	return true;
}

bool FileSystem::__check_exist(const string& path) {
	if (!file_exist(path)) {
		Terminal::lnprint("File don`t exists");
		return false;
	}
	return true;
}


//bool FileSystem::read_superblock() {
//	string buffer(512, '\0');
//	memset(buffer, 0, sizeof(buffer));
//	size_t offset = 0;
//
//	ata_read_sector(0, buffer);
//
//	size_t file_count;
//	size_t dir_count;
//
//	memcpy(&file_count, buffer + offset, sizeof(size_t));
//	offset += sizeof(size_t);
//
//	memcpy(&dir_count, buffer + offset, sizeof(size_t));
//	offset += sizeof(size_t);
//
//	current_directory->files.clear();
//	current_directory->directories.clear();
//
//	for (size_t i = 0; i < file_count; ++i) {
//		size_t name_length;
//		memcpy(&name_length, buffer.c_str() + offset, sizeof(size_t));
//		offset += sizeof(size_t);
//
//		string file_name(buffer.c_str() + offset, name_length);
//		offset += name_length;
//
//		File file;
//		memcpy(&file, buffer.c_str() + offset, sizeof(File));
//		offset += sizeof(File);
//
//		current_directory->files[file_name] = file;
//	}
//
//	for (size_t i = 0; i < dir_count; ++i) {
//		size_t name_length;
//		memcpy(&name_length, buffer.c_str() + offset, sizeof(size_t));
//		offset += sizeof(size_t);
//
//		string dir_name(buffer.c_str() + offset, name_length);
//		offset += name_length;
//
//		Directory dir;
//		memcpy(&dir, buffer.c_str() + offset, sizeof(Directory));
//		offset += sizeof(Directory);
//
//		current_directory->directories[dir_name] = dir;
//	}
//
//	return true;
//}

//bool FileSystem::write_superblock() {
//	char buffer[512];
//	memset(buffer, 0, sizeof(buffer));
//	size_t offset = 0;
//	Directory* root_dir = &root;
//
//	size_t file_count = root_dir->files.size();
//	size_t dir_count = root_dir->directories.size();
//
//	memcpy(buffer + offset, &file_count, sizeof(size_t));
//	offset += sizeof(size_t);
//
//	memcpy(buffer + offset, &dir_count, sizeof(size_t));
//	offset += sizeof(size_t);
//
//	root_dir->files.forEach([&](const string& key, const File& file) {
//		size_t name_length = key.size();
//		memcpy(buffer + offset, &name_length, sizeof(size_t));
//		offset += sizeof(size_t);
//
//		memcpy(buffer + offset, key.c_str(), name_length);
//		offset += name_length;
//
//		memcpy(buffer + offset, &file, sizeof(File));
//		offset += sizeof(File);
//	});
//
//	root_dir->directories.forEach([&](const string& key, const Directory& dir) {
//		size_t name_length = key.size();
//		memcpy(buffer + offset, &name_length, sizeof(size_t));
//		offset += sizeof(size_t);
//
//		memcpy(buffer + offset, key.c_str(), name_length);
//		offset += name_length;
//
//		memcpy(buffer + offset, &dir, sizeof(Directory));
//		offset += sizeof(Directory);
//		});
//
//	ata_write_to_sector(0, buffer);
//
//	return true;
//}

void FileSystem::open_file(const string& path) {
	Directory* last_directory = current_directory;
	vector<string> last_current_path = current_path;

	string key;
	if (!__cd(path, key)) return;

	opened_file = key;

	current_directory = last_directory;
	current_path = last_current_path;
}

const string& FileSystem::get_opened_file() {
	return opened_file;
}

void FileSystem::close_file() {
	opened_file = "";
}