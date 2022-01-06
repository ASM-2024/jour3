#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	if (argc < 2)
		errx(1, "Usage: %s <PATH>", argv[0]);

	// Open file and handle non-existent file error case
	int fd = open(argv[1], 0, O_RDONLY);
	if (fd < 0)
		errx(1, "File not found: %s", argv[1]);

	// Create stat buffer to hold file info
	struct stat statbuf = { 0 };

	// Call fstat(3) == stat(3) but for file descriptors
	if (fstat(fd, &statbuf) == -1)
		errx(1, "Error reading file information");

	// Current total amount of bytes read
	ssize_t tot_read = 0;

	// Amount of read bytes
	ssize_t nread = 0;
	// Buffer for read bytes. 64 is chosen solely for demonstration purposes.
	// You should use sizes based on system information, so in most cases 4096
	// bytes as it corresponds to the size of a page of memory.
	char buf[64];

	// RPL = Read-Print Loop
	while (tot_read < statbuf.st_size)
	{
		nread = read(fd, buf, 64);
		tot_read += nread;

		// Standard streams have macros defined for their fd number
		// Use them to improve readability
		write(STDOUT_FILENO, buf, nread);
	}

	return 0;
}
