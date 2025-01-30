#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void loop(void){
    char *input;
    char *prompt = "gsh> ";
    size_t bufsize = 0;

    while(1){
        printf("%s", prompt);
        if (getline(&input, &bufsize, stdin) == -1) {
            printf("\n");
            break;
        }
        
        printf("received: %s", input);
        if(!strcmp(input, "exit\n")){
            break;
        }

        free(input);
        input = NULL;
    }
}

int main(int argc, char **argv){
    signal(SIGINT, SIG_IGN);

    if(argc > 1){
        fprintf(stderr, "gsh can't execute scripts (yet)\n");
        exit(EXIT_FAILURE);
    } else {
        loop();
    }

    return 0;
}