#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void disableRawMode() {
    // Reset to canonical mode atexit
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    struct termios raw = orig_termios;

    // ICANON: turns off canonical mode, read input byte 
    // by byte instead of line-by-line
    // ISIG diables ^C and ^X
    // IXON diables ^S and ^Q
    raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP | ICRNL | IXON);
    // turn off all output processing features
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

}

int main() {
    enableRawMode();

    
    // Read byte-by-byte from stdin
    while (1) {
	char c = '\0';
	read(STDIN_FILENO, &c, 1);
	if (iscntrl(c)) {
	    printf("%d\r\n",c);
	} else {
	    printf("%d ('%c')\r\n",c,c);
	}
	if (c == 'q') break;
    }
    return EXIT_SUCCESS;
}
