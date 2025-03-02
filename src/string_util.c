#include "string_util.h"
#include <string.h>

/*
** Remove beginning space
** REMEMBER to free returned string
*/
char* trim_left(char *s, int n) {
    // count the number of space at the beginning
    int i;
    for (i = 0; i < n; i++) {
        if (s[i] != ' ') {
            break;
        }
    }

    int new_len = n - i;
    char* new_str = malloc((new_len + 1) * sizeof(char));
    if (new_str == NULL) {
        // failed to allocated new memory
        return s;
    }
    strncpy(new_str, s+i, new_len);

    new_str[new_len] = '\0';
    return new_str;
}

/*
** Remove ending space
** REMEMBER to free returned string
*/
char* trim_right(char *s, int n) {
    // count the number of space at the beginning
    int i;
    for (i = n-1; i >= 0; i--) {
        if (s[i] != ' ') {
            break;
        }
    }

    int new_len = i+1;
    char* new_str = malloc((new_len + 1) * sizeof(char));
    if (new_str == NULL) {
        // failed to allocated new memory
        return s;
    }
    strncpy(new_str, s, new_len);
    new_str[new_len] = '\0';
    return new_str;
}


void cut_off_first_n_character(char* s, int n) {
#ifdef DEBUG
    printf("cutoff first %d character from %s", n , s);
#endif
    if (n >= strlen(s)) {
        s[0] = '\0';
        return;
    }

    memmove(s, s + n, strlen(s) - n + 1);
};
