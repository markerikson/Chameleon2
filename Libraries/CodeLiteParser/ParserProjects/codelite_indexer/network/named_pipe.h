#ifndef named_pipe_h__
#define named_pipe_h__

#ifdef __WXMSW__
// Windows
#include <WinSock2.h>
#include <Windows.h>
#include <string>

#define PIPE_HANDLE HANDLE
#define INVALID_PIPE_HANDLE INVALID_HANDLE_VALUE

#else

// *uix
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#define PIPE_HANDLE int
#define INVALID_PIPE_HANDLE -1
#endif

class  clNamedPipe
{
public:

	enum ZNP_ERROR {
		ZNP_OK = 0
		, ZNP_TIMEOUT
		, ZNP_CONNECT_ERROR
		, ZNP_CONNECT_WAIT_ERROR
		, ZNP_BAD_WRITE_SIZE
		, ZNP_READ_ERROR
		, ZNP_WRITE_ERROR
		, ZNP_IMPERSONATION_ERROR
		, ZNP_UNKNOWN
	};



	clNamedPipe(const char* path);


	virtual ~clNamedPipe();

	virtual bool write(const void* data, size_t dataLength, size_t *written, long timeToLive);

	virtual bool read(void* data, size_t dataLength, size_t *read, long timeToLive);

	virtual void disconnect() = 0;

	const char* getPipePath() const {
		return _pipePath.c_str();
	}

	ZNP_ERROR getLastError() {
		return _lastError;
	}

	void setLastError(ZNP_ERROR code) {
		_lastError = code ;
	}



protected:

	void setPipePath(const char* path) {
		_pipePath = path;
	}

	void setPipeHandle(PIPE_HANDLE handle) {
		_pipeHandle = handle;
	}

	PIPE_HANDLE _pipeHandle;

private:
	std::string _pipePath;
	ZNP_ERROR   _lastError;
};

#endif // named_pipe_h__
