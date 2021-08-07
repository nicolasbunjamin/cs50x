#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc != 2)
    {
        // Valid usage must include one RAW file
        printf("Usage: recover image.raw\n");
        return 1;
    }

    // Open (read) the file
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Image cannot be opened for reading\n");
        return 1;
    }

    // JPEGs are stored in data blocks, each of which is 512 bytes long
    unsigned char data_block[BLOCK_SIZE];

    // Track number of JPEGs recovered
    int output_counter = 0;

    // Initialize boolean for writing action
    bool writing = false;

    // Initialize variable for output files
    FILE *output = NULL;

    // Run loop until program finds a data block less than 512 bytes long
    // Function fread returns the number of items read (i.e. 1)
    while (fread(data_block, BLOCK_SIZE, 1, input) == 1)
    {
        // Check if file type is JPEG through the first four bytes in the data block
        if (data_block[0] == 0xff && data_block[1] == 0xd8 & data_block[2] == 0xff && (data_block[3] & 0xf0) == 0xe0)
        {
            // Checking if an output file is currently being written
            if (writing == true)
            {
                // Close current JPEG
                fclose(output);
            }

            // Initalize a char array for output file names, 8 bytes long (###.jpg\0)
            // A string/pointer (char*) cannot be used because it's only 4 bytes long
            char output_file_name[8];
            // Name the output files as ###.jpg
            sprintf(output_file_name, "%03i.jpg", output_counter);
            // Create (write) output files
            output = fopen(output_file_name, "w");
            // Increment output counter
            output_counter++;
            // Start writing mode
            writing = true;
        }

        // Write onto output file
        if (writing == true)
        {
            fwrite(data_block, BLOCK_SIZE, 1, output);
        }
    }
    // Close everything
    fclose(input);
    fclose(output);
    return 0;
}
