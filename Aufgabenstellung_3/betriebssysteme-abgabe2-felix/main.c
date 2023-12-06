#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// Struct for the file header (size: 14 Bytes)
#pragma pack(push, 1)
typedef struct {
    char signature[2];
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
} FileHeader;
#pragma pack(pop)

// BMP Header Struct (size: 40 Bytes)
#pragma pack(push, 1)
typedef struct {
    uint32_t headerSize;
    int32_t imageWidth;
    int32_t imageHeight;
    uint16_t countOfColourLayers;
    uint16_t bitDepth;
    uint32_t compressionMethod;
    uint32_t imageSize;
    int32_t horizontalRes;
    int32_t vertikalRes;    
    uint32_t countPaletteColors;
    uint32_t countColors;
} BMPHeader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} RGBPixel;
#pragma pack(pop)

#define BMP_HEADER_SIZE (sizeof(FileHeader)+sizeof(BMPHeader))

int main(int argc, char *argv[]) {

    //Print Header for Debugging
    printf("FileHeader: %d, BMPHeader: %d\n", (int)sizeof(FileHeader),(int)sizeof(BMPHeader));
    
    // Max amount of arguments = 2
    if (argc > 3 || argc < 2) {
        printf("Usage: %s <name of file> [tile size]\n", argv[0]);
        exit(1);
    }
    
    // Save size of tiles (für Olya: Kachel)
    int tileSize = 4;
    if (argc >= 3) {
        tileSize = atoi(argv[2]);
    }

    // Open the bitmap file
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf("Error: Failed to open file: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    FileHeader originalFileHeader;
    ssize_t bytes_read = read(fd, &originalFileHeader, sizeof(originalFileHeader));
    // If there are less bytes read, then the file is not valid
    if (bytes_read != sizeof(originalFileHeader)) {
        printf("Error: Invalid file header\n");
        exit(EXIT_FAILURE);
    }

    BMPHeader originalBMPHeader;
    bytes_read = read(fd, &originalBMPHeader, sizeof(originalBMPHeader));
    // If there are less bytes read, then the file is not a valid bitmap file
    if (bytes_read != sizeof(originalBMPHeader)) {
        printf("Error: Invalid bitmap header\n");
        exit(EXIT_FAILURE);
    }

    // Check the file signature to make sure it's a bitmap file
    if (originalFileHeader.signature[0] != 'B' || originalFileHeader.signature[1] != 'M') {
        printf("Error: It's not a bitmap image\n");
        exit(EXIT_FAILURE);
    }

    // Get the image size, width, height and bit depth
    uint32_t image_size = originalFileHeader.fileSize;
    int32_t width = originalBMPHeader.imageWidth;
    int32_t height = originalBMPHeader.imageHeight;
    uint16_t bit_depth = originalBMPHeader.bitDepth;

    // Print the image size, width, height and bit depth for debugging purposes
    printf( "width: %i, height: %i, size: %i\n", width, height, image_size);

    // Allocate memory for the pixel data
    unsigned char* pixel_data = (unsigned char*) malloc(image_size - BMP_HEADER_SIZE);
    // If the memory allocation fails, exit the program
    if (pixel_data == NULL) {
        printf("Error: Failed to allocate memory for pixel data\n");
        exit(EXIT_FAILURE);
    }

    // Read the pixel data
    bytes_read = read(fd, pixel_data, image_size - BMP_HEADER_SIZE);
    if (bytes_read != image_size - BMP_HEADER_SIZE) {
        printf("Error: Failed to read pixel data\n");
        exit(EXIT_FAILURE);
    }
    
    // Close the bitmap file
    close(fd);

    // Pixel data is upside-down, so the first pixel in the data is the pixel on the bottom left.
    // So, to get the value of a pixel with given coordinates x and y, this circumstance has to be considered.
    // However, this only applies when width and height are positive.

    int bytes_per_pixel = bit_depth / 8; // Careful: this only works for bit depths which are a multiples of 8
    int pixel_bytes_per_row = width * bytes_per_pixel;

    // Calculate the amount of necessary bytes for each row so that the next row starts at a 4-byte boundary.
    // This is done by adding 3 to the amount of bytes in a row and then masking the last two bits.
    // A more exhaustive calculation would be something like ((width * bit_depth + 31) / 32) * 4;
    int total_bytes_per_row = (pixel_bytes_per_row + 3) & ~3;

    // The padding size is the number of bytes which are added to the end of each row, so that the next row starts at a 4-byte boundary.
    // E.g. if the image width is 15 pixels, the row size is 48 bytes (15 pixels * 3 bytes per pixel + 3 bytes padding).
    // Calculate the padding size by subtracting the amount of pixel bytes in a row from the amount of total bytes in a row.
    // int padding_size = total_bytes_per_row - pixel_bytes_per_row;

    // Only bitmaps with a bit depth of 24 bit and positive width and height are considered.
    // That means e.g. assuming the bottom left is (0|0), (0|14) should therefore be the top left pixel of the resulting image,
    // so it is the first pixel of the last row which starts at k = height-1 * width * 3 + (height-1) * padding_size.

    // At position k the byte corresponds to the blue value of the pixel. The next byte (+1) is green and +2 is red.
    // This is because the pixel values are stored in reverse order (BGR).

    // Print pixel data
    // for (int i = 0; i < height; i++) {
    //     int offset = i * pixel_bytes_per_row + i * padding_size;
    //     for (int j = 0, k = offset; j < width; j++, k += bytes_per_pixel) {
    //         printf("%02x%02x%02x ", pixel_data[k + 2], pixel_data[k + 1], pixel_data[k]);
    //     }
    //     offset += pixel_bytes_per_row;
    //     for (int j = 0; j < padding_size; j++) {
    //         printf("%02x ", pixel_data[offset + j]);
    //     }
    //     printf("\n");
    // }

    // Make mosaik
    for (int curRow = 0; curRow < height; curRow+=tileSize) {
        for (int curColumn = 0; curColumn < width; curColumn+=tileSize) {
            long sumBlue = 0, sumGreen = 0, sumRed = 0;
            int count = 0;

            for(int y_t = curRow; y_t < curRow+tileSize && y_t < height; y_t++){
                for(int x_t = curColumn; x_t < curColumn+tileSize && x_t < width; x_t++){
                    int pixel = y_t * total_bytes_per_row + x_t * bytes_per_pixel;
                    // Summ up all colors
                    sumBlue += pixel_data[pixel];
                    sumGreen += pixel_data[pixel + 1];
                    sumRed += pixel_data[pixel + 2];
                    count++;
                }
            }

            // divide colors
            sumBlue /= count;
            sumGreen /= count;
            sumRed /= count;

            // write colours
            for(int y_t = curRow; y_t < curRow+tileSize && y_t < height; y_t++){
                for(int x_t = curColumn; x_t < curColumn+tileSize && x_t < width; x_t++){
                    int pixel = y_t * total_bytes_per_row + x_t * bytes_per_pixel;
                    // Summ up all colors
                    pixel_data[pixel] = sumBlue;
                    pixel_data[pixel + 1] = sumGreen;
                    pixel_data[pixel + 2] = sumRed;
                }
            }

            // int corner = y * (pixel_bytes_per_row + padding_size) + x * bytes_per_pixel;
            // pixel_data[corner] = pixel_data[corner + 1] = pixel_data[corner + 2] = 0xFF;
        }
    }

    // Create a new file
    char new_filename[256];
    for(int i = 0; argv[1][i] != '.'; i++){
        new_filename[i] = argv[1][i];
    }
    char suffix[64];
    sprintf(suffix, "_mosaic_%d.bmp", tileSize);
    strcat(new_filename, suffix);
    fd = open(new_filename, O_CREAT | O_WRONLY, 0644);
    if (fd < 0) {
        printf("Error: Failed to create new file");
        exit(EXIT_FAILURE);
    }
    
    // Write the file header of the new file
    ssize_t bytes_written = write(fd, &originalFileHeader, sizeof(originalFileHeader));
    if (bytes_written != sizeof(originalFileHeader)) {
        printf("Error: Failed to write file header\n");
        exit(EXIT_FAILURE);
    }

    // Write the bitmap header of the new file
    bytes_written = write(fd, &originalBMPHeader, sizeof(originalBMPHeader));
    if (bytes_written != sizeof(originalBMPHeader)) {
        printf("Error: Failed to write bitmap header\n");
        exit(EXIT_FAILURE);
    }

    // Write the pixel data of the new file
    bytes_written = write(fd, pixel_data, image_size - BMP_HEADER_SIZE);
    if (bytes_written != image_size - BMP_HEADER_SIZE) {
        printf("Error: Failed to write pixel data\n");
        exit(EXIT_FAILURE);
    }

    printf("File saved as %s\n", new_filename);

    close(fd);

    // Free memory
    free(pixel_data);

    return 0;
}
