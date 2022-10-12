#include "npshell.h"

int main() {
    struct NPShell * shell = npshell_new();
    
    while(npshell_getline(shell) > 0) {
        npshell_exec(shell);
    }

    npshell_del(shell);
}
