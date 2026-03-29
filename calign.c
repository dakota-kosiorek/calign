#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int main(int argc, char *argv[]) {
	int gap = -1;
	int mismatch = -1;
	int match = 1;
	int score = 0;

	int i, j;
	int d, h, v;

	const char *valid_c = "ATGC";

	if (argc != 3) {
		printf("Too few arguments, './calign [SEQ 1] [SEQ 2]'\n");
		return -1;
	}

	char *s1 = argv[1];
	char *s2 = argv[2];

	int s1_size = strlen(s1);
	int s2_size = strlen(s2);

	for (i = 0; i < s1_size; i++) s1[i] = toupper(s1[i]);
	for (i = 0; i < s2_size; i++) s2[i] = toupper(s2[i]);

	if (strspn(s1, valid_c) != s1_size || strspn(s2, valid_c) != s2_size) {
		printf("Sequences must have valid characters: 'A|T|G|C'\n");
		return -1;
	}

	// Allocated space for alignment; 1D array to be treated as a 2D array
	int *matrix = malloc( ((s1_size + 1) * (s2_size + 1)) * sizeof(int) );
	if (matrix == NULL) {
		printf("Memory allocation error\n");
		return -1;
	}

	// Initial 1st row and column values
	for (i = 0; i < s1_size + 1; i++) matrix[i] = i * gap;
	for (i = 0; i < s2_size + 1; i++) matrix[(s1_size + 1) * i] = i * gap;

	// Alignment algorithm
	for (i = 1; i < (s2_size + 1); i++) {
		for (j = 1; j < (s1_size + 1); j++) {
			d = matrix[(i - 1) * (s1_size + 1) + (j - 1)]; 	// Diagonal score
			if (s1[j - 1] == s2[i - 1]) d += match;
			else d += mismatch;

			h = gap + matrix[i * (s1_size + 1) + (j - 1)];	// Horizontal score
			v = gap + matrix[(i - 1) * (s1_size + 1) + j];	// Vertical score

			matrix[i * (s1_size + 1) + j] = max(d, h, v);
		}
	}

	// Print matrix
	for (i = 0; i < (s2_size + 2); i++) {
		for (j = 0; j < (s1_size + 2); j++) {
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

	// Allocate space for the alignment 'code'
	int a_code_size = 0;
	int a_code_max_size = s1_size + s2_size;
	char *a_code = malloc( (a_code_max_size + 1) * sizeof(char));
	if (matrix == NULL) {
		printf("Memory allocation error\n");
		free(matrix);
		return -1;
	}

	for (int i = 0; i < a_code_max_size; i++) a_code[i] = 'E';
	a_code[a_code_max_size] = '\0';

	i = s2_size;
	j = s1_size;

	// Get the alignment code
	while (i > 0 && j > 0) {
		d = matrix[(i - 1)	* (s1_size + 1) + (j - 1)]; 	// Diagonal score
		h = matrix[i		* (s1_size + 1) + (j - 1)];	// Horizontal score
		v = matrix[(i - 1)	* (s1_size + 1) + j];		// Vertical score

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
	}

	a_code[a_code_size] = '\0';
	char *temp = realloc(a_code, a_code_size + 1);
	if (temp == NULL) {
		printf("Memory allocation error\n");
		free(matrix);
		free(a_code);
		return -1;
	}
	a_code = temp;

	// Reverse alignment code
	char *a_code_rev = malloc( (a_code_size + 1) * sizeof(char) );
	if (a_code_rev == NULL) {
		printf("Memory allocation error\n");
		free(matrix);
		free(a_code);
		return -1;
	}
	a_code_rev[a_code_size] = '\0';

	for (i = 0; i < a_code_size; i++) {
		a_code_rev[i] = a_code[a_code_size - i - 1];
	}

	free(a_code);

	// Print seq 1 alignment

	int offset = 0;
	printf("Score %d\n", score);

	for (i = 0; i < a_code_size; i++) {
		char c = a_code_rev[i];

		if (c == 'D' || c == 'H')
			c = s1[i - offset];
		else if (c == 'V') {
			c = '-';
			offset++;
		}

		printf("%c", c);
	} printf("\n");

	offset = 0;

	// Print seq 2 alignment
	for (i = 0; i < a_code_size; i++) {
		char c = a_code_rev[i];

		if (c == 'D' || c == 'V')
			c = s2[i - offset];
		else if (c == 'H') {
			c = '-';
			offset++;
		}

		printf("%c", c);
	} printf("\n");

	free(a_code_rev);
	free(matrix);

	return 0;
}
