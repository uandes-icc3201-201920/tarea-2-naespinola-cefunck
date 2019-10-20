/*
Main program for the virtual memory project.
Make all of your modifications to this file.
You may add or rearrange any code or data as you need.
The header files page_table.h and disk.h explain
how to use the page table and disk interfaces.
*/

#include "page_table.h"
#include "disk.h"
#include "program.h"
#include <stdbool.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int replacement_algorithm(char * algorithm)
{
	// inicializamos las variables necesarias
	int frame, nframes;
	if(!strcmp(algorithm,"rand")) {
	// elegir un marco victima de manera aleatoria
		frame = rand() % nframes;
	} else if(!strcmp(algorithm,"fifo")) {
	// elegir un marco victima de manera fifo (primero en entrar es el primero en salir)
		int next_out;
		frame = next_out;
	} else {
		fprintf(stderr,"unknown replacement algorithm: %s\n",algorithm);
		exit(1);
	}
	// retornar el numero de marco victima
	return frame;
}

int * get_frame_table(struct page_table *pt)
{
	// obtenemos la cantidad de marcos
	int nframes = page_table_get_npages(pt);
	// obtenemos la cantidad de páginas
	int npages = page_table_get_npages(pt);
	// creamos la tabla de marcos, tendra 0 si el marco esta libre y 1 si no
	int frame_table[nframes];
	// recorremos las paginas de la tabla
	for (size_t page_index = 0; page_index < npages; page_index++) {
		int assigned_frame, assigned_bits;
		page_table_get_entry(pt, page_index, assigned_frame,assigned_bits);
	// si la página tiene un marco asignado lo anotamos en la tabla de marcos
		if (assigned_frame != 0) {
			frame_table[page_index]	= 1;
		}else{
			frame_table[page_index]	= 0;
		}
	}
	return frame_table;
}

void page_fault_handler( struct page_table *pt, int page)
{
	printf("page fault on page #%d\n",page);
	// inicializamos las variables necesarias
	int frame, bits;
	bool free_frame_exist = false;
	int *frame_table;
	frame_table = get_frame_table(pt);
	int frame_table_length = page_table_get_nframes(pt);
	// debemos verificar si existen marcos libres
	// primero recorremos la tabla de marcos
	// usaremos 0 para indicar que el marco está libre y 1 en caso contrario
	for (size_t frame_index = 0; frame_index < frame_table_length; frame_index++) {
		if (frame_table[frame_index] == 0) {
			// en este caso el marco i está libre
			frame = frame_index;
			free_frame_exist = true;
			break;
		}
	}
	if (free_frame_exist) {
	// si existen marcos libres entonces debemos cargar la página a un marco libre
		printf("implementar caso si existen marcos libres\n");
	}else{
	// si no existen marcos libres debemos llamar al algoritmo de reemplazo de página
		printf("implementar caso si existen marcos libres\n");
		frame = replacement_algorithm("rand");
	//frame = replacement_algorithm("fifo");
	}
	// debemos definir los bits de protección
	bits = PROT_READ+PROT_WRITE+PROT_EXEC;
	// debemos actualizar la tabla de páginas con el numero del marco y los bits de protección
	page_table_set_entry(pt, page, frame, bits);
	page_table_print(pt);
	// debemos volver a ejecutar la instrucción
	//exit(1);
}

int main( int argc, char *argv[] )
{
	if(argc!=5) {
		printf("use: virtmem <npages> <nframes> <lru|fifo> <access pattern>\n");
		return 1;
	}

	int npages = atoi(argv[1]);
	int nframes = atoi(argv[2]);
	const char *replacement_algorithm = argv[3];
	const char *program = argv[4];

	struct disk *disk = disk_open("myvirtualdisk",npages);
	if(!disk) {
		fprintf(stderr,"couldn't create virtual disk: %s\n",strerror(errno));
		return 1;
	}


	struct page_table *pt = page_table_create( npages, nframes, page_fault_handler );
	if(!pt) {
		fprintf(stderr,"couldn't create page table: %s\n",strerror(errno));
		return 1;
	}

	char *virtmem = page_table_get_virtmem(pt);

	char *physmem = page_table_get_physmem(pt);

	if(!strcmp(program,"pattern1")) {
		access_pattern1(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"pattern2")) {
		access_pattern2(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"pattern3")) {
		access_pattern3(virtmem,npages*PAGE_SIZE);

	} else {
		fprintf(stderr,"unknown program: %s\n",argv[3]);

	}

	page_table_delete(pt);
	disk_close(disk);

	return 0;
}
