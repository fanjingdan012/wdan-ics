/**************************************************************************
 Row/column summation functions.  Edit this file as follows:
 1. Put your name and Andrew ID in the comment below
 2. Implement different versions of the summation functions
 3. Edit the table at the end to include your implementations.  Put
 your best solutions (best row & column sum, best column sum)
 as the first two entries in this table.
 ***************************************************************************/

/**************************************************************************
 My student ID is 09302010035.
 My name is Wu Yanhong.
 ***************************************************************************/

#include  <stdio.h>
#include  <stdlib.h>
#include  "rowcol.h"
#include  <math.h>

/* Reference implementation of column sum. */
/* Compute sum of each column in matrix.  Note that the argument
 conventions are the same for row & column sum, but 2nd argument
 is never referenced
 */

void c_sum(matrix_t M, vector_t rowsum, vector_t colsum) {
	int i, j;

	for (j = 0; j < N; j++) {
		colsum[j] = 0;
		for (i = 0; i < N; i++)
			colsum[j] += M[i][j];
	}
}
// Change the loop sequence, so that it has better spatial locality.
// Unrolling *4*32
void c_sum1(matrix_t M, vector_t rowsum, vector_t colsum) {
	int i, j;
	for (j = 0; j < N; j++) {
		colsum[j] = 0;
	}
	// The outer loop unrolling *4.
	for (i = N - 1; i >= 3; i -= 4) {
		// The inner loop unrolling *32.
		for (j = N - 1; j >= 31; j -= 32) {
			colsum[j] += M[i][j] + M[i - 1][j] + M[i - 2][j] + M[i - 3][j];
			colsum[j - 1] += M[i][j - 1] + M[i - 1][j - 1] + M[i - 2][j - 1]
					+ M[i - 3][j - 1];
			colsum[j - 2] += M[i][j - 2] + M[i - 1][j - 2] + M[i - 2][j - 2]
					+ M[i - 3][j - 2];
			colsum[j - 3] += M[i][j - 3] + M[i - 1][j - 3] + M[i - 2][j - 3]
					+ M[i - 3][j - 3];
			colsum[j - 4] += M[i][j - 4] + M[i - 1][j - 4] + M[i - 2][j - 4]
					+ M[i - 3][j - 4];
			colsum[j - 5] += M[i][j - 5] + M[i - 1][j - 5] + M[i - 2][j - 5]
					+ M[i - 3][j - 5];
			colsum[j - 6] += M[i][j - 6] + M[i - 1][j - 6] + M[i - 2][j - 6]
					+ M[i - 3][j - 6];
			colsum[j - 7] += M[i][j - 7] + M[i - 1][j - 7] + M[i - 2][j - 7]
					+ M[i - 3][j - 7];
			colsum[j - 8] += M[i][j - 8] + M[i - 1][j - 8] + M[i - 2][j - 8]
					+ M[i - 3][j - 8];
			colsum[j - 9] += M[i][j - 9] + M[i - 1][j - 9] + M[i - 2][j - 9]
					+ M[i - 3][j - 9];
			colsum[j - 10] += M[i][j - 10] + M[i - 1][j - 10]
					+ M[i - 2][j - 10] + M[i - 3][j - 10];
			colsum[j - 11] += M[i][j - 11] + M[i - 1][j - 11]
					+ M[i - 2][j - 11] + M[i - 3][j - 11];
			colsum[j - 12] += M[i][j - 12] + M[i - 1][j - 12]
					+ M[i - 2][j - 12] + M[i - 3][j - 12];
			colsum[j - 13] += M[i][j - 13] + M[i - 1][j - 13]
					+ M[i - 2][j - 13] + M[i - 3][j - 13];
			colsum[j - 14] += M[i][j - 14] + M[i - 1][j - 14]
					+ M[i - 2][j - 14] + M[i - 3][j - 14];
			colsum[j - 15] += M[i][j - 15] + M[i - 1][j - 15]
					+ M[i - 2][j - 15] + M[i - 3][j - 15];
			colsum[j - 16] += M[i][j - 16] + M[i - 1][j - 16]
					+ M[i - 2][j - 16] + M[i - 3][j - 16];
			colsum[j - 17] += M[i][j - 17] + M[i - 1][j - 17]
					+ M[i - 2][j - 17] + M[i - 3][j - 17];
			colsum[j - 18] += M[i][j - 18] + M[i - 1][j - 18]
					+ M[i - 2][j - 18] + M[i - 3][j - 18];
			colsum[j - 19] += M[i][j - 19] + M[i - 1][j - 19]
					+ M[i - 2][j - 19] + M[i - 3][j - 19];
			colsum[j - 20] += M[i][j - 20] + M[i - 1][j - 20]
					+ M[i - 2][j - 20] + M[i - 3][j - 20];
			colsum[j - 21] += M[i][j - 21] + M[i - 1][j - 21]
					+ M[i - 2][j - 21] + M[i - 3][j - 21];
			colsum[j - 22] += M[i][j - 22] + M[i - 1][j - 22]
					+ M[i - 2][j - 22] + M[i - 3][j - 22];
			colsum[j - 23] += M[i][j - 23] + M[i - 1][j - 23]
					+ M[i - 2][j - 23] + M[i - 3][j - 23];
			colsum[j - 24] += M[i][j - 24] + M[i - 1][j - 24]
					+ M[i - 2][j - 24] + M[i - 3][j - 24];
			colsum[j - 25] += M[i][j - 25] + M[i - 1][j - 25]
					+ M[i - 2][j - 25] + M[i - 3][j - 25];
			colsum[j - 26] += M[i][j - 26] + M[i - 1][j - 26]
					+ M[i - 2][j - 26] + M[i - 3][j - 26];
			colsum[j - 27] += M[i][j - 27] + M[i - 1][j - 27]
					+ M[i - 2][j - 27] + M[i - 3][j - 27];
			colsum[j - 28] += M[i][j - 28] + M[i - 1][j - 28]
					+ M[i - 2][j - 28] + M[i - 3][j - 28];
			colsum[j - 29] += M[i][j - 29] + M[i - 1][j - 29]
					+ M[i - 2][j - 29] + M[i - 3][j - 29];
			colsum[j - 30] += M[i][j - 30] + M[i - 1][j - 30]
					+ M[i - 2][j - 30] + M[i - 3][j - 30];
			colsum[j - 31] += M[i][j - 31] + M[i - 1][j - 31]
					+ M[i - 2][j - 31] + M[i - 3][j - 31];
		}
		for (; j >= 0; j--) {
			colsum[j] += M[i][j];
		}
	}
	for (; i >= 0; i--) {
		for (j = N - 1; j >= 31; j -= 32) {
			colsum[j] += M[i][j];
			colsum[j - 1] += M[i][j - 1];
			colsum[j - 2] += M[i][j - 2];
			colsum[j - 3] += M[i][j - 3];
			colsum[j - 4] += M[i][j - 4];
			colsum[j - 5] += M[i][j - 5];
			colsum[j - 6] += M[i][j - 6];
			colsum[j - 7] += M[i][j - 7];
			colsum[j - 8] += M[i][j - 8];
			colsum[j - 9] += M[i][j - 9];
			colsum[j - 10] += M[i][j - 10];
			colsum[j - 11] += M[i][j - 11];
			colsum[j - 12] += M[i][j - 12];
			colsum[j - 13] += M[i][j - 13];
			colsum[j - 14] += M[i][j - 14];
			colsum[j - 15] += M[i][j - 15];
			colsum[j - 16] += M[i][j - 16];
			colsum[j - 17] += M[i][j - 17];
			colsum[j - 18] += M[i][j - 18];
			colsum[j - 19] += M[i][j - 19];
			colsum[j - 20] += M[i][j - 20];
			colsum[j - 21] += M[i][j - 21];
			colsum[j - 22] += M[i][j - 22];
			colsum[j - 23] += M[i][j - 23];
			colsum[j - 24] += M[i][j - 24];
			colsum[j - 25] += M[i][j - 25];
			colsum[j - 26] += M[i][j - 26];
			colsum[j - 27] += M[i][j - 27];
			colsum[j - 28] += M[i][j - 28];
			colsum[j - 29] += M[i][j - 29];
			colsum[j - 30] += M[i][j - 30];
			colsum[j - 31] += M[i][j - 31];
		}
		for (; j >= 0; j--) {
			colsum[j] += M[i][j];
		}
	}
}
/* Reference implementation of row & column sum */
/* Compute sum of each row & of each column in matrix */

void rc_sum(matrix_t M, vector_t rowsum, vector_t colsum) {
	int i, j;
	for (i = 0; i < N; i++) {
		rowsum[i] = colsum[i] = 0;
		for (j = 0; j < N; j++) {
			rowsum[i] += M[i][j];
			colsum[i] += M[j][i];
		}
	}
}
//Also change the loop sequence,and use the loop unrolling *2*8.
void rc_sum1(matrix_t M, vector_t rowsum, vector_t colsum) {
	int i, j, temp, temp2;
	//Initial handling.
	for (i = 0; i < N; i++) {
		rowsum[i] = colsum[i] = 0;
	}
	//the outer loop unrooling*2.
	for (i = N - 1; i >= 1; i -= 2) {
		temp = 0;
		temp2 = 0;
		//the inner loop unrolling*16.
		for (j = N - 1; j >= 15; j -= 16) {
			temp += M[i][j];
			temp += M[i][j - 1];
			temp += M[i][j - 2];
			temp += M[i][j - 3];
			temp += M[i][j - 4];
			temp += M[i][j - 5];
			temp += M[i][j - 6];
			temp += M[i][j - 7];
			temp += M[i][j - 8];
			temp += M[i][j - 9];
			temp += M[i][j - 10];
			temp += M[i][j - 11];
			temp += M[i][j - 12];
			temp += M[i][j - 13];
			temp += M[i][j - 14];
			temp += M[i][j - 15];
			colsum[j] += M[i][j];
			colsum[j - 1] += M[i][j - 1];
			colsum[j - 2] += M[i][j - 2];
			colsum[j - 3] += M[i][j - 3];
			colsum[j - 4] += M[i][j - 4];
			colsum[j - 5] += M[i][j - 5];
			colsum[j - 6] += M[i][j - 6];
			colsum[j - 7] += M[i][j - 7];
			colsum[j - 8] += M[i][j - 8];
			colsum[j - 9] += M[i][j - 9];
			colsum[j - 10] += M[i][j - 10];
			colsum[j - 11] += M[i][j - 11];
			colsum[j - 12] += M[i][j - 12];
			colsum[j - 13] += M[i][j - 13];
			colsum[j - 14] += M[i][j - 14];
			colsum[j - 15] += M[i][j - 15];
		}
		for (; j >= 0; j--) {
			temp += M[i][j];
			colsum[j] += M[i][j];
		}
		rowsum[i] = temp;
		temp2 = 0;
		for (j = N - 1; j >= 15; j -= 16) {
			temp2 += M[i - 1][j];
			temp2 += M[i - 1][j - 1];
			temp2 += M[i - 1][j - 2];
			temp2 += M[i - 1][j - 3];
			temp2 += M[i - 1][j - 4];
			temp2 += M[i - 1][j - 5];
			temp2 += M[i - 1][j - 6];
			temp2 += M[i - 1][j - 7];
			temp2 += M[i - 1][j - 8];
			temp2 += M[i - 1][j - 9];
			temp2 += M[i - 1][j - 10];
			temp2 += M[i - 1][j - 11];
			temp2 += M[i - 1][j - 12];
			temp2 += M[i - 1][j - 13];
			temp2 += M[i - 1][j - 14];
			temp2 += M[i - 1][j - 15];
			colsum[j] += M[i - 1][j];
			colsum[j - 1] += M[i - 1][j - 1];
			colsum[j - 2] += M[i - 1][j - 2];
			colsum[j - 3] += M[i - 1][j - 3];
			colsum[j - 4] += M[i - 1][j - 4];
			colsum[j - 5] += M[i - 1][j - 5];
			colsum[j - 6] += M[i - 1][j - 6];
			colsum[j - 7] += M[i - 1][j - 7];
			colsum[j - 8] += M[i - 1][j - 8];
			colsum[j - 9] += M[i - 1][j - 9];
			colsum[j - 10] += M[i - 1][j - 10];
			colsum[j - 11] += M[i - 1][j - 11];
			colsum[j - 12] += M[i - 1][j - 12];
			colsum[j - 13] += M[i - 1][j - 13];
			colsum[j - 14] += M[i - 1][j - 14];
			colsum[j - 15] += M[i - 1][j - 15];
		}
		for (; j >= 0; j--) {
			temp2 += M[i - 1][j];
			colsum[j] += M[i - 1][j];
		}
		rowsum[i - 1] = temp2;
	}
	for (; i >= 0; i--) {
		temp = 0;
		for (j = N - 1; j >= 15; j -= 16) {
			temp += M[i][j];
			temp += M[i][j - 1];
			temp += M[i][j - 2];
			temp += M[i][j - 3];
			temp += M[i][j - 4];
			temp += M[i][j - 5];
			temp += M[i][j - 6];
			temp += M[i][j - 7];
			temp += M[i][j - 8];
			temp += M[i][j - 9];
			temp += M[i][j - 10];
			temp += M[i][j - 11];
			temp += M[i][j - 12];
			temp += M[i][j - 13];
			temp += M[i][j - 14];
			temp += M[i][j - 15];
			colsum[j] += M[i][j];
			colsum[j - 1] += M[i][j - 1];
			colsum[j - 2] += M[i][j - 2];
			colsum[j - 3] += M[i][j - 3];
			colsum[j - 4] += M[i][j - 4];
			colsum[j - 5] += M[i][j - 5];
			colsum[j - 6] += M[i][j - 6];
			colsum[j - 7] += M[i][j - 7];
			colsum[j - 8] += M[i][j - 8];
			colsum[j - 9] += M[i][j - 9];
			colsum[j - 10] += M[i][j - 10];
			colsum[j - 11] += M[i][j - 11];
			colsum[j - 12] += M[i][j - 12];
			colsum[j - 13] += M[i][j - 13];
			colsum[j - 14] += M[i][j - 14];
			colsum[j - 15] += M[i][j - 15];
		}
		for (; j >= 0; j--) {
			temp += M[i][j];
			colsum[j] += M[i][j];
		}
		rowsum[i] = temp;
	}
}

/* This table contains a list, where each entry gives the name
 of an summation function,
 the value ROWCOL if the function sums both rows and columns or
 COL if it only sums columns.  It also has a a string describing
 the function.
 Use this table to create a list with a number of different solutions.
 Then pick the best ones for your actual solution
 */

rc_fun_rec rc_fun_tab[] = {

/* Your first entry should be the best column summation function */
{ c_sum1, COL, "Best column sum" },
/* Your second entry should be the best row & column summation function */
{ rc_sum1, ROWCOL, "Best row and column sum" },

{ c_sum, COL, "Column sum, reference implementation" },

{ rc_sum, ROWCOL, "Row and column sum, reference implementation" },

/* This is the null terminator that marks the end of the function list.
 Leave this here. */
{ NULL, ROWCOL, NULL } };
