#include <mpi.h>
#include "pbm.h"
#include "helper.h"

int main(int argc, char *argv[]) {
    image image_boulevard, image_boulevard_out;
    int width, height;
    int numProc, myRank;
    double start = 0, end = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    const int filterSize = 3;
    int LP[filterSize][filterSize] = {{1, 1, 1},
                                      {1, 1, 1},
                                      {1, 1, 1}};

    if (myRank == 0) {
        readInput("../sources/boulevard.pgm", &image_boulevard);
        image_boulevard_out.height = image_boulevard.height;
        image_boulevard_out.width = image_boulevard.width;
        image_boulevard_out.maxValue = image_boulevard.maxValue;
        memcpy(image_boulevard_out.type, image_boulevard.type, TYPE_LEN + 1);

        height = image_boulevard.height;
        width = image_boulevard.width;
        image_boulevard_out.pixel = (uchar *) malloc(
                sizeof(uchar) * image_boulevard_out.height * image_boulevard_out.width);
        printInfo(image_boulevard);
        cout << endl;

        /*
         * Moving pixels to output (they are ignored later)
         */
        for (int i = 0; i < width * height; i++) {
            if (i < width)
                image_boulevard_out.pixel[i] = image_boulevard.pixel[i];
            if (i > width * height - width)
                image_boulevard_out.pixel[i] = image_boulevard.pixel[i];
            if (i % width == 0)
                image_boulevard_out.pixel[i] = image_boulevard.pixel[i];
            if ((i % width - (width - 1)) == 0)
                image_boulevard_out.pixel[i] = image_boulevard.pixel[i];
        }

        start = MPI_Wtime();
    }
    // sending information about width and height to subprocesses
    MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int local_counts[numProc];
    int offsets[numProc];

    for (int i = 0; i < numProc; i++) {
        /*
         * We set up number of data to send:
         * width of image * height w/o first and last row for single process and we add LP padding
         */
        local_counts[i] = width * ((height - 1) / numProc + 2);
    }
    /*
     * First process rank=0 will have to do additional (remainder) not taken in local_counts for other process
     */
    local_counts[0] += ((height - 1) % numProc) * width;

    /*
     * First don't have an offset
     */
    offsets[0] = 0;
    for (int i = 1; i < numProc; i++) {
        /*
         * The rest of process will have offset given as number of taken data + previous offset - two rows (we need
         * three to make calculations for only one middle row)
         */
        offsets[i] = local_counts[i - 1] + offsets[i - 1] - width * 2;
    }
    // local buffer for local process
    uchar *local_buffer = allocVector(local_counts[myRank]);

    //sending scattered data to all sub processes
    MPI_Scatterv(image_boulevard.pixel, local_counts, offsets, MPI_UNSIGNED_CHAR, local_buffer, local_counts[myRank],
                 MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    uchar *local_buffer_output = allocVector(local_counts[myRank]);

    /*
     * For each element we are counting the filter value
     */
    for (int i = 1; i < local_counts[myRank] - 1; i++) {
        int average = local_buffer[i - width - 1] * LP[0][0] +
                      local_buffer[i - width] * LP[0][1] +
                      local_buffer[i - width + 1] * LP[0][2] +
                      local_buffer[i - 1] * LP[1][0] +
                      local_buffer[i] * LP[1][1] +
                      local_buffer[i + 1] * LP[1][2] +
                      local_buffer[i + width - 1] * LP[2][0] +
                      local_buffer[i + width] * LP[2][1] +
                      local_buffer[i + width + 1] * LP[2][2];
        int ourPoint = average / (
                LP[0][0] + LP[0][1] + LP[0][2] + LP[1][0] + LP[1][1] + LP[1][2] + LP[2][0] + LP[2][1] + LP[2][2]
        );
        local_buffer_output[i] = ourPoint;
    }

    /*
     * We recalculate AGAIN local counts and offset for making output (almost mirror action)
     */
    for (int i = 0; i < numProc; i++) {
        local_counts[i] = width * ((height - 2) / numProc);
    }
    local_counts[0] += ((height - 2) % numProc) * width;

    offsets[0] = width;
    for (int i = 1; i < numProc; i++) {
        offsets[i] = local_counts[i - 1] + offsets[i - 1];
    }
    // gathering data to pixel in out output from all local processes
    MPI_Gatherv(&local_buffer_output[width], local_counts[myRank], MPI_UNSIGNED_CHAR, image_boulevard_out.pixel,
                local_counts, offsets, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    MPI_Finalize();

    if (myRank == 0) {
        end = MPI_Wtime();

        writeData("../output/mpi_boulevard.pgm", &image_boulevard_out);
        cout << "end of program, working time: " << end - start << endl;
    }

    return 0;
}