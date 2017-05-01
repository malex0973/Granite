#pragma once
#include "../filesystem.hpp"
#include <unordered_map>

namespace Granite
{
class MMapFile : public File
{
public:
	MMapFile(const std::string &path, FileMode mode);
	~MMapFile();
	void *map() override;
	void *map_write(size_t size) override;
	void unmap() override;
	size_t get_size() const override;
	bool reopen() override;

private:
	int fd = -1;
	void *mapped = nullptr;
	size_t size = 0;
};

class OSFilesystem : public FilesystemBackend
{
public:
	OSFilesystem(const std::string &base);
	~OSFilesystem();
	std::vector<ListEntry> list(const std::string &path) override;
	std::unique_ptr<File> open(const std::string &path, FileMode mode) override;
	bool stat(const std::string &path, FileStat &stat) override;
	FileNotifyHandle install_notification(const std::string &path, std::function<void (const FileNotifyInfo &)> func) override;
	FileNotifyHandle find_notification(const std::string &path) const override;
	void uninstall_notification(FileNotifyHandle handle) override;
	void poll_notifications() override;
	int get_notification_fd() const override;

private:
	std::string base;

	struct VirtualHandler
	{
		std::function<void (const FileNotifyInfo &)> func;
		FileNotifyHandle virtual_handle;
	};

	struct Handler
	{
		std::string path;
		std::vector<VirtualHandler> funcs;
		bool directory;
	};
	std::unordered_map<FileNotifyHandle, Handler> handlers;
	std::unordered_map<std::string, FileNotifyHandle> path_to_handler;
	std::unordered_map<FileNotifyHandle, FileNotifyHandle> virtual_to_real;
	int notify_fd;
	FileNotifyHandle virtual_handle = 0;
};
}