#include "fs/fs.h"
#include "terminal/terminal.h"
#include "utils/utils.h"

using namespace SimpleOS;

bool FileSystem::create_dir(const string& path) {

	Directory* last_directory = current_directory;
	vector<string> last_current_path = current_path;

	string key;
	if (!__cd(path, key)) return false;

	if (current_directory->directories.has(key)) {
		Terminal::lnprint(key + " already exist");

		current_directory = last_directory;
		current_path = last_current_path;
		return false;
	}

	Directory new_dir;
	new_dir.parent = current_directory;
	current_directory->directories.insert(key, new_dir);

	current_directory = last_directory;
	current_path = last_current_path;

	return true;
}

bool FileSystem::delete_dir(const string& path) {

	Directory* last_directory = current_directory;
	vector<string> last_current_path = current_path;

	string key;
	if (!__cd(path, key)) return false;

	if (!__check_dir_exist(key)) return false;

	cd_down(key);

	current_directory->files.forEach([](const string& name, const File&) {
		delete_file(name);
		});

	current_directory->directories.forEach([](const string& name, const Directory&) {
		delete_dir(name);
		});

	cd_up();

	current_directory->directories.erase(key);

	current_directory = last_directory;
	current_path = last_current_path;

	return true;
}

bool FileSystem::dir_exist(const string& path) {
	Directory* last_directory = current_directory;
	vector<string> last_current_path = current_path;

	string key;
	if (!__cd(path, key)) return false;

	bool result = current_directory->directories.has(key);

	current_directory = last_directory;
	current_path = last_current_path;

	return result;
}

bool FileSystem::cd(const string& path) {

	vector<string> _path = split(path, '/');

	return cd(_path);
}

bool FileSystem::cd(const vector<string>& path) {
	Directory* last_directory = current_directory;

	bool is_first = true;

	for (size_t i = 0; i < path.size(); i++) {

		if (path[i] == "..") {
			cd_up();
		}

		else if (is_first ? dir_exist(path[i]) : __check_dir_exist(path[i])) {
			cd_down(path[i]);
		}

		else if (is_first) {
			current_directory = &root;

			if (!__check_dir_exist(path[i])) {
				current_directory = last_directory;
				return false;
			}
			else cd_down(path[i]);
		}

		is_first = false;
	}
	//Terminal::set_pre_arrow_text(get_current_path() + ">");

	return true;
}

void FileSystem::cd() {
	current_directory = &root;
	current_path.clear();
}

bool FileSystem::cd_up() {
	if (current_directory->parent) {
		current_directory = current_directory->parent;
		current_path.pop();

		return true;
	}
	//Terminal::lnprint("Already at root directory");
	return false;
}

void FileSystem::cd_down(const string& name) {
	current_directory = &current_directory->directories[name];

	current_path.push(name);
}

void FileSystem::tree(const Directory& dir, ssize_t level) {
	//Terminal::lnprint(get_current_path());
	print_tree(dir, level);
	//print_tree(dir, level);
}

void FileSystem::print_tree(const Directory& dir, ssize_t level) {
	string indent = string(level * 2, ' ');

	dir.files.forEach([&](const string& name, const FileSystem::File&) {
		Terminal::lnprint(indent + "|-- " + name);
		});

	dir.directories.forEach([&](const string& name, const FileSystem::Directory& sub_dir) {
		Terminal::lnprint(indent + "|-- " + name + "/");

		print_tree(sub_dir, level + 1);
		});

}

bool FileSystem::__check_dir_exist(const string& path) {
	if (!dir_exist(path)) {
		Terminal::lnprint("Directory don`t exists");
		return false;
	}
	return true;
}

void FileSystem::set_current_path(const vector<string>& path) {
	current_path = path;
	//Terminal::set_pre_arrow_text(get_current_path() + ">");
}

string FileSystem::get_current_path() {

	string path = "";
	for (size_t i = 0; i < current_path.size(); i++) {
		path = path + current_path[i] + "/";
	}

	return path;
}

FileSystem::Directory* FileSystem::get_current_directory() {
	return current_directory;
}

const FileSystem::Directory& FileSystem::get_root() {
	return root;
}

bool FileSystem::__cd(const string& __path, string& buffer) {

	vector<string> path = split(__path, '/');

	string key = path.back();
	path.pop();

	if (!path.empty()) {
		if (!cd(path)) {
			return false;
		}
	}
	buffer = key;

	return true;
}