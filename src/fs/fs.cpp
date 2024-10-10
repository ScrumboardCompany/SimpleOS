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

//void FileSystem::init_fs() {
//	Superblock temp_block;
//
//	if (read_superblock()) {
//		block = temp_block;
//
//		uint32_t current_sector = block.file_table_start;
//
//		for (size_t i = 0; i < block.total_files; ++i) {
//			char buffer[512];
//			ata_read_sector(current_sector, buffer);
//
//			char filename[32];
//			memcpy(filename, buffer, 32);
//
//			File file;
//			memcpy(&file, buffer + 32, sizeof(File));
//
//			files[string(filename)] = file;
//
//			current_sector++;
//		}
//	}
//	else {
//		format();
//	}
//
//}

bool FileSystem::create_file(const string& name, const string& data) {

	if (current_directory->files.has(name)) {
		Terminal::lnprint(name + " already exist");
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

	current_directory->files.insert(name, new_file);

	//write_superblock();

	return true;
}

bool FileSystem::delete_file(const string& name) {
	
	if (!__check_exist(name)) return false;

	File file = current_directory->files[name];

	for (size_t i = 0, size = file.sectors.size(); i < size; ++i) {
		ata_delete_from_sector(file.sectors[i]);

		if (taken_sectors.has(file.sectors[i]))
			taken_sectors.pop(taken_sectors.find(file.sectors[i]));

	}
	current_directory->files.erase(name);

	//write_superblock();

	return true;
}

bool FileSystem::write_to_file(const string& name, const string& data) {

	if (!__check_exist(name)) return false;

	if (!delete_file(name)) {
		return false;
	}

	if (!create_file(name, data)) return false;

	return true;
}

bool FileSystem::append_to_file(const string& name, const string& data) {

	if (!__check_exist(name)) return false;

	current_directory->files[name].size += data.size();

	if (!distr_to_sectors(current_directory->files[name], data.c_str())) return false;

	//write_superblock();

	return true;
}

bool FileSystem::read_file(const string& name, string& buffer) {

	if (!__check_exist(name)) return false;

	File file = current_directory->files[name];
	size_t size = file.sectors.size();

	for (size_t i = 0; i < size; i++) {
		string data;
		ata_read_sector(file.sectors[i], data);
		buffer = buffer + data;
	}

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

bool FileSystem::file_exist(const string& name) {

	return current_directory->files.has(name);
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

bool FileSystem::__check_exist(const string& name) {
	if (!current_directory->files.has(name)) {
		Terminal::lnprint("File don`t exists");
		return false;
	}
	return true;
}

bool FileSystem::__check_dir_exist(const string& name) {
	if (!current_directory->directories.has(name)) {
		Terminal::lnprint("Directory don`t exists");
		return false;
	}
	return true;
}

//bool FileSystem::read_superblock() {
//
//	char buffer[512];
//
//	ata_read_sector(0, buffer);
//
//	size_t offset = 0;
//	size_t file_count;
//	memcpy(&file_count, buffer + offset, sizeof(size_t));
//	offset += sizeof(size_t);
//
//	for (size_t i = 0; i < file_count; ++i) {
//		char filename[32];
//		memcpy(filename, buffer + offset, 32);
//		offset += 32;
//
//		File file;
//		memcpy(&file.size, buffer + offset, sizeof(size_t));
//		offset += sizeof(size_t);
//
//		size_t sector_count;
//		memcpy(&sector_count, buffer + offset, sizeof(size_t));
//		offset += sizeof(size_t);
//
//		file.sectors.resize(sector_count);
//		for (size_t j = 0; j < sector_count; ++j) {
//			memcpy(&file.sectors[j], buffer + offset, sizeof(uint32_t));
//			offset += sizeof(uint32_t);
//		}
//
//		files[string(filename)] = file;
//	}
//
//	files.forEach([](const string&, File file) {
//		for (size_t i = 0; i < file.sectors.size(); i++) {
//			if (!taken_sectors.has(file.sectors[i])) {
//				taken_sectors.push(file.sectors[i]);
//			}
//		}
//		});
//
//	return true;
//}

//bool FileSystem::write_superblock() {
//	char buffer[512];
//	size_t offset = 0;
//
//	size_t file_count = files.size();
//	memcpy(buffer + offset, &file_count, sizeof(size_t));
//	offset += sizeof(size_t);
//
//	files.forEach([&](const string& key, File file) {
//
//		memcpy(buffer + offset, key.c_str(), 32);
//		offset += 32;
//
//		memcpy(buffer + offset, &file.size, sizeof(size_t));
//		offset += sizeof(size_t);
//
//		size_t sector_count = file.sectors.size();
//		memcpy(buffer + offset, &sector_count, sizeof(size_t));
//		offset += sizeof(size_t);
//
//		for (size_t i = 0; i < file.sectors.size(); i++) {
//			memcpy(buffer + offset, &file.sectors[i], sizeof(uint32_t));
//			offset += sizeof(uint32_t);
//		}
//	});
//
//	ata_write_to_sector(0, buffer);
//
//	return true;
//}

void FileSystem::open_file(const string& name) {
	opened_file = name;
}

const string& FileSystem::get_opened_file() {
	return opened_file;
}

void FileSystem::close_file() {
	opened_file = "";
}