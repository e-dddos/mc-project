#include "touch.h"

void main(void)
{
    system_init();
	while (1)
    {
        run_tacho();
        check_button();
    }
}
