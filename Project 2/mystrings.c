#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
        FILE *f;
        char* strings;
        int file_size;
        int current = 0;
        int string_beginning = 0;

        f = fopen(argv[1], "rb");

        fseek(f, 0, SEEK_END);
        file_size = ftell(f);
        fseek(f, 0, SEEK_SET);

        strings = malloc(file_size);
        fread(strings, 1, file_size, f);

        while(1){
                while (((strings[current] > 32) && (strings[current] < 126)) || (strings[current] == 9)){
                        if (current == file_size - 1){
                                break;
                        }
                        current ++;
                }
                if (current - string_beginning >= 4){
                        printf("%.*s\n", current - string_beginning, &strings[string_beginning]);
                }

                current++;
                string_beginning = current;
                if (current == file_size){
                        break;
                }
        }
        free(strings);
        fclose(f);

        return 0;
}

