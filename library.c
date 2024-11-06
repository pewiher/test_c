// import of library 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#define MAX_LINE_LENGTH 256

void *current_library = NULL;

// Function to load a shared library
int load_library(const char *lib_path) {
    if (current_library) {
        dlclose(current_library);
        current_library = NULL;
    }

    current_library = dlopen(lib_path, RTLD_LAZY);
    if (!current_library) {
        fprintf(stderr, "Error: Unable to load the library.... '%s'. %s\n", lib_path, dlerror());
        return -1;
    }
    printf("Library '%s' loaded success.\n", lib_path);
    return 0;
}

// Función para llamar una función de la biblioteca cargada
int call_function(const char *func_name) {
    if (!current_library) {
        fprintf(stderr, "Error: No library is loaded.\n");
        return -1;
    }

    void (*func)(void) = dlsym(current_library, func_name);
    char *error;
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "Error: the function '%s' not found in the library.\n", func_name);
        return -1;
    }

    func();
    printf("function '%s' execute success.\n", func_name);
    return 0;
}

// Función para procesar comandos
void process_command(char *line) {
    char command[MAX_LINE_LENGTH];
    char argument[MAX_LINE_LENGTH];

    if (sscanf(line, "%s %s", command, argument) != 2) {
        fprintf(stderr, "Error: invalid command: %s\n", line);
        return;
    }

    if (strcmp(command, "use") == 0) {
        load_library(argument);
    } else if (strcmp(command, "call") == 0) {
        call_function(argument);
    } else {
        fprintf(stderr, "Error: Unknown command '%s'.\n", command);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <ruta_al_script>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *script_file = fopen(argv[1], "r");
    if (!script_file) {
        perror("Error when opening the script file");
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), script_file)) {
        if (line[0] == '#' || line[0] == ';') continue; // Ignorar comentarios
        line[strcspn(line, "\n")] = 0; // Remover salto de línea
        process_command(line);
    }

    if (current_library) {
        dlclose(current_library);
    }

    fclose(script_file);
    return EXIT_SUCCESS;
}
