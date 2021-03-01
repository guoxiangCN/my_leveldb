#include "leveldb/env.h"
#include "leveldb/slice.h"
#include <Windows.h>
#include <thread>

namespace leveldb {

	namespace {

		/**
		 * @brief 写入文件的用户态缓存区大小
		*/
		constexpr const size_t kWritableFileBufferSize = 65536;

		/**
		 * @brief 默认的mmap上限, 64位系统上1000个,32位系统上为0
		*/
		constexpr const int kDefaultMmapLimit = (sizeof(void*) > 8) ? 1000 : 0;

		/**
		 * @brief 全局变量 mmap的限制数
		*/
		int g_mmap_limit = kDefaultMmapLimit;

		/**
		 * @brief Windows平台上从error_code获取error_msg
		 * @param error_code 错误码
		 * @return 错误信息
		*/
		std::string GetWindowsErrorMessage(DWORD error_code)
		{
			std::string message;
			char* error_text = nullptr;
			// Use MBCS version of FormatMessage to match return value.
			size_t error_text_size = ::FormatMessageA(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				reinterpret_cast<char*>(&error_text), 0, nullptr);
			if (!error_text) {
				return message;
			}
			message.assign(error_text, error_text_size);
			::LocalFree(error_text);
			return message;
		}

		Status WindowsError(const std::string& context, DWORD error_code) {
			if (error_code == ERROR_FILE_NOT_FOUND || error_code == ERROR_PATH_NOT_FOUND)
				return Status::NotFound(context, GetWindowsErrorMessage(error_code));
			return Status::IOError(context, GetWindowsErrorMessage(error_code));
		}

		/**
		 * @brief 对windows HANDLE的封装
		*/
		class ScopedHandle {
		public:
			ScopedHandle(HANDLE handle) : handle_(handle) {}
			
			ScopedHandle(const ScopedHandle&) = delete;
			
			ScopedHandle(ScopedHandle &&other) noexcept : handle_(other.Release()){}
			
			~ScopedHandle() { Close(); }

			ScopedHandle& operator=(const ScopedHandle&) = delete;
			
			ScopedHandle& operator=(ScopedHandle&& rhs) noexcept 
			{
				if (this != &rhs) 
				{
					handle_ = rhs.Release();
				}
				return *this;
			}

			bool Close() 
			{
				if (!IsValid())
				{
					return true;
				}
				HANDLE h = handle_;
				handle_ = INVALID_HANDLE_VALUE;
				return ::CloseHandle(h);
			}

			bool IsValid() const { return handle_ != nullptr && handle_ != INVALID_HANDLE_VALUE; }

			HANDLE Get() const { return handle_; }

			/**
			 * @brief 将当前HANDLE设置成无效值，并返回旧的HADNLE
			 * @return 
			*/
			HANDLE Release() 
			{
				HANDLE old = handle_;
				handle_ = INVALID_HANDLE_VALUE;
				return old;
			}

		private:
			HANDLE handle_;
		};

	} // end of namespace {

	/**
	 * @brief 
	*/
	class WindowsSequentialFile : public SequentialFile {
	public:

		WindowsSequentialFile(HANDLE h, std::string filename) 
			: handle_(h), filename_(std::move(filename)) 
		{
			
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