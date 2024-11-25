#include "fs/fs.h"
#include "libs/string/string.h"
#include "libs/string/class.h"
#include "fs/disk.h"
#include "terminal/terminal.h"

using namespace SimpleOS;

vector<size_t> FileSystem::taken_sectors;

FileSystem::Directory FileSystem::root;
FileSystem::Directory* FileSystem::current_directory = &root;
vector<string> FileSystem::current_path;

string FileSystem::opened_file = "";

string FileSystem::File::serialize() const {
	string result;

	result = result + string::to_string(size);

	for (size_t i = 0; i < sectors.size(); i++) {
		result = result + " " + string::to_string((int)sectors[i]);
	}

	return result;
}

void FileSystem::File::deserialize(const string& input) {
	vector<string> tokens = split(input, ' ');
	if (tokens.empty()) return;

	size = tokens[0].to_int();
	sectors.clear();
	for (size_t i = 1; i < tokens.size(); ++i) {
		sectors.push(string::to_int(tokens[i]));
	}
}

string FileSystem::Directory::serialize() const {
	string result;

	result = result + "FILES\n";

	files.forEach([&result](const string& key, const File& file) {
		result = result + key + ":" + file.serialize() + "\n";
		});

	result = result + "DIRECTORIES\n";

	directories.forEach([&result](const string& key, const Directory& dir) {
		result = result + key + "\n" + dir.serialize();
		});

	result = result + "END\n";
	return result;
}

void FileSystem::Directory::deserialize(const string& input) {
	files.clear();
	directories.clear();

	vector<string> lines = split(input, '\n');
	size_t i = 0;

	if (i < lines.size() && lines[i] == "FILES") {
		++i;
		while (i < lines.size() && lines[i] != "DIRECTORIES") {
			size_t delim_pos = lines[i].find(':');
			if (delim_pos != string::npos) {
				string file_name = lines[i].substr(0, delim_pos);
				File file;
				size_t content_start = delim_pos + 1;
				size_t content_length = lines[i].size() - content_start;
				file.deserialize(lines[i].substr(content_start, content_length));
				files[file_name] = file;
			}
			++i;
		}
	}

	if (i < lines.size() && lines[i] == "DIRECTORIES") {
		++i;
		while (i < lines.size() && lines[i] != "END") {
			string dir_name = lines[i];
			Directory dir;
			dir.parent = this;

			size_t dir_start = ++i;
			size_t dir_end = dir_start;
			int nested_level = 1;

			while (dir_end < lines.size() && nested_level > 0) {
				if (lines[dir_end] == "DIRECTORIES") {
					++nested_level;
				}
				else if (lines[dir_end] == "END") {
					--nested_level;
				}
				++dir_end;
			}

			vector<string> sub_lines;
			for (size_t j = dir_start; j < dir_end - 1; ++j) {
				sub_lines.push(lines[j]);
			}
			dir.deserialize(string::join("\n", sub_lines));
			directories[dir_name] = dir;
			i = dir_end;
		}
	}
}

void FileSystem::init() {
	get_metadata_of_files();
}

void FileSystem::get_metadata_of_files() {
	string metadata;

	size_t total_sectors = ata_get_sector_count();
	size_t start_sector = total_sectors * (1.f - PERCENTAGE_FOR_METADATA);

	for (; start_sector < total_sectors; start_sector++) {
		if (ata_empty_sector(start_sector)) break;
		
		string sector;
		ata_read_sector(start_sector, sector);

		metadata = metadata + sector;
	}

	if (!metadata.empty()) {
		root.deserialize(metadata);
	}
}

void FileSystem::set_metadata_of_files() {

	string metadata = root.serialize();

	size_t total_sectors = ata_get_sector_count();
	size_t start_sector = total_sectors * (1.f - PERCENTAGE_FOR_METADATA);

	File metadata_file;
	metadata_file.sectors.push(start_sector);

	for (; start_sector < total_sectors; start_sector++) {
		if (ata_empty_sector(start_sector)) break;

		ata_delete_from_sector(start_sector);
	}

	distr_files_metadata_to_sectors(metadata_file, metadata.c_str());
}

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

	set_metadata_of_files();

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

	set_metadata_of_files();

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

	set_metadata_of_files();

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
	set_metadata_of_files();

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

bool FileSystem::check_is_metadata_of_files_sector(size_t num) {
	return num < ata_get_sector_count() * (1.f - PERCENTAGE_FOR_METADATA);
}

ssize_t FileSystem::free_sector() {

	for (uint32_t i = 0, sector_count = ata_get_sector_count(); i < sector_count; i++) {
		if (!taken_sectors.has(i) && check_is_metadata_of_files_sector(i)) {
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

bool FileSystem::distr_files_metadata_to_sectors(File& file, const char* data) {
	size_t total_sectors = ata_get_sector_count();
	size_t start_sector = total_sectors * (1.f - PERCENTAGE_FOR_METADATA);

	auto free_last_sector = [&taken_sectors, start_sector, total_sectors]() -> ssize_t {
		for (uint32_t i = start_sector; i < total_sectors; ++i) {
			if (!taken_sectors.has(i)) {
				taken_sectors.push(i);
				return i;
			}
		}

		return _INVALID_SECTOR;
		};


	size_t free_space = ata_get_free_space_in_sector(file.sectors.back());

	if (free_space) {
		char buffer[free_space];

		strncpy(buffer, data, free_space - 1);

		buffer[free_space - 1] = '\0';

		ata_append_to_sector(file.sectors.back(), buffer);

		data += free_space;
	}

	while (strlen(data) > 0) {
		ssize_t freesector = free_last_sector();
		if (freesector == _INVALID_SECTOR) {
			Terminal::lnprint("No free space for files metadata of sectors");
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