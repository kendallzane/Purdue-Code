/*
 * CS354: Operating Systems.
 * Purdue University
 * Example that shows how to read one line with simple editing
 * using raw terminal.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define MAX_BUFFER_LINE 2048

// Buffer where line is stored
int line_length;
//int line_position;
char line_buffer[MAX_BUFFER_LINE];

// Simple history array
// This history does not change.
// Yours have to be updated.
int history_index = 0;
int max_history = 3;
int max_history_size = 1000;
char history[3][1000];
int history_length = 0;


void read_line_print_usage()
{
  char * usage = "\n"
    " ctrl-?       Print usage\n"
    " Backspace    Deletes last character\n"
    " up arrow     See last command in the history\n";

  write(1, usage, strlen(usage));
}

/*
 * Input a line with some basic editing.
 */
char * read_line() {

  // Set terminal in raw mode
  tty_raw_mode();

  line_length = 0;
  //line_position = 0;

  // Read one line until enter is typed
  while (1) {

    // Read one character in raw mode.
    char ch;
    read(0, &ch, 1);

    if (ch>=32) {
      // It is a printable character.

      // Do echo
      write(1,&ch,1);

      // If max number of character reached return.
      if (line_length==MAX_BUFFER_LINE-2) break;

      // add char to buffer.
      line_buffer[line_length]=ch;
      line_length++;
    }
    else if (ch==10) {
      // <Enter> was typed. Return line
      // Print newline
	  int i = 0;
	  for (i; i < max_history-1; i++) {
		  if (i+1 < history_length) {
			strncpy(history[i+1], history[i], max_history_size);
		  }
	  }
	  strncpy(history[0], line_buffer, line_length);
	  if (history_length < max_history) {
		  history_length++;
		  printf("history_length : %i\n", history_length);
	  }
      write(1,&ch,1);

      break;
    }
    else if (ch == 31) {
      // ctrl-?
      read_line_print_usage();
      line_buffer[0]=0;
      break;
    }
    else if (ch == 8) { 
		/*
      // <backspace> was typed. Remove previous character read.

      // Go back one character
      ch = 8;
      write(1,&ch,1);

      // Write a space to erase the last character read
      ch = ' ';
      write(1,&ch,1);

      // Go back one character
      ch = 8;
      write(1,&ch,1);

      // Remove one character from buffer
      line_length--;
	  */
    }
    else if (ch==27) {
      // Escape sequence. Read two chars more
      //
      // HINT: Use the program "keyboard-example" to
      // see the ascii code for the different chars typed.
      //
      char ch1; 
      char ch2;
      read(0, &ch1, 1);
      read(0, &ch2, 1);
      if (ch1==91 && ch2==65) {
			// Up arrow. Print next line in history.
			printf("\nup\n");
			if (history_length > 0) {
				// Erase old line
				// Print backspaces
				int i = 0;
				for (i =0; i < line_length; i++) {
					ch = 8;
					write(1,&ch,1);
				}

				// Print spaces on top
				for (i =0; i < line_length; i++) {
					ch = ' ';
					write(1,&ch,1);
				}

				// Print backspaces
				for (i =0; i < line_length; i++) {
					ch = 8;
					write(1,&ch,1);
				}
				

				
				// Copy line from history
				strcpy(line_buffer, history[history_index]);
				line_length = strlen(line_buffer);
				printf("accessing history #%i : %s\n", history_index, line_buffer);

				if (history_index < history_length) {
					history_index++;
				}
				
				// echo line
				write(1, line_buffer, line_length);
			} else {
				printf("\n hs = 0\n");
			}
      } else if (ch1==91 && ch2==65) {
			// Down arrow. Print previous line in history.
			printf("\ndowdn\n");
			if (history_length > 0) {
				// Erase old line
				// Print backspaces
				int i = 0;
				for (i =0; i < line_length; i++) {
					ch = 8;
					write(1,&ch,1);
				}

				// Print spaces on top
				for (i =0; i < line_length; i++) {
					ch = ' ';
					write(1,&ch,1);
				}

				// Print backspaces
				for (i =0; i < line_length; i++) {
					ch = 8;
					write(1,&ch,1);
				}
				

				
				// Copy line from history
				strcpy(line_buffer, history[history_index]);
				line_length = strlen(line_buffer);
				
				printf("accessing history #%i : %s\n", history_index, line_buffer);
				
				if (history_index-1 > 0) {
					history_index--;
				}
				
				// echo line
				write(1, line_buffer, line_length);
			} else {
				printf("\n hs = 0\n");
			}
	  }
    }

  }

  // Add eol and null char at the end of string
  line_buffer[line_length]=10;
  line_length++;
  line_buffer[line_length]=0;

  return line_buffer;
}

