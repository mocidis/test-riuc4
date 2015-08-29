#include "riuc4_uart.h"
#include "ansi-utils.h"
void on_riuc4_status(int port, riuc4_signal_t signal, uart4_status_t *ustatus) {
    fprintf(stdout, "RIUC4 port %d, update signal %s. Status: tx=%d,rx=%d,ptt=%d,sq=%d\n", port, RIUC4_SIGNAL_NAME[signal], ustatus->tx, ustatus->rx, ustatus->ptt, ustatus->sq);
}
int main(int n, char *opts[]) {
    char temp[10];
    int fEnd = 0;
    int port_idx;

    serial_t serial;
    riuc4_t riuc4;

    if(n < 2) {
        fprintf(stdout, "Usage: myapp-riuc4 <device>\n");
        exit(-1);
    }
    fprintf(stdout, "RIUC - 4COM - RIUC-ccp-v0.2\n");
    riuc4_init(&serial, &riuc4, &on_riuc4_status);

    riuc4_start(&serial, opts[1]);

    while(!fEnd) {
        fprintf(stdout, "COMMAND:<port[1-4]><command[DdEertsl+-q]>:\n");
        fgets(temp, sizeof(temp), stdin);
        port_idx = temp[0] - '1';
        if( port_idx < 0 || port_idx > 3 ) {
            fprintf(stdout, "Command error\n");
            continue;
        }
        switch (temp[1]) {
            case 'D':
                    riuc4_disable_rx(&riuc4, port_idx);
                    break;
            case 'd':
                    riuc4_disable_tx(&riuc4, port_idx);
                    break;
            case 'E':
                    riuc4_enable_rx(&riuc4, port_idx);
                    break;
            case 'e':
                    riuc4_enable_tx(&riuc4, port_idx);
                    break;
            case 'r':
                    riuc4_probe_rx(&riuc4, port_idx);
                    break;
            case 't':
                    riuc4_probe_tx(&riuc4, port_idx);
                    break;
            case 's':
                    riuc4_probe_sq(&riuc4, port_idx);
                    break;
            case 'l':
                    riuc4_probe_ptt(&riuc4, port_idx);
                    break;
            case '+':
                    riuc4_on_ptt(&riuc4, port_idx);
                    break;
            case '-':
                    riuc4_off_ptt(&riuc4, port_idx);
                    break;
            case 'q':
                    fEnd = 1;
                    break;
            default:
                    fprintf(stdout, "Unknown command");
                    break;
        }
    }

    riuc4_end(&serial);

    return 0;
}
