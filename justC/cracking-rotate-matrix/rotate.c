#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <unistd.h>

#define N   2
int matrix[2*N+1][2*N+1];
void print_matrix()
{
    int i=0, j=0;
    for (; i<=2*N; i++) {
        for (j=0; j<=2*N; j++) {
            printf ("%2d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
    
/**
 * The basic idea of 90 degree clockwise rotation is as follows:-
 * Imagine the given matrix is shifted down and left by 'N' units so that
 * the cell coordinates are now ranging between [-N, +N] and since this
 * matrix is of 'odd' dimension, the matrix[N][N] element now shifts and
 * becomes shifted_matrix[0][0]. So, now the job is to rotate this shifted_matrix
 * around its element [0][0]. So now, in this problem space, a point at
 * shifted_matrix[a][b], where a>0, b>0, moves to position [b][-a].
 * Similarly, the point at shifted_matrix[+b][-a] moves to position [-a][-b],
 * which in turn moves to [-b][+a], and finally, the shifted_matrix[-b][+a] moves
 * back into the original position [+a][+b]. This is what essentially is implemented
 * here.
 * Given a point [a,b] in shifted_matrix's space, the rotate_one_point(a,b) implements
 * this above rotation in-place, i.e. without requiring any extra space.
 * It uses the functions get_new_coord() and set_new_coord(), which are essentially
 * mapping a point (a,b) from shifted_matrix's space into the original matrix's space.
 * We just need to call rotate_one_point() for every point in the top-right "square"
 * in the original matrix, and then we have the entire matrix rotated.
 * There is a light boundary case, due to which the 
    for (i=N; i<=2*N; i++)
        rotate_one_point(i, N);
 * above loop is necessary. If the matrix were to be a 2Nx2N matrix subtle changes
 * have to be made.
 */
int get_new_coord(int p, int q)
{
    assert(p >= -N && p <= +N);
    assert(q >= -N && q <= +N);
    return matrix[p+N][q+N];
}
int set_new_coord(int p, int q, int val)
{
    assert(p >= -N && p <= +N);
    assert(q >= -N && q <= +N);
    return matrix[p+N][q+N] = val;
}


void rotate_one_point(int a, int b)
{
    assert(a >= N && a <= 2*N);
    assert(b >= N && b <= 2*N);
    int A = a-N;
    int B = b-N;
    int tmp = get_new_coord(-B,A); //only one extra space needed for the entire rotation
    set_new_coord(-B,A, get_new_coord(-A,-B));
    set_new_coord(-A,-B, get_new_coord(+B,-A));
    set_new_coord(+B,-A, get_new_coord(+A,+B));
    set_new_coord(+A,+B, tmp);
}

void rotate_matrix()
{
    int i,j;
    for (i=N+1; i<=2*N; i++)
        for (j=N+1; j<=2*N; j++)
            rotate_one_point(i,j);
    for (i=N; i<=2*N; i++)
        rotate_one_point(i, N);
}

int main()
{
    //initialize
    int i = 0, j = 0;
    for (; i<=2*N; i++) {
        for (j=0; j<=2*N; j++) {
            matrix[i][j] = (i);
        }
    }

    print_matrix();
    rotate_matrix();
    printf("Rotated matrix===>\n");
    print_matrix();
    rotate_matrix();
    printf("Rotated matrix===>\n");
    print_matrix();
    rotate_matrix();
    printf("Rotated matrix===>\n");
    print_matrix();
    rotate_matrix();
    printf("Rotated matrix===>\n");
    print_matrix();

    return 0;
}

