#include <iostream>
#include <fstream>
#include <fcntl.h>      // open
#include <sys/types.h>  // fstat
#include <sys/stat.h>   // fstat
#include <sys/mman.h>   // mmap, munmap
#include <unistd.h>     // close, lseek

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>\n";
        return EXIT_FAILURE;
    }
    
    const char* filepath = argv[1];

    // 1. Create a dummy file if it doesn't exist

    // 2. Open the file with Read/Write permissions
    int fd = open(filepath, O_RDWR);
    if (fd == -1) {
        perror("Error opening file for RDWR");
        return EXIT_FAILURE;
    }
    
    // 3. Get the file size
    struct stat fileInfo;
    if (fstat(fd, &fileInfo) == -1) {
        perror("Error getting file size (fstat)");
        close(fd);
        return EXIT_FAILURE;
    }
    size_t file_size = fileInfo.st_size;

    std::cout << "Original file size: " << file_size << " bytes.\n";

    // 4. Map the file into virtual memory
    char* data = (char*)mmap(
        NULL,                               // Address: Let the kernel choose
        file_size,                          // Length: The size of the file
        PROT_READ | PROT_WRITE,             // Protection: Read and Write access
        MAP_SHARED,                         // Flags: Changes reflect in the file and are visible to other processes
        fd,                                 // File descriptor
        0                                   // Offset: Start mapping from the beginning of the file
    );

    if (data == MAP_FAILED) {
        perror("Error mapping the file (mmap)");
        close(fd);
        return EXIT_FAILURE;
    }
    
    char* cleanup_start = (char*) &&cleanup;

    

    cleanup:

    // 7. Unmap the memory
    if (munmap(data, file_size) == -1) {
        perror("Error un-mapping the file (munmap)");
        close(fd);
        return EXIT_FAILURE;
    }

    // 8. Close the file descriptor
    close(fd);

    return EXIT_SUCCESS;
}