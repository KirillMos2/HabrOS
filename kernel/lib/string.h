int strlen(const char* text) {
    int l = 0;
    while (text[l] != '\0') l++;
    return l;
}

int strequ(const char* t1, const char* t2) {
    if (strlen(t1) != strlen(t2)) {return 0;}
    for (int i = 0; i != strlen(t1); i++) {
        if (t1[i] != t2[i]) {return 0;}
        if (t1[i] == '\0') {return 1;}
    }
    return 1;
}

int strstart(const char* text, const char* subtext) {
    if (strlen(subtext) == strlen(text)) {return strequ(text, subtext);}
    else if (strlen(subtext) > strlen(text)) {return 0;}
    else {
        for (int i = 0; i != strlen(subtext); i++) {
            if (text[i] != subtext[i]) {return 0;}
            if (subtext[i] == '\0') {return 1;}
        }
    }
}
