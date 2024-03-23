#include <stdio.h>

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <strings.h>
#include <limits.h>
#include <assert.h>

#include "search.h"
#include "chrono.h"

#include "countlatch.h"
#include "thread_pool.h"

#define NITERS 50
#define NVALS 5000000
#define NTHREADS 4
 
thread_pool_t tpool; 

// utilitary functions

// read a line from a file 
static int readline(FILE *f, char line[], int capacity) {//5call
	int i = 0, c;
	while (i < capacity - 1 && (c = fgetc(f)) != EOF && c != '\n')
		line[i++] = c;
	line[i] = 0;
	while (c != EOF && c != '\n') c = fgetc(f);
 	return (i != 0 || c != EOF) ? i : -1;
}

// check if the string "suffix" ends the string "s"
static bool strends(const char *s, const char *suffix) {//3call
	int is = strlen(s) - 1, ip = strlen(suffix) - 1;
	
	while (is >= 0 && ip >= 0 && s[is] == suffix[ip]) { is--; ip--; }

	return ip == -1;
}


// 
// auxiliary functions for search
//

// create a structure that will contain the search results
void search_result_init(search_result_t *res, int limit) {//1call
	bzero(res, sizeof(search_result_t));
	res->results = (fresult_t *) malloc(sizeof(fresult_t)*limit);
	res->results_capacity = limit;
}

// destroy the search results
void search_result_destroy(search_result_t *res) {
	for (int i = 0; i < res->total_results; ++i)  
		free(res->results[i].path);
	free(res->results);
}


/*
   if *fes is NULL force it to point to the next free fresult_t in res
   return false if no free fresult_t exists on res
*/
static bool enforce_result(search_result_t * res, fresult_t ** fres) {//6call
	if (*fres != NULL) return true;
	if (res->total_results == res->results_capacity) return false;
	*fres = res->results + res->total_results;
	res->total_results++;
	(*fres)->count = 0;
	return true;
}

/*
 * search a word in the file with the specified name collecting
 * the result in res. Errors are ignored, but if the found files limit
 * is achieved the fact is memorized in result
 */
static void search_text(const char * path, const char * to_find, search_result_t * res) { //4call
	FILE *f = fopen(path, "r");
	if (f == NULL) return;
	char line[MAX_LINE];

	fresult_t * presult = NULL;
	int status = OK;
	
	while (readline(f, line, MAX_LINE) != -1) {
		if (strstr(line, to_find) != NULL) {
			if (!enforce_result(res, &presult) ) {
				status = TOO_MANY_FOUND_FILES ;
				break;
			}
			presult->count++;
		}
	}
	if (presult != NULL) {
		presult->path = strdup(path);
		res->total_ocorrences += presult->count;
	}
	res->status |= status;
	fclose(f);
}


/*
 * Search the folder and corresponding sub-folders  
 * where to find files containing the string "to_find".
 * 
 * fills a result with the names and ocurrence count of the files that contains 
 * the text presented in "to_find"
 */
void search(const char * path, const char * to_find,  const char *suffix, search_result_t * res) {// procura pelas subpastas e na respetiva pasta ficheiros que terminam com a mesma extensão ex: ".txt" == "t" coloca os resultados das ocorrencias em res
//2call
	char buffer[MAX_PATH];		// auxiliary buffer

	DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(path))) //Abre uma diretoria passada como argumento
        return;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {// encontrou uma subpasta por isso ele utiliza recursividade e começa de novo só que dentro dessa pasta
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(buffer, MAX_PATH,  "%s/%s", path, entry->d_name);
            search(buffer, to_find, suffix, res);
        } else {
            if (strends(entry->d_name, suffix)) {//comparação feita pelo ultimo indice da extensão e arg[3]
			    snprintf(buffer, MAX_PATH, "%s/%s", path, entry->d_name); // junta o path com o ficheiro encontrado
				search_text(buffer, to_find, res);// procura pela palavra a ser encontrada
				res->total_processed++;
			}
        }
    }
    closedir(dir);
}

typedef struct sum_task {
	count_latch_t * cdl;
	int * begin;
	int * end;
	int total_proc;
}task_t;


void par_search(const char * path, const char * to_find,  const char *suffix, search_result_t * res) {
	char buffer[MAX_PATH];		
	DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(path))) 
        return;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
	         if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(buffer, MAX_PATH,  "%s/%s", path, entry->d_name);
            search(buffer, to_find, suffix, res);
        } else {
            if (strends(entry->d_name, suffix)) {
			    snprintf(buffer, MAX_PATH, "%s/%s", path, entry->d_name);
				search_text(buffer, to_find, res);
				res->total_processed++;
			}
        }
    }
    closedir(dir);
}


void show_results( const char *folder, search_result_t * res) {	//Realiza o print dos resultados ddepois de correr tudo
	
	int startidx = strlen(folder);
	for (int i = 0; i < res->total_results; ++i) {
		fresult_t* fres = res->results + i;
		printf("~%s(%d)\n", fres->path + startidx, fres->count);
	}
	
	printf("\n");
	printf("processed: %d\n", res->total_processed);
	printf("total found files: %d\n", res->total_results);
	printf("total word ocorrences: %d\n", res->total_ocorrences);
	
	if (res->status != OK) printf("warn: the found files limit was achieved\n"); 
}

int main(int argc, char *argv[]){

	printf("okay\n");
	if (argc != 4) {
		printf("usage: traverse <folder> <text> <suffix>\n");
		return 1;
	}
	
	search_result_t result;
	
	search_result_init(&result, 50000 /* found files limit */);
	thread_pool_init(&tpool, NTHREADS); //INIT THREADS

	search(argv[1], argv[2], argv[3],  &result);
	
	show_results(argv[1], &result);
	
	search_result_destroy(&result);
	thread_pool_cleanup(&tpool); //CLEAN THREADS
	 
	return 0;
}


