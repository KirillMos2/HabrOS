const char* version = "HabrOS 0.0.2 BUILD #14";

typedef unsigned char ui8;
typedef unsigned short ui16;
typedef unsigned int ui32;
typedef unsigned long ui64;

typedef struct heap_cell {
    heap_cell* next;
    void* data;
};
