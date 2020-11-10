#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <getopt.h>

#define INIT_LINES_NUM 300
#define INIT_PGS_NUM 25
#define DELIM "\n"

typedef struct paragraph	//holds begining of the paragraph as pointer to *char array
{							
	char **pg_start;
}paragraph;

char * read_file(char *filepath)	//finds out size of the file and read it whole into the buffer
{
	int file_length = 0;
	char *file_content = NULL;
	
	FILE * fp = fopen(filepath, "r");
	if(fp == NULL)
	{
		perror("Cannot open the file: ");
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	file_length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	file_content = malloc(file_length);
	if(file_content != NULL)
	{
		fread(file_content, 1, file_length, fp);
	}
	
	fclose(fp);
	return file_content;
}


int parse_lyrics(char **file_content, char *lines[], paragraph *pgs)	//parse input with strtok 
{
	
	char *token = NULL;
	char *saveptr = NULL;
	char *line = NULL;
	int line_length = 0, line_counter = 0, pg_counter = 0, new_pg = 1;
		
	
	for(token = __strtok_r(*file_content, DELIM, &saveptr);
		token != NULL;
		token = __strtok_r(NULL, DELIM, &saveptr))
		{
			line_length = strlen(token);
			line = malloc(line_length + 1);
			memcpy(line, token, line_length + 1);
			
			lines[line_counter] = line;
			if(new_pg)
			{
				new_pg = 0;
				pgs[pg_counter].pg_start = &lines[line_counter]; //saves pointer to start of the paragraph
				pg_counter++;
			}
			line_counter++;
			line = NULL;
			
			if(*saveptr == '\n')
			{
				new_pg = 1;
				lines[line_counter] = NULL;
				line_counter++;
			}
		}
	lines[line_counter + 1] = NULL;
	
	return pg_counter;
}

void shuffle(paragraph *pg, int size)						//shuffles paragraphs of lyrics
{   
    if(size > 1)
    {
    	for(int i = size - 1; i > 0 ; i--)
    	{
    		int j = rand() % (i+1);
    		char **line = pg[j].pg_start;
    		pg[j].pg_start = pg[i].pg_start;
    		pg[i].pg_start = line;
    		line = NULL;
    	}
    }
}

void shuffle_lines(char *lines[])	//shuffles lines in every paragraph
{	
	int lines_cnt = 0;
	char *actual_line = NULL;
	
	do
	{
		lines_cnt++;

	}
	while(lines[lines_cnt] != NULL);
	
	if(lines_cnt > 1)
    {
    	for(int i = lines_cnt - 1; i > 0 ; i--)
    	{
    		int j = rand() % (i);
    		memcpy(&actual_line, &lines[j], sizeof(char *));
    		lines[j] = lines[i];
    		memcpy(&lines[i], &actual_line, sizeof(char *));
    		actual_line = NULL;
    	}
    }		
}

void print(paragraph *pg, int size, int sfl_lines)	//prints lyrics with shuffled paragraphs
{
	char *actual_line = NULL;
	int line_cnt = 0;

	for(int itr = 0; itr < size; itr++)
	{
		if(sfl_lines)
		{
			shuffle_lines(pg[itr].pg_start);	// if sfl_lines is set as input arg it will shuffle lines in paragraph before print them
		}
		actual_line = *pg[itr].pg_start;
		do
		{				
			printf("%s\n",actual_line);
			free(*(pg[itr].pg_start + line_cnt));	//after line is printed it is freed
			line_cnt++;
			actual_line = *(pg[itr].pg_start + line_cnt);
		}
		while(actual_line != NULL);
		line_cnt = 0;
		printf("\n");
	}
}

void print_help()	// prints help output
{
	char *help_output = "\nProgram will reshuffle lyrics in a file and print them on STDOUT\n"
				"./shuffle_lyrics [OPTIONS]\n"
				"-f [PATH] \t Path to file with lyrics.\n"
				"-l \t\t Will shuffle also lines in paragraphs. Optional\n";
	printf("%s",help_output);
}

int main(int argc, char *argv[])
{
	char *lines[INIT_LINES_NUM] = {NULL};
	paragraph pgs [INIT_PGS_NUM];
	char *file_content = NULL;
	char *filepath = NULL;
	int pg_counter = 0, opt, sfl_lines = 0;

	
	while ((opt = getopt (argc, argv, "lhf:")) != -1)	//process imput arguments
	{
		switch (opt)
	  	{
			case 'f':
				filepath = optarg;
			break;
			case 'l':
				sfl_lines = 1;
			break;
			case 'h':
				print_help();
			break;
			default:
				print_help();
				return 0;
			break;
	  }
	}	
	
	struct timeval tv;
    gettimeofday(&tv, NULL);
    int usec = tv.tv_usec;
    srand(usec);				//generate a seed for a rand()
	
	if((file_content = read_file(filepath)) == NULL)
	{
		return 0;
	}
	pg_counter = parse_lyrics(&file_content, &lines[0], &pgs[0]);
	shuffle(&pgs[0], pg_counter);	
	print(&pgs[0], pg_counter, sfl_lines);

	free(file_content);
	return 0;
}
