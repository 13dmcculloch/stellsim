/* Generic macros for stellsim */

/* Compile time array length */
#define LOOKUP_LEN(a) ((sizeof(a)/sizeof(*a)))

/* Check if ptr is NULL */
#define MEM_CHECK(p) if(p == NULL) { printf("Nullptr at %p\n", (void *)&p); return 1; }

/* (UNIX) Cursor terminal commands */
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))
#define clear() printf("\033[H\033[J") 
