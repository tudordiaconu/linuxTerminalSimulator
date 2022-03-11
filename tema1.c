#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

// structura de director
typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

// structura de fisier
typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

// functia de creare a unui fisier
void touch (Dir* parent, char* name) {
	// initializarea noului fisier
	File* new_file = malloc(sizeof(*new_file));
	new_file->parent = parent;
	new_file->next = NULL;


	int already_exists = 0;
	File* curr_file;
	Dir* curr_dir;

	// ne plimbam prin lista de directoare pentru a vedea daca exista deja
	// director cu acest nume
	if (parent->head_children_dirs) {
		curr_dir = parent->head_children_dirs;
		
		while(curr_dir && curr_dir->next) {
			if (strcmp(curr_dir->name, name) == 0) {
				printf("File already exists\n");
				curr_dir = curr_dir->next;
				already_exists = 1;

				free(new_file);
				return;
			}

			curr_dir = curr_dir->next;
		}

		// verificam si ultimul director din lista directoarelor
		if (strcmp(curr_dir->name, name) == 0) {
			printf("File already exists\n");
			curr_dir = curr_dir->next;
			
			free(new_file);
			already_exists = 1;
			return;
		}
	}

	if(already_exists == 0) {
		// verificam existenta numelui in lista fisierelor
		if(parent->head_children_files) {
			curr_file = parent->head_children_files;

			while(curr_file && curr_file->next) {
				//cazul in care exista deja fisier cu numele dorit
				if(strcmp(curr_file->name, name) == 0) {
					printf("File already exists\n");
					curr_file = curr_file->next;

					free(new_file);
					return;
				}

				curr_file = curr_file->next;
			}

			// verificam si ultimul fisier
			if(strcmp(curr_file->name, name) == 0) {
				printf("File already exists\n");

				free(new_file);
				return;
			} else {
				// daca ajungem aici inseamna ca nu exista alt fisier/director
				// cu numele dorit, deci puteam crea fisierul
				curr_file->next = new_file;
				new_file->name = strdup(name);
			}

		// cazul in care fisierul creat este primul fisier din parinte
		} else {
			parent->head_children_files = new_file;
			new_file->name = strdup(name);
		}
	}
}

// functia de creare a unui director
void mkdir (Dir* parent, char* name) {
	// initializarea noului director
	Dir* new_dir = malloc(sizeof(*new_dir));
	new_dir->parent = parent;
	new_dir->next = NULL;
	new_dir->head_children_dirs = NULL;
	new_dir->head_children_files = NULL;

	int already_exists = 0;
	File* curr_file;
	Dir* curr_dir;

	// verificam daca exista deja un fisier care are numele dorit
	if(parent->head_children_files) {
		curr_file = parent->head_children_files;

		while(curr_file && curr_file->next) {
			if(strcmp(curr_file->name, name) == 0) {
				printf("Directory already exists\n");
				already_exists = 1;
				curr_file = curr_file->next;
				free(new_dir);
				return;
			}

			curr_file = curr_file->next;
		}

		// verificam si ultimul fisier din lista
		if(strcmp(curr_file->name, name) == 0) {
			printf("Directory already exists\n");
			already_exists = 1;
			curr_file = curr_file->next;
			free(new_dir);
			
			return;
		}
	}

	if(already_exists == 0) {
		// verificam daca exista deja un director care are numele dorit
		if(parent->head_children_dirs) {
			curr_dir = parent->head_children_dirs;

			while(curr_dir && curr_dir->next) {
				if(strcmp(curr_dir->name, name) == 0) {
					printf("Directory already exists\n");
					curr_dir = curr_dir->next;
					free(new_dir);
					return;
				}

				curr_dir = curr_dir->next;
			}

			// verificam si ultimul director din lista
			if(strcmp(curr_dir->name, name) == 0) {
				printf("Directory already exists\n");
				free(new_dir);
				return;
				
			} else {
				// daca se ajunge pe acest else, inseamna ca nu exista
				// coliziuni de nume, deci putem adauga directorul la finalul
				// listei de directori
				curr_dir->next = new_dir;
				new_dir->name = strdup(name);
			}

		} else {
			// daca se ajunge pe acest else, inseamna ca directorul adaugat
			// este primul director din parinte, deci implicit head-ul listei
			parent->head_children_dirs = new_dir;
			new_dir->name = strdup(name);
		}
	}
}

// functia de afisare a directoarelor si fisierelor dintr-ul director
void ls (Dir* parent) {
	// pointer prin care ne "plimbam" prin lista de directoare
	Dir* curr_dir = parent->head_children_dirs;
	
	// printam directoarele
	while(curr_dir && curr_dir->next) {
		printf("%s\n", curr_dir->name);
		curr_dir = curr_dir->next;
	}

	// conditie pentru cazul in care nu ar exista decat 1 sau 0 directoare
	// in parinte, pentru ca nu s-ar mai intra in while in acest caz
	if(curr_dir) {
		printf("%s\n", curr_dir->name);
	}

	// pointer prin care ne "plimbam" prin lista de fisiere
	File* curr_file = parent->head_children_files;

	// printam fisierele
	while(curr_file && curr_file->next) {
		printf("%s\n", curr_file->name);
		curr_file = curr_file->next;
	}

	// conditie pentru cazul in care nu ar exista decat 1 sau 0 fisiere
	// in parinte, pentru ca nu s-ar mai intra in while in acest caz
	if(curr_file) {
		printf("%s\n", curr_file->name);
	}
}

// functie care "rupe legaturile" unui fisier si elibereaza memoria aferenta
void nullify(File* file) {
	file->next = NULL;
	file->parent = NULL;

	free(file->name);
	free(file);
}

// functie de stergere a unui fisier
void rm (Dir* parent, char* name) {
	// pointer prin care ne plimbam prin lista de fisiere pentru a gasi
	// fisierul dorit
	File* curr_file = parent->head_children_files;

	// caz in care fisierul pe care dorim sa-l stergem este head-ul listei
	if (curr_file && !strcmp(curr_file->name, name)) {
		// reactualizam head-ul
		parent->head_children_files = curr_file->next;

		nullify(curr_file);
		return;
	}

	// plimbarea prin lista
	while(curr_file && curr_file->next) {
		// pointer care ne va ajuta la refacerea legaturilor
		File *next_file = curr_file->next;
		
		// daca gasim fisierul pe care dorim sa il stergem, refacem legaturile
		// in lista inainte de a sterge fisierul
		if(!strcmp(next_file->name, name)) {
			curr_file->next = next_file->next;

			nullify(next_file);
			return;	
		}

		curr_file = curr_file->next;
	}

	// cazul in care fisierul pe care dorim sa-l stergem este
	// ultimul din lista
	if(curr_file && !strcmp(curr_file->name, name)) {
		// pointer prin care ne plimbam prin lista cu scopul
		// de a ajunge la penultimul element al listei
		File* aux_file = parent->head_children_files;
		while(aux_file && aux_file->next != curr_file) {
			aux_file = aux_file->next;
		}

		// refacerea legaturilor
		aux_file->next = NULL;
		curr_file->parent = NULL;

		free(curr_file->name);
		free(curr_file);
		return;
	}

	// daca se ajunge aici, inseamna ca fisierul pe care
	// dorim sa-l stergem nu exista
	printf("Could not find the file\n");
}

// functie care sterge toate fisierele dintr-un folder
void remove_files(Dir* folder) {
	if (!folder) {
		return;
	}

	File* curr_file = folder->head_children_files;

	// parcurgem intreaga lista a fisierelor
	while(curr_file) {
		File* next_file = curr_file->next;

		// stergem fisierul si ne mutam pe urmatorul
		free(curr_file->name);
		curr_file->parent = NULL;
		curr_file->next = NULL;
		free(curr_file);

		curr_file = next_file;
	}
}

// functie care sterge si elibereaza memoria unui folder
void set_the_folder_free(Dir* folder) {
	remove_files(folder);
	folder->head_children_dirs = NULL;
	folder->head_children_files = NULL;
	
	free(folder->name);
	free(folder);
}

// functie recursiva care sterge si elibereaza un folder, stergand si
// eliberand si alte foldere care se afla in interiorul lui
void recursive_free(Dir* folder) {
	if(!folder) {
		return;
	}

	// pointer cu care ne vom plimba prin lista de directoare
	Dir* curr_dir = folder->head_children_dirs;
	Dir* next_dir;

	// eliberam recursiv fiecare folder din lista de directoare
	while(curr_dir) {
		next_dir = curr_dir->next;
		recursive_free(curr_dir);
		curr_dir = next_dir;
	}
	
	// la final, stergem si eliberam memoria folderului propriu-zis
	// in aceasta functie fiind sterse si fisierele
	set_the_folder_free(folder);
}

// functie de remove a unui director
void rmdir (Dir* parent, char* name) {
	Dir* curr_dir = parent->head_children_dirs;
	int found = 0;

	// cazul in care dorim sa stergem primul folder din lista de foldere
	if(curr_dir && !strcmp(curr_dir->name, name)) {
		parent->head_children_dirs = curr_dir->next;
		recursive_free(curr_dir);
		return;
	}

	// parcurgem lista de foldere pentru a gasit folderul dorit pentru stergere
	while(curr_dir && curr_dir->next) {
		Dir* next_dir = curr_dir->next;

		// daca il gasim, il eliberam si refacem legaturile listei
		if(strcmp(next_dir->name, name) == 0) {
			curr_dir->next = next_dir->next;
			recursive_free(next_dir);
			return;
		}

		curr_dir = curr_dir->next;
	}
	
	// cazul in care folderul pe care dorim sa-l stergem este la finalul listei
	if(curr_dir && !strcmp(curr_dir->name, name)) {
		// parcurgem lista pana la penultimul element al listei
		Dir* aux_dir = parent->head_children_dirs;
		while(aux_dir && aux_dir != curr_dir) {
			aux_dir = aux_dir->next;
		}

		// refacem legaturile si stergem folderul
		aux_dir->next = NULL;
		recursive_free(curr_dir);
		return;
	}

	// daca se ajunge aici, inseamna ca nu exista directorul
	printf("Could not find the dir\n");
}

// functie de schimbare a directorului in care ne aflam
void cd(Dir** target, char *name) {
	Dir* curr_dir = (*target)->head_children_dirs;
	int found = 0;

	// cazul in care dorim sa ne intoarcem la parintele directorului curent
	// si nu ne aflam deja in home
	if(!strcmp(name, "..") && (*target)->parent) {
		*target = (*target)->parent;
		found = 1;
	} else if(!(*target)->parent && !strcmp(name, "..")) {
		// cazul in care ne aflam in home
		found = 1;
	} else {
		// cazul normal, in care ni se da numele folderului
		// in care vrem sa ne deplasam
		while (curr_dir && curr_dir->next) {
			// daca gasim folderul dorit, ne deplasam pe el
			if(strcmp(curr_dir->name, name) == 0) {
				*target = curr_dir;
				found = 1;
				break;
			}

			curr_dir = curr_dir->next;
		}

		// verificarea ultimului folder din lista si mutarea
		// pe el, in caz ca este folderul dorit
		if(strcmp(curr_dir->name, name) == 0 && !found) {
			*target = curr_dir;
			found = 1;
		}
	}

	// cazul in care folderul nu are alte foldere
	if(!curr_dir && !found) {
		printf("No directories found!\n");
		return;
	}

	// cazul in care nu exista directorul pe care vrem sa ne mutam
	if (!found) {
		printf("No directories found!\n");
	}
}

// functie care returneaza calea relativa a directorului in care ne aflam
char *pwd (Dir* target) {
	char* returned = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
	int nr = 0;

	// vector de string-uri utilizat pentru concatenarea folderelor la cale
	char** files = calloc(50, sizeof(char *));

	for (int i = 0; i < 50; i++) {
		files[i] = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
	}

	Dir* curr_dir = target;

	// popularea vectorului de string-uri cu numele folderelor
	while(curr_dir) {
		strcpy(files[nr], curr_dir->name);
		nr++;

		curr_dir = curr_dir->parent;
	}

	strcat(returned, "/");
	// concatenarea fiecarui folder in ordinea corecta
	for (int i = nr - 1; i >= 0; i--) {
		strcat(returned, files[i]);
		strcat(returned, "/");
	}

	// delimitatorul de string obligatoriu
	returned[strlen(returned) - 1] = '\0';

	// eliberarea vectorului de string-uri
	for(int i = 0; i < 50; i++) {
		free(files[i]);
	}
	free(files);

	return returned;
}

void stop (Dir* target) {
	recursive_free(target);
}

// functia care afiseaza structura arborescenta a unui folder
void tree (Dir* target, int level) {
	if (!target) {
		return;
	}
	
	Dir* curr_dir = target->head_children_dirs;
	File* curr_file = target->head_children_files;

	// parcurgerea folderelor din folderul curent
	while(curr_dir) {
		for(int i = 0; i < level; i++) {
			printf("    ");
		}

		// afisam folderul copil si apoi il parcurgem
		// la randul sau
		printf("%s\n", curr_dir->name);
		tree(curr_dir, level + 1);

		curr_dir = curr_dir->next;
	}

	// parcurgerea fisierelor din folderul curent
	while(curr_file) {
		for (int i = 0; i < level; i++) {
			printf("    ");
		}

		// afisam fisierele
		printf("%s\n", curr_file->name);
		curr_file = curr_file->next;
	}
}

// functia care afla daca un obiect este fisier sau director
char is_it_a_dir_or_a_file(Dir* parent, char* name) {
	Dir* curr_dir = parent->head_children_dirs;
	File* curr_file = parent->head_children_files;

	while(curr_dir) {
		if(!strcmp(curr_dir->name, name)) {
			return 'd';
		}

		curr_dir = curr_dir->next;
	}

	while(curr_file) {
		if(!strcmp(curr_file->name, name)) {
			return 'f';
		}

		curr_file = curr_file->next;
	}

	return '0';
}

// functie care modifica numele unui director/fisier
void mv(Dir* parent, char *oldname, char *newname) {
	Dir* curr_dir = parent->head_children_dirs;
	File* curr_file = parent->head_children_files;
	Dir* last_dir = parent->head_children_dirs;
	File* last_file = parent->head_children_files;

	char is_it = is_it_a_dir_or_a_file(parent, oldname);
	char already_exists = is_it_a_dir_or_a_file(parent, newname);

	// daca exista deja fisier/director cu numele dorit
	if(already_exists == 'd' || already_exists == 'f') {
		printf("File/Director already exists\n");
		return;
	}

	// daca dorim sa modificam numele unui director
	if(is_it == 'd') {
		Dir* aux_dir = curr_dir;

		// parcurgem lista de directoare pana gasim directorul
		// pe care vrem sa il modificam
		while(strcmp(curr_dir->name, oldname)) {
			curr_dir = curr_dir->next;
		}

		// daca folderul pe care dorim sa il modificam e primul din lista
		if(curr_dir == parent->head_children_dirs) {
			// daca nu e singurul folder din lista
			if (curr_dir->next) {
				parent->head_children_dirs = curr_dir->next;
			} else {
				// daca este singurul folder din lista, 
				// ii modificam doar numele
				strcpy(curr_dir->name, newname);
				return;
			}
		} else {
			// parcurgem lista pana cand ajungem la folderul de dinaintea
			// folderului pe care dorim sa-l modificam
			while(aux_dir->next != curr_dir) {
				aux_dir = aux_dir->next;
			}

			// refacem legaturile
			aux_dir->next = curr_dir->next;
		}

		// pointer pe care il facem sa pointeze la ultimul director din lista
		while(last_dir && last_dir->next) {
			last_dir = last_dir->next;
		}

		// directorul modificat este adaugat la finalul listei
		last_dir->next = curr_dir;
		curr_dir->next = NULL;
		strcpy(curr_dir->name, newname);

	} else if(is_it == 'f') {
		// daca dorim sa modificam numele unui fisier
		File* aux_file = curr_file;

		// parcurgem lista de fisiere pana gasim fisierul
		// pe care vrem sa il modificam
		while(strcmp(curr_file->name, oldname)) {
			curr_file = curr_file->next;
		}

		// daca fisierul pe care dorim sa il modificam e primul din lista
		if(curr_file == parent->head_children_files) {
			// daca nu e singurul fisier din lista
			if(curr_file->next) {
				parent->head_children_files = curr_file->next;
			} else {
				// daca este singurul fisier din lista, 
				// ii modificam doar numele
				strcpy(curr_file->name, newname);
				return;
			}
		} else {
			// parcurgem lista pana cand ajungem la fisierul de dinaintea
			// fisierului pe care dorim sa-l modificam
			while(aux_file->next != curr_file) {
				aux_file = aux_file->next;
			}

			// refacem legaturile
			aux_file->next = curr_file->next;
		}

		// pointer pe care il facem sa pointeze la ultimul fisier din lista
		while(last_file && last_file->next) {
			last_file = last_file->next;
		}

		// fisierul modificat este adaugat la finalul listei
		last_file->next = curr_file;
		curr_file->next = NULL;
		strcpy(curr_file->name, newname);

	} else {
		printf("File/Director not found\n");
	}
}

// functie care initializeaza directorul home
Dir* init_home() {
	Dir* home = malloc(sizeof(*home));
	home->name = malloc(5 * sizeof(char));
	strcpy(home->name, "home");

	home->parent = NULL;
	home->head_children_dirs = NULL;
	home->head_children_files = NULL;
	home->next = NULL;
}

int main () {
	char *command = malloc (MAX_INPUT_LINE_SIZE * sizeof(char));
	char *file = malloc(MAX_INPUT_LINE_SIZE * sizeof(char));
	char *newname = malloc(MAX_INPUT_LINE_SIZE * sizeof(char));
	char *a = malloc(MAX_INPUT_LINE_SIZE * sizeof(char));

	Dir* home = init_home();
	Dir* curr_dir = home;

	fgets(a, MAX_INPUT_LINE_SIZE, stdin);

	while (1) {
		sscanf(a, "%s %s %s", command, file, newname);

		if(strcmp(command, "touch") == 0) {
			touch(curr_dir, file);
		}

		if(strcmp(command, "mkdir") == 0) {
			mkdir(curr_dir, file);
		}

		if(strcmp(command, "ls") == 0) {
			ls(curr_dir);
		}

		if(strcmp(command, "rm") == 0) {
			rm(curr_dir, file);
		}

		if(strcmp(command, "rmdir") == 0) {
			rmdir(curr_dir, file);
		}

		if(strcmp(command, "cd") == 0) {
			cd(&curr_dir, file);
		}

		if(strcmp(command, "tree") == 0) {
			tree(curr_dir, 0);
		}

		if(strcmp(command, "pwd") == 0) {
			char* returned = pwd(curr_dir);
			printf("%s\n", returned);
			free(returned);
		}

		if(strcmp(command, "mv") == 0) {
			mv(curr_dir, file, newname);
		}

		if(strcmp(command, "stop") == 0) {
			break;
		}

		fgets(a, MAX_INPUT_LINE_SIZE, stdin);
	}

	free(a);
	free(newname);
	free(file);
	free(command);
	stop(home);
	
	return 0;
}
