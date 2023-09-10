/* Console
 *
 * Douglas McCulloch, September 2023
 */

#include <string.h>

#include "console.h"

void enter_console_msg();

int console(Symbol *lookup, size_t lookup_len)
{
    enter_console_msg();

    char input_buffer[100];
    /* Event loop */
    while(strcmp(input_buffer, EXIT_COMMAND))
    {
        printf(">>> ");
        scanf("%s", &input_buffer);
        // handle_input(input_buffer);
    }

    return 0;
}

void enter_console_msg()
{
    printf("\nStellsim alpha console.\nDouglas McCulloch, September 2023.\n");
}
