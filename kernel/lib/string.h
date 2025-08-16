int strlen(const char* text) {
    return sizeof(text) / sizeof(char);
}

int strequ(const char* t1, const char* t2) {
    if (strlen(t1) != strlen(t2)) {return 0;}
    for (int i = 0; i != strlen(t1); i++) {
        if (t1[i] != t2[i]) {return 0;}
    }
    return 1;
}
