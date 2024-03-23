#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <elf.h>

// Stubs para chamadas de sistema a implementar em assembly no ficheiro isel_calls.s
// (por ordem de uso neste programa)
//
pid_t   isel_getpid();
void    isel_exit(int status);
int     isel_open(const char *pathname, int flags, ...);
off_t   isel_lseek(int fd, off_t offset, int whence);
int     isel_close(int fd);
void *  isel_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

#define RES_OK         0
#define ERR_BAD_ARGS   1
#define ERR_OPEN_FAIL  2
#define ERR_SEEK_FAIL  3
#define ERR_MMAP_FAIL  4
#define ERR_NOT_AN_ELF 5
#define ERR_UNSUPP_ELF 6

void * openAndMapFile(const char * fname, off_t * plen);

int inspectElf(const void * fptr, off_t len);

int main(int argc, const char * argv[]) {
	
	printf("PID: %d\n\n", isel_getpid());
	
	// Check arguments
	if (argc != 2) {
		fprintf(stderr, "use: %s filename\n", argv[0]);
		isel_exit(ERR_BAD_ARGS);
	}
	
	// Memory map the file to inspect
	off_t len;
	void * fptr = openAndMapFile(argv[1], &len);

	printf("File name: %s\n", argv[1]);
	printf("File size: %ld\n\n", len);

	// Inspect the file
	int res = inspectElf(fptr, len);
	
	printf("\nDone. [Press ENTER to terminate.]\n");
	getchar();
	return res;
}

void * openAndMapFile(const char * fname, off_t * plen) {

	// Open file to inspect
	int fd = isel_open(fname, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Failed to open file \"%s\" [ fd: %d, errno: %d ]\n", fname, fd, errno);
		isel_exit(ERR_OPEN_FAIL);
	}

	// Find file size
	off_t len = isel_lseek(fd, 0, SEEK_END);
	if (len < 0) {
		fprintf(stderr, "Failed to seek file \"%s\" [ fd: %d, errno: %d ]\n", fname, fd, errno);
		isel_close(fd);
		isel_exit(ERR_SEEK_FAIL);
	}

	// Memory map the whole file
	void * fptr = isel_mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
	if ((long)fptr <= 0) {
		fprintf(stderr, "Failed to mmap file \"%s\" [ fd: %ld, errno: %d ]\n", fname, (long)fptr, errno);
		isel_close(fd);
		isel_exit(ERR_MMAP_FAIL);
	}

	// File descriptor no longer needed after memory mapping the file
	isel_close(fd);
	
	// Return the memory mapping and its length
	if (plen) *plen = len;
	return fptr;
}

int inspectElf(const void * fptr, off_t len) {

	// ELF Header
	Elf64_Ehdr * phdr = (Elf64_Ehdr *)fptr;
	
	// Basic check: is it an ELF?
	if (memcmp(phdr->e_ident, "\177ELF", 4) != 0) {
		fprintf(stderr, "Error: not an ELF file\n");
		return ERR_NOT_AN_ELF;
	}
	
	// Basic check: is it in a supported format?
	if ( phdr->e_ident[EI_CLASS]      != ELFCLASS64      ||
	     phdr->e_ident[EI_DATA]       != ELFDATA2LSB     ||
	     phdr->e_ident[EI_VERSION]    != EV_CURRENT      ||
	    (phdr->e_ident[EI_OSABI]      != ELFOSABI_SYSV   &&
	     phdr->e_ident[EI_OSABI]      != ELFOSABI_LINUX) ||
	     phdr->e_ident[EI_ABIVERSION] != 0
	) {
		fprintf(stderr, "Error: unsupported ELF format\n");
		return ERR_UNSUPP_ELF;
	}

	// Print summary
	if (phdr->e_entry) {
		printf("Entry point        : %p\n", (void *)phdr->e_entry);
	}
	printf("Number of sections : %u\n", phdr->e_shnum);

	return RES_OK;
}
