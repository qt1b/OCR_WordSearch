#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h> // for filesize

/* Cette grille sera
constituée au minimum de cinq lignes et de cinq colonnes. 
	A passer en majuscule !
*/
char * grid_loader(char * path, int * len) {
	// gets the size of the file
	struct stat st;
	stat(path,&st);
	// printf("size of the given file is : %ld\n",st.st_size);
	// copies the content of the file to a string
	FILE *file_ptr = fopen(path,"r");
	if (NULL == file_ptr)
        	err(EXIT_FAILURE,"file can't be opened \n");
	// then get the number of lines
	len[0] = 0, len[1] = 0;
	char * grid = malloc(st.st_size);
	int x=0,y=0;
	// wip
	signed char ch;
	while (( ch = fgetc(file_ptr)) != EOF){
		if (ch == '\n') {
			if ( len[1] == 0 ) {
				len[1] = x;
			}
			y++;
			x=0;
		}
		else {
		       grid[y*len[1]+x] = ch;
		       x++;
		}
	}
	len[0] = y; // unsafe ? the file must have newline at the end
	// printf("size of the grid: nb of lines: %d ; nb of cols: %d\n",len[0],len[1]);
	return grid;
}

size_t get_word_len(char * word) {
	size_t i = 0;
	while (word[i] != '\0')
		i++;
	return i;
}


typedef enum {N,NE,E,SE,S,SW,W,NW} Direction; 

int * get_dir(Direction dir) {
	int * res = calloc(2,sizeof(int));
	switch (dir) {
		case N: 
			res[0] = -1;
			break;
		case NE: 
			res[0] = -1, res[1] = 1;
			break;
		case E: 
			res[1] = 1;
			break;
		case SE: 
			res[0] = 1, res[1] = 1;
			break;
		case S: 
			res[0] = 1;
			break;
		case SW: 
			res[0] = 1, res[1] = -1;
			break;
		case W: 
			res[1] = -1;
			break;
		case NW: 
			res[0] = -1, res[1] = -1;
			break;
		default: 
			err(1,"Invalid direction");
	}
	return res;
}

#define NO_OVERFLOW(y,x,max) ((y>=0 && y<max[0]) && (x>=0 && x<max[1]))

void solver(char * path, char * word) {
	int * len = malloc(sizeof(int) * 2);
	// len[0] = nb of lines ; len[1] = nb of columns
	int word_len = strlen(word);
	if (word_len < 2)
		err(EXIT_FAILURE,"Word's size is less than 2");
	for (int i = 0; i<word_len; i++)
		word[i] = (char)toupper((int)word[i]);
	char * grid = grid_loader(path,len);
	// char * res;
	// strcpy(res,"Not found");
	int y = 0;
	for (; y < len[0]; y++) {
		for (int x = 0; x < len[1]; x++) {
			// printf("%c ",grid[y*len[1]+x]);
			if (grid[y*len[1] + x] == word[0]) {
				for (Direction d = 0; d < 8; d++) {
					int * add = get_dir(d);
					// printf("d: %d, pos:y:%d ; x:%d",d,y+add[0],x+add[1]);
					int i = 1;
					for (; i<word_len && NO_OVERFLOW(y+add[0],x+add[1],len) &&
						grid[(y+add[0]*i)*len[1] +x+add[1]*i] == word[i]; i++) { 
						// printf("current pos:y:%d ; x:%d",y+add[0],x+add[1]*i); 
					}
					if (i == word_len) {
						i--;
						printf(/*res,19,*/"(%d,%d)(%d,%d)\n",x,y,x+add[1]*i,y+add[0]*i);
						y = len[0] + 1;
					}
					free(add);
				}
			}
		}
		// printf("\n");
	}
	if (y == len[0])
		printf("Not found\n");
	free(grid);
	free(len);
}

int main(int argc, char ** argv) {
	if (argc != 3)
		return -1;
	// printf("arg0:%s\narg1:%s\narg2:%s\n",argv[0],argv[1],argv[2]);
	solver(argv[1],argv[2]);
	return 0;
}
