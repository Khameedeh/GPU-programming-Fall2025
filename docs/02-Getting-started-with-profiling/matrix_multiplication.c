#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAXN 1000   // maximum possible size
#define ION  1000    // size for I/O mode
#define CPUN 1000   // size for CPU mode

void multiply(int A[MAXN][MAXN], int B[MAXN][MAXN], int C[MAXN][MAXN], int order, int n) {
    int i, j, k;
    switch(order) {
        case 0: // ijk
            for (i=0; i<n; i++)
                for (j=0; j<n; j++)
                    for (k=0; k<n; k++)
                        C[i][j] += A[i][k] * B[k][j];
            break;
        case 1: // ikj
            for (i=0; i<n; i++)
                for (k=0; k<n; k++)
                    for (j=0; j<n; j++)
                        C[i][j] += A[i][k] * B[k][j];
            break;
        case 2: // jik
            for (j=0; j<n; j++)
                for (i=0; i<n; i++)
                    for (k=0; k<n; k++)
                        C[i][j] += A[i][k] * B[k][j];
            break;
        default:
            fprintf(stderr,"Invalid loop order: %d\n", order);
            exit(1);
    }
}

void create_and_write_matrices(const char *filename, int n) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open for write");
        exit(1);
    }

    char buffer[16];
    for (int m=0; m<2; m++) {
        for (int i=0; i<n; i++) {
            for (int j=0; j<n; j++) {
                int len = snprintf(buffer, sizeof(buffer), "0 ");
                if (write(fd, buffer, len) != len) {
                    perror("write");
                    exit(1);
                }
            }
            if (write(fd, "\n", 1) != 1) {
                perror("write newline");
                exit(1);
            }
        }
    }

    close(fd);
    printf("Matrices written (syscall write) to file: %s\n", filename);
}

void read_matrix(const char *filename, int A[MAXN][MAXN], int B[MAXN][MAXN], int n) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open for read");
        exit(1);
    }

    char buf[32];
    int val = 0, sign = 1;
    int *matrices[2] = {&A[0][0], &B[0][0]};
    int current = 0, count = 0;

    ssize_t r;
    while ((r = read(fd, buf, sizeof(buf))) > 0) {
        for (ssize_t i=0; i<r; i++) {
            if (buf[i] == '-') {
                sign = -1;
            } else if (buf[i] >= '0' && buf[i] <= '9') {
                val = val*10 + (buf[i]-'0');
            } else {
                if (count < n*n*2) {
                    matrices[current][count % (n*n)] = sign*val;
                    count++;
                    if (count == n*n) current = 1;
                }
                val = 0;
                sign = 1;
            }
        }
    }

    if (r < 0) perror("read");
    close(fd);
}

int main(int argc, char *argv[]) {
    int order = 0;
    char *mode = "cpu";
    char *file = "./matrices.txt";
    int n;

    if (argc >= 2) {
        if (strcmp(argv[1], "ijk") == 0) order = 0;
        else if (strcmp(argv[1], "ikj") == 0) order = 1;
        else if (strcmp(argv[1], "jik") == 0) order = 2;
        else { fprintf(stderr,"Unknown loop order: %s\n", argv[1]); return 1; }
    }
    if (argc >= 3) {
        if (strcmp(argv[2], "cpu") == 0) mode = "cpu";
        else if (strcmp(argv[2], "io") == 0) mode = "io";
        else { fprintf(stderr,"Unknown mode: %s\n", argv[2]); return 1; }
    }

    static int A[MAXN][MAXN], B[MAXN][MAXN], C[MAXN][MAXN];

    if (strcmp(mode,"cpu") == 0) {
        n = CPUN;
        srand(time(NULL));
        for (int i=0; i<n; i++)
            for (int j=0; j<n; j++) {
                A[i][j] = rand() % 10;
                B[i][j] = rand() % 10;
                C[i][j] = 0;
            }
        printf("Mode: CPU-bound (random matrices, N=%d)\n", n);
    } else {
        n = ION;
        create_and_write_matrices(file, n);
        read_matrix(file, A, B, n);
        printf("Mode: I/O-bound (syscall read, N=%d)\n", n);
    }

    multiply(A, B, C, order, n);

    printf("Result[0][0] = %d\n", C[0][0]);
    return 0;
}
