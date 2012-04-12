

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>


bool makeDvdSlow(int fd, bool slow) {
	// see apple's IODVDStorageFamily driver, particularly
	// http://www.opensource.apple.com/source/IODVDStorageFamily/IODVDStorageFamily-30/IODVDMediaBSDClient.h
	//
	// XXX aka:  /System/Library/Frameworks/Kernel.framework/Versions/A/Headers/IOKit/storage/IODVDMediaBSDClient.h
	const unsigned long DKIOCDVDSETSPEED = _IOW('d', 131, uint16_t);

	uint16_t val = slow ? 1350 : 0xffff;

	return (0 == ioctl(fd, DKIOCDVDSETSPEED, &val));
}


int main(int argc, char** argv) {
	const char* errorstr = 0;
	int fd = -1;

	bool fast = (argc == 3) && (0 == strcmp(argv[2], "fast"));
	bool slow = (argc == 3) && (0 == strcmp(argv[2], "slow"));
	if (!fast && !slow) {
		fprintf(stderr, "usage: %s <filename> [fast | slow]\n", argv[0]);
		return 1;
	}

	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		errorstr = "during open";
		goto end;
	}

	if (!makeDvdSlow(fd, slow)) {
		errorstr = "while manipulating device";
		goto end;
	}

end:
	if (fd >= 0) {
		if (0 != close(fd)) {
			errorstr = "while closing";
		}
	}

	if (errorstr) {
		fprintf(stderr, "error %s: errno = %0d (%s)\n", errorstr, errno, strerror(errno));
		return 1;
	}

	return 0;
}

