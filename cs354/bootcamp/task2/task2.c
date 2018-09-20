#include <stdio.h>
#include <math.h>
#include "calc_coord.h"

int main()
{
	double c;
	int failed = 0;
	printf("TESTING: SLOPE FORMULA\n");
	printf("----------------------\n");
	printf("Test 1: Slope from (2,1) to (4,7) should be 3\n");
	c = slope(2,1,4,7);
	printf("Calculated Slope: %f\n", c);
	if (c == (double)3) {
		printf("Test Passed...\n");
	} else {
		printf("Test Failed!\n");
		failed = 1;
	}

	printf("Test 2: Slope from (2,5) to (6,8) should be 0.75\n");
	c = slope(2,5,6,8);
	printf("Calculated Slope: %f\n", c);
	if (c == (double)0.75) {
		printf("Test Passed...\n");
	} else {
		printf("Test Failed!\n");
		failed = 1;
	}
	

	printf("Test 3: Slope from (2,-3) to (-4,3) should be -1\n");
	c = slope(2,-3,-4,3);
	printf("Calculated Slope: %f\n", c);
	if (c == (double)-1) {
		printf("Test Passed...\n");
	} else {
		printf("Test Failed!\n");
		failed = 1;
	}
	
	if (!failed) {
		printf("ALL SLOPE TESTS PASSED!\n");
	} else {
		printf("SLOPE TEST FORMULA DOES NOT WORK AS INTENDED!\n");
	}
	printf("\n");
	
	failed = 0;
	printf("TESTING: DISTANCE FORMULA\n");
	printf("----------------------\n");
	printf("Test 1: Distance from point (10,10) to line 0=3x+4y+5 should be 15\n");
	c = distance(3,4,5,10,10);
	printf("Calculated Slope: %f\n", c);
	if (c == (double)15) {
		printf("Test Passed...\n");
	} else {
		printf("Test Failed!\n");
		failed = 1;
	}

	printf("Test 2: Distance from point (6,2) to line 0=5x+12y-2 should be 4\n");
	c = distance(5,12,-2,6,2);
	printf("Calculated Slope: %f\n", c);
	if (c == (double)4) {
		printf("Test Passed...\n");
	} else {
		printf("Test Failed!\n");
		failed = 1;
	}
	

	printf("Test 3: Distance from point (4,-1) to line 0=7x+14 should be 2\n");
	c = distance(7,0,14,-4,-1);
	printf("Calculated Slope: %f\n", c);
	if (c == (double)2) {
		printf("Test Passed...\n");
	} else {
		printf("Test Failed!\n");
		failed = 1;
	}
	
	if (!failed) {
		printf("ALL DISTANCE TESTS PASSED!\n");
	} else {
		printf("DISTANCE TEST FORMULA DOES NOT WORK AS INTENDED!\n");
	}
	printf("\n");

}
