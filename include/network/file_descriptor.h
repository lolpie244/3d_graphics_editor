#include <iostream>
#include <unistd.h>

namespace tcp_socket
{
class FileDescriptor
{
	int value;
	int* count;
public:
	FileDescriptor(): value(-1), count(new int(1)){};
	FileDescriptor(int value): value(value), count(new int(1)) {
		std::cout << "Create fd " << value << '\n';

	};
	FileDescriptor(FileDescriptor const &fd) {
		value = fd.value;
		count = fd.count;
		(*count)++;
	}
	FileDescriptor& operator=(const FileDescriptor& fd) {
		value = fd.value;
		count = fd.count;
		(*count)++;
		return *this;
	}

	operator int() const {
		return value;
	}
	bool operator==(const FileDescriptor& other_fd) {
		return value == other_fd.value;
	}

	~FileDescriptor() {
		(*count)--;
		if (*count == 0) {
			std::cout << "Close fd: " << value;
			::close(value);
			delete count;
		}
	}
};
} // namespace tcp_socket
