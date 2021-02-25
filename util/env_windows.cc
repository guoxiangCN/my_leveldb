#include "leveldb/env.h"
#include "leveldb/slice.h"
#include <Windows.h>
#include <thread>

namespace leveldb {

	/**
	 * @brief 
	*/
	class WindowsSequentialFile : public SequentialFile {
	public:

		WindowsSequentialFile(HANDLE h, std::string filename) 
			: handle_(h), filename_(std::move(filename)) {
			
		}

		~WindowsSequentialFile() override
		{
			if (handle_) {
				CloseHandle(handle_);
			}
		}

		virtual Status Read(size_t n, Slice* result, char* scratch) override
		{
			return Status();
		}

		virtual Status Skip(uint64_t n) override
		{
			return Status();
		}

	private:
		HANDLE handle_;
		std::string filename_;
	};

	class WindowsEnv : public Env {

		virtual Status NewSequentialFile(const std::string& fname, SequentialFile** result) override
		{
			
			return Status();
		}

		virtual Status NewRandomAccessFile(const std::string& fname, RandomAccessFile** result) override
		{
			return Status();
		}

		virtual Status NewWritableFile(const std::string& fname, WritableFile** result) override
		{
			return Status();
		}

		virtual Status NewAppendableFile(const std::string& fname, WritableFile** result) override
		{
			return Status();
		}

		virtual bool FileExists(const std::string& fname) override
		{
			return false;
		}

		virtual Status GetChildren(const std::string& dir, std::vector<std::string>* result) override
		{
			return Status();
		}

		virtual Status CreateDir(const std::string& dirname) override
		{
			return Status();
		}

		virtual Status GetFileSize(const std::string& fname, uint64_t* file_size) override
		{
			return Status();
		}

		virtual Status RenameFile(const std::string& src, const std::string& target) override
		{
			return Status();
		}

		virtual Status LockFile(const std::string& fname, FileLock** lock) override
		{
			return Status();
		}

		virtual Status UnlockFile(FileLock* lock) override
		{
			return Status();
		}

		virtual void Schedule(void(*func)(void* arg), void* arg) override
		{
		}

		virtual void StartThread(void(*func)(void* arg), void* arg) override
		{

		}

		virtual Status GetTestDirectory(std::string* path) override
		{
			return Status();
		}

		virtual Status NewLogger(const std::string& fname, Logger** result) override
		{
			return Status();
		}

		virtual uint64_t NowMicros() override
		{
			FILETIME ft;
			::GetSystemTimeAsFileTime(&ft);
			uint64_t num_ticks = (static_cast<uint64_t>(ft.dwHighDateTime) << 32) + ft.dwLowDateTime;
			return num_ticks / 10;
		}

		virtual void SleepForMicroseconds(int micros) override
		{
			std::this_thread::sleep_for(std::chrono::microseconds(micros));
		}
	};


	

}