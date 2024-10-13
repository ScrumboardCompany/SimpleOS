#include "fs/fs.h"
#include "libs/string/string.h"
#include "libs/string/class.h"
#include "fs/disk.h"
#include "terminal/terminal.h"

using namespace SimpleOS;

//map<string, FileSystem::File> FileSystem::files;
vector<size_t> FileSystem::taken_sectors;
FileSystem::Superblock FileSystem::block;

FileSystem::Directory FileSystem::root;
FileSystem::Directory* FileSystem::current_directory = &root;
vector<string> FileSystem::current_path;

string FileSystem::opened_file = "";

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

	//write_superblock();

	current_directory = last_directory;
	current_path = last_current_path;

	return true;
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