#include "kernel/types.h"
#include "user/user.h"
# define N 10

//Matrix initilization
int A[N][N];
int B[N][N];
int C[N][N];


//Computing the row results
void compute_row(int row, int rowstore[N] ){

    for(int i = 0; i < N; i++){
        int sum = 0;
        for(int j = 0; j < N; j++){
            sum += A[row][j] * B[j][i];
        }
        rowstore[i] = sum;
    }
}
int main() {
    int p[2]; //defining pipe
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            A[i][j] = i + 1;
            B[i][j] = j + 1;
        }
    if(pipe(p) <0){
        printf("pipe error");//-1 if pipe not created
        exit(0);
    }
    for(int i = 0; i<N; i++){
    int pid = fork(); // parent has the pid of the child >0 and child has pid 0
    if(pid < 0){
        printf("fork error");
        exit(0);
    }
    else if (pid ==0){ // if child process then pid is zero
        int rowx[N];
        close(p[0]); // closing read end
        compute_row(i, rowx);
        write(p[1], &i, sizeof(int)); 
        write(p[1], rowx, sizeof(rowx)); // values in row
        close(p[1]);
        exit(0);
    }
}
    close(p[1]);
    for (int i = 0; i < N; i++) {
        int row_idx;
        int row[N];
        read(p[0], &row_idx, sizeof(int)); // buffer =  [ 9, row9, 1, row2, 2, row3]
        read(p[0], row, sizeof(row));
        for (int j = 0; j < N; j++) {
            C[row_idx][j] = row[j];
        }
    }

    close(p[0]);


    for (int i = 0; i < N; i++) {
        wait(0);
    }

    printf("Result matrix C = A x B:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
            
        }
        printf("\n");

    
}
exit(0);
}