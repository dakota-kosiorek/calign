#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ERR 1
#define TRUE 1
#define FALSE 0

int get_cell(int *m, int rows, int r, int c) {
	return m[r * (rows + 1) + c];
}

int max(int a, int b, int c) {
	int m_abc, m_bc;

	// Get what is larger between b and c, m_bc
	if (b > c) m_bc = b;
	else m_bc = c;

	// Get what is larger betwen a and bc, m_abc
	if (a > m_bc) m_abc = a;
	else m_abc = m_bc;

	return m_abc;
}

int traceback(char *algn_code, int algn_size, int algn_max_size, 
		int s1_size, int s2_size, 
		int *m) {
	// Allocate space for the alignment 'code'
	if (algn_code == NULL) {
		algn_size = 0;
		algn_max_size = s1_size + s2_size;
		algn_code = malloc( (algn_max_size + 1) * sizeof(char));
		if (algn_code == NULL) {
			printf("Memory allocation error\n");
			return ERR;
		}

		for (int i = 0; i < algn_max_size; i++) algn_code[i] = 'E';
		algn_code[algn_max_size] = '\0';
	}

	// while row > 0 and column > 0
	int i = s2_size;
	int j = s1_size;

	// Get the alignment code
	while (i > 0 && j > 0) {
		int d = get_cell(m, s1_size, i - 1, j - 1);	// Diagonal score
		int h = get_cell(m, s1_size, i    , j - 1);	// Horizontal score
		int v = get_cell(m, s1_size, i - 1, j);	   	// Vertical score
		printf("D: %d, H: %d, V: %d\n", d, h, v);
		break;
		/*
		char c;
		int dir = max(d, h, v);

		if (dir == d) dir = 0;
		else if (dir == v) dir = 1;
		else if (dir == h) dir = 2;

		switch ( dir ) {
			case 0:
				c = 'D';
				i--;
				j--;

				if (s1[j - 1] == s2[i - 1]) score += match;
				else score += mismatch;

				break;
			case 1:
				i--;
				score += gap;
				c = 'V';
				break;
			case 2:
				j--;
				score += gap;
				c = 'H';
				break;
		}

		a_code[a_code_size] = c;
		a_code_size++;
		*/
	}

	// start at bottom right of the matrix
	// see if the left hoizontal, vetical, or left diagonal values are higher
	// IF none of those values match, go to the higher cell and continue the loop
	// ELSE rerun traceback starting at that cell with the initial alignment code done so far
	// Print alignment
	
	free(algn_code);
	return 0;
}

int main(int argc, char *argv[]) {
	int gap = -1;
	int mismatch = -1;
	int match = 1;
	int score = 0;

	int d, h, v;

	const char *valid_c = "ATGC";

	if (argc != 3) {
		printf("Too few arguments, './calign [SEQ 1] [SEQ 2]'\n");
		return ERR;
	}

	char *s1 = argv[1];
	char *s2 = argv[2];

	int s1_size = strlen(s1);
	int s2_size = strlen(s2);

	for (int i = 0; i < s1_size; i++) s1[i] = toupper(s1[i]);
	for (int i = 0; i < s2_size; i++) s2[i] = toupper(s2[i]);

	if (strspn(s1, valid_c) != s1_size || strspn(s2, valid_c) != s2_size) {
		printf("Sequences must have valid characters: 'A|T|G|C'\n");
		return ERR;
	}

	// Allocated space for alignment; 1D int array to be treated as a 2D matrix
	int *matrix = malloc( ((s1_size + 1) * (s2_size + 1)) * sizeof(int) );
	if (matrix == NULL) {
		printf("Memory allocation error\n");
		return ERR;
	}

	// Initial 1st row and column values
	for (int i = 0; i < s1_size + 1; i++) matrix[i] = i * gap;
	for (int i = 0; i < s2_size + 1; i++) matrix[(s1_size + 1) * i] = i * gap;

	// Alignment algorithm
	for (int i = 1; i < (s2_size + 1); i++) {
		for (int j = 1; j < (s1_size + 1); j++) {
			d = matrix[(i - 1) * (s1_size + 1) + (j - 1)]; 	// Diagonal score
			if (s1[j - 1] == s2[i - 1]) d += match;
			else d += mismatch;

			h = gap + matrix[i * (s1_size + 1) + (j - 1)];	// Horizontal score
			v = gap + matrix[(i - 1) * (s1_size + 1) + j];	// Vertical score

			matrix[i * (s1_size + 1) + j] = max(d, h, v);
		}
	}

	// Print matrix
	for (int i = 0; i < (s2_size + 2); i++) {
		for (int j = 0; j < (s1_size + 2); j++) {
			if (i == 0 && j == 0) 		printf("  ");
			else if (i == 0 && j == 1)	printf("%3c ", ' ');
			else if (i == 1 && j == 0)	printf("  ");
			else if (i == 0 && j > 1) 	printf("%3c ", s1[j - 2]);
			else if (i > 1 && j == 0) 	printf("%c ", s2[i - 2]);
			else if (i > 0 && j > 0) 	printf("%3d ", matrix[(i-1) * (s1_size + 1) + (j-1)]);
		} printf("\n");
	}

	// Traceback
	printf("\n");

	// TODO: fix the traceback so the BEST alignment(s) is shown
	traceback(NULL, 0, 0, s1_size, s2_size, matrix);
	// TODO: error handling where I free matrix and generated alignment code mem

	free(matrix);
	return 0;
}
