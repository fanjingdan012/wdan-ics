/**************************************************************************
 Polynomial evaluation functions.  Edit this file as follows:
 1. Put your name and Andrew ID in the comment below
 2. Implement different versions of the polynomial evaluation function
 3. Edit the table at the end to include your implementations.  Put
 your best solutions (minimum CPE, minimum C10) as the first
 two entries in this table.
 ***************************************************************************/

#include  <stdio.h>
#include  <stdlib.h>
#include  "poly.h"

/**************************************************************************
 My name is Wu Yanhong.
 My student number is 09302010035.
 ***************************************************************************/

/* Polynomial evaluation.  Reference implementation */
/* Original method. */
int poly_eval(int *a, int degree, int x) {
	int result = 0;
	int i;
	int xpwr = 1; /* Successive powers of x */
	for (i = 0; i <= degree; i++) {
		result += a[i] * xpwr;
		xpwr *= x;
	}
	return result;
}
/* Use the Horner's Rule.*/
int poly_evalg(int *a, int degree, int x) {
	int result = a[degree];
	int i;
	for (i = degree - 1; i >= 0; i--) {
		result = a[i] + result * x;
	}
	return result;
}
/* Loop Unrolling*2 */
int poly_eval2(int *a, int degree, int x) {
	int result = a[degree];
	int i;
	int pow1 = x;
	int pow2 = pow1 * x;
	for (i = degree - 1; i >= 1; i -= 2) {
		result *= pow2;
		result += a[i - 0] * pow1;
		result += a[i - 1];
	}
	for (; i >= 0; i--) {
		result *= x;
		result += a[i];
	}
	return result;
}
/* Loop Unrolling*4 */
int poly_eval4(int *a, int degree, int x) {
	int result = a[degree];
	int i;
	int pow1 = x;
	int pow2 = pow1 * x;
	int pow3 = pow2 * x;
	int pow4 = pow3 * x;
	for (i = degree - 1; i >= 3; i -= 4) {
		result *= pow4;
		result += a[i - 0] * pow3;
		result += a[i - 1] * pow2;
		result += a[i - 2] * pow1;
		result += a[i - 3];
	}
	for (; i >= 0; i--) {
		result *= x;
		result += a[i];
	}
	return result;
}

/* Loop Unrolling*3 */
int poly_eval3(int *a, int degree, int x) {
	int result = a[degree];
	int i;
	int pow2 = x * x;
	for (i = degree - 1; i >= 1; i -= 2) {
		result *= pow2;
		result += a[i] * x;
		result += a[i - 1];
	}
	for (; i >= 0; i--) {
		result *= x;
		result += a[i];
	}
	return result;
}
/* Loop Unrolling*96 */
int poly_eval96(int *a, int degree, int x) {
	int result = a[degree];
	int i;
	//At first,Compute x^n
	int pow1 = x;
	int pow2 = pow1 * x;
	int pow3 = pow2 * x;
	int pow4 = pow3 * x;
	int pow5 = pow4 * x;
	int pow6 = pow5 * x;
	int pow7 = pow6 * x;
	int pow8 = pow7 * x;
	int pow9 = pow8 * x;
	int pow10 = pow9 * x;
	int pow11 = pow10 * x;
	int pow12 = pow11 * x;
	int pow13 = pow12 * x;
	int pow14 = pow13 * x;
	int pow15 = pow14 * x;
	int pow16 = pow15 * x;
	int pow17 = pow16 * x;
	int pow18 = pow17 * x;
	int pow19 = pow18 * x;
	int pow20 = pow19 * x;
	int pow21 = pow20 * x;
	int pow22 = pow21 * x;
	int pow23 = pow22 * x;
	int pow24 = pow23 * x;
	int pow25 = pow24 * x;
	int pow26 = pow25 * x;
	int pow27 = pow26 * x;
	int pow28 = pow27 * x;
	int pow29 = pow28 * x;
	int pow30 = pow29 * x;
	int pow31 = pow30 * x;
	int pow32 = pow31 * x;
	int pow33 = pow32 * x;
	int pow34 = pow33 * x;
	int pow35 = pow34 * x;
	int pow36 = pow35 * x;
	int pow37 = pow36 * x;
	int pow38 = pow37 * x;
	int pow39 = pow38 * x;
	int pow40 = pow39 * x;
	int pow41 = pow40 * x;
	int pow42 = pow41 * x;
	int pow43 = pow42 * x;
	int pow44 = pow43 * x;
	int pow45 = pow44 * x;
	int pow46 = pow45 * x;
	int pow47 = pow46 * x;
	int pow48 = pow47 * x;
	int pow49 = pow48 * x;
	int pow50 = pow49 * x;
	int pow51 = pow50 * x;
	int pow52 = pow51 * x;
	int pow53 = pow52 * x;
	int pow54 = pow53 * x;
	int pow55 = pow54 * x;
	int pow56 = pow55 * x;
	int pow57 = pow56 * x;
	int pow58 = pow57 * x;
	int pow59 = pow58 * x;
	int pow60 = pow59 * x;
	int pow61 = pow60 * x;
	int pow62 = pow61 * x;
	int pow63 = pow62 * x;
	int pow64 = pow63 * x;
	int pow65 = pow64 * x;
	int pow66 = pow65 * x;
	int pow67 = pow66 * x;
	int pow68 = pow67 * x;
	int pow69 = pow68 * x;
	int pow70 = pow69 * x;
	int pow71 = pow70 * x;
	int pow72 = pow71 * x;
	int pow73 = pow72 * x;
	int pow74 = pow73 * x;
	int pow75 = pow74 * x;
	int pow76 = pow75 * x;
	int pow77 = pow76 * x;
	int pow78 = pow77 * x;
	int pow79 = pow78 * x;
	int pow80 = pow79 * x;
	int pow81 = pow80 * x;
	int pow82 = pow81 * x;
	int pow83 = pow82 * x;
	int pow84 = pow83 * x;
	int pow85 = pow84 * x;
	int pow86 = pow85 * x;
	int pow87 = pow86 * x;
	int pow88 = pow87 * x;
	int pow89 = pow88 * x;
	int pow90 = pow89 * x;
	int pow91 = pow90 * x;
	int pow92 = pow91 * x;
	int pow93 = pow92 * x;
	int pow94 = pow93 * x;
	int pow95 = pow94 * x;
	int pow96 = pow95 * x;
	//Loop Unrolling 96 times.
	for (i = degree - 1; i >= 95; i -= 96) {
		result *= pow96;
		result += a[i - 0] * pow95;
		result += a[i - 1] * pow94;
		result += a[i - 2] * pow93;
		result += a[i - 3] * pow92;
		result += a[i - 4] * pow91;
		result += a[i - 5] * pow90;
		result += a[i - 6] * pow89;
		result += a[i - 7] * pow88;
		result += a[i - 8] * pow87;
		result += a[i - 9] * pow86;
		result += a[i - 10] * pow85;
		result += a[i - 11] * pow84;
		result += a[i - 12] * pow83;
		result += a[i - 13] * pow82;
		result += a[i - 14] * pow81;
		result += a[i - 15] * pow80;
		result += a[i - 16] * pow79;
		result += a[i - 17] * pow78;
		result += a[i - 18] * pow77;
		result += a[i - 19] * pow76;
		result += a[i - 20] * pow75;
		result += a[i - 21] * pow74;
		result += a[i - 22] * pow73;
		result += a[i - 23] * pow72;
		result += a[i - 24] * pow71;
		result += a[i - 25] * pow70;
		result += a[i - 26] * pow69;
		result += a[i - 27] * pow68;
		result += a[i - 28] * pow67;
		result += a[i - 29] * pow66;
		result += a[i - 30] * pow65;
		result += a[i - 31] * pow64;
		result += a[i - 32] * pow63;
		result += a[i - 33] * pow62;
		result += a[i - 34] * pow61;
		result += a[i - 35] * pow60;
		result += a[i - 36] * pow59;
		result += a[i - 37] * pow58;
		result += a[i - 38] * pow57;
		result += a[i - 39] * pow56;
		result += a[i - 40] * pow55;
		result += a[i - 41] * pow54;
		result += a[i - 42] * pow53;
		result += a[i - 43] * pow52;
		result += a[i - 44] * pow51;
		result += a[i - 45] * pow50;
		result += a[i - 46] * pow49;
		result += a[i - 47] * pow48;
		result += a[i - 48] * pow47;
		result += a[i - 49] * pow46;
		result += a[i - 50] * pow45;
		result += a[i - 51] * pow44;
		result += a[i - 52] * pow43;
		result += a[i - 53] * pow42;
		result += a[i - 54] * pow41;
		result += a[i - 55] * pow40;
		result += a[i - 56] * pow39;
		result += a[i - 57] * pow38;
		result += a[i - 58] * pow37;
		result += a[i - 59] * pow36;
		result += a[i - 60] * pow35;
		result += a[i - 61] * pow34;
		result += a[i - 62] * pow33;
		result += a[i - 63] * pow32;
		result += a[i - 64] * pow31;
		result += a[i - 65] * pow30;
		result += a[i - 66] * pow29;
		result += a[i - 67] * pow28;
		result += a[i - 68] * pow27;
		result += a[i - 69] * pow26;
		result += a[i - 70] * pow25;
		result += a[i - 71] * pow24;
		result += a[i - 72] * pow23;
		result += a[i - 73] * pow22;
		result += a[i - 74] * pow21;
		result += a[i - 75] * pow20;
		result += a[i - 76] * pow19;
		result += a[i - 77] * pow18;
		result += a[i - 78] * pow17;
		result += a[i - 79] * pow16;
		result += a[i - 80] * pow15;
		result += a[i - 81] * pow14;
		result += a[i - 82] * pow13;
		result += a[i - 83] * pow12;
		result += a[i - 84] * pow11;
		result += a[i - 85] * pow10;
		result += a[i - 86] * pow9;
		result += a[i - 87] * pow8;
		result += a[i - 88] * pow7;
		result += a[i - 89] * pow6;
		result += a[i - 90] * pow5;
		result += a[i - 91] * pow4;
		result += a[i - 92] * pow3;
		result += a[i - 93] * pow2;
		result += a[i - 94] * pow1;
		result += a[i - 95];
	}
	for (; i >= 0; i--) {
		result *= x;
		result += a[i];
	}
	return result;
}
/* Loop Unrolling*128 Just as an reference. */
int poly_eval128(int *a, int degree, int x) {
	int result = a[degree];
	int i;
	int pow1 = x;
	int pow2 = pow1 * x;
	int pow3 = pow2 * x;
	int pow4 = pow3 * x;
	int pow5 = pow4 * x;
	int pow6 = pow5 * x;
	int pow7 = pow6 * x;
	int pow8 = pow7 * x;
	int pow9 = pow8 * x;
	int pow10 = pow9 * x;
	int pow11 = pow10 * x;
	int pow12 = pow11 * x;
	int pow13 = pow12 * x;
	int pow14 = pow13 * x;
	int pow15 = pow14 * x;
	int pow16 = pow15 * x;
	int pow17 = pow16 * x;
	int pow18 = pow17 * x;
	int pow19 = pow18 * x;
	int pow20 = pow19 * x;
	int pow21 = pow20 * x;
	int pow22 = pow21 * x;
	int pow23 = pow22 * x;
	int pow24 = pow23 * x;
	int pow25 = pow24 * x;
	int pow26 = pow25 * x;
	int pow27 = pow26 * x;
	int pow28 = pow27 * x;
	int pow29 = pow28 * x;
	int pow30 = pow29 * x;
	int pow31 = pow30 * x;
	int pow32 = pow31 * x;
	int pow33 = pow32 * x;
	int pow34 = pow33 * x;
	int pow35 = pow34 * x;
	int pow36 = pow35 * x;
	int pow37 = pow36 * x;
	int pow38 = pow37 * x;
	int pow39 = pow38 * x;
	int pow40 = pow39 * x;
	int pow41 = pow40 * x;
	int pow42 = pow41 * x;
	int pow43 = pow42 * x;
	int pow44 = pow43 * x;
	int pow45 = pow44 * x;
	int pow46 = pow45 * x;
	int pow47 = pow46 * x;
	int pow48 = pow47 * x;
	int pow49 = pow48 * x;
	int pow50 = pow49 * x;
	int pow51 = pow50 * x;
	int pow52 = pow51 * x;
	int pow53 = pow52 * x;
	int pow54 = pow53 * x;
	int pow55 = pow54 * x;
	int pow56 = pow55 * x;
	int pow57 = pow56 * x;
	int pow58 = pow57 * x;
	int pow59 = pow58 * x;
	int pow60 = pow59 * x;
	int pow61 = pow60 * x;
	int pow62 = pow61 * x;
	int pow63 = pow62 * x;
	int pow64 = pow63 * x;
	int pow65 = pow64 * x;
	int pow66 = pow65 * x;
	int pow67 = pow66 * x;
	int pow68 = pow67 * x;
	int pow69 = pow68 * x;
	int pow70 = pow69 * x;
	int pow71 = pow70 * x;
	int pow72 = pow71 * x;
	int pow73 = pow72 * x;
	int pow74 = pow73 * x;
	int pow75 = pow74 * x;
	int pow76 = pow75 * x;
	int pow77 = pow76 * x;
	int pow78 = pow77 * x;
	int pow79 = pow78 * x;
	int pow80 = pow79 * x;
	int pow81 = pow80 * x;
	int pow82 = pow81 * x;
	int pow83 = pow82 * x;
	int pow84 = pow83 * x;
	int pow85 = pow84 * x;
	int pow86 = pow85 * x;
	int pow87 = pow86 * x;
	int pow88 = pow87 * x;
	int pow89 = pow88 * x;
	int pow90 = pow89 * x;
	int pow91 = pow90 * x;
	int pow92 = pow91 * x;
	int pow93 = pow92 * x;
	int pow94 = pow93 * x;
	int pow95 = pow94 * x;
	int pow96 = pow95 * x;
	int pow97 = pow96 * x;
	int pow98 = pow97 * x;
	int pow99 = pow98 * x;
	int pow100 = pow99 * x;
	int pow101 = pow100 * x;
	int pow102 = pow101 * x;
	int pow103 = pow102 * x;
	int pow104 = pow103 * x;
	int pow105 = pow104 * x;
	int pow106 = pow105 * x;
	int pow107 = pow106 * x;
	int pow108 = pow107 * x;
	int pow109 = pow108 * x;
	int pow110 = pow109 * x;
	int pow111 = pow110 * x;
	int pow112 = pow111 * x;
	int pow113 = pow112 * x;
	int pow114 = pow113 * x;
	int pow115 = pow114 * x;
	int pow116 = pow115 * x;
	int pow117 = pow116 * x;
	int pow118 = pow117 * x;
	int pow119 = pow118 * x;
	int pow120 = pow119 * x;
	int pow121 = pow120 * x;
	int pow122 = pow121 * x;
	int pow123 = pow122 * x;
	int pow124 = pow123 * x;
	int pow125 = pow124 * x;
	int pow126 = pow125 * x;
	int pow127 = pow126 * x;
	int pow128 = pow127 * x;
	for (i = degree - 1; i >= 127; i -= 128) {
		result *= pow128;
		result += a[i - 0] * pow127;
		result += a[i - 1] * pow126;
		result += a[i - 2] * pow125;
		result += a[i - 3] * pow124;
		result += a[i - 4] * pow123;
		result += a[i - 5] * pow122;
		result += a[i - 6] * pow121;
		result += a[i - 7] * pow120;
		result += a[i - 8] * pow119;
		result += a[i - 9] * pow118;
		result += a[i - 10] * pow117;
		result += a[i - 11] * pow116;
		result += a[i - 12] * pow115;
		result += a[i - 13] * pow114;
		result += a[i - 14] * pow113;
		result += a[i - 15] * pow112;
		result += a[i - 16] * pow111;
		result += a[i - 17] * pow110;
		result += a[i - 18] * pow109;
		result += a[i - 19] * pow108;
		result += a[i - 20] * pow107;
		result += a[i - 21] * pow106;
		result += a[i - 22] * pow105;
		result += a[i - 23] * pow104;
		result += a[i - 24] * pow103;
		result += a[i - 25] * pow102;
		result += a[i - 26] * pow101;
		result += a[i - 27] * pow100;
		result += a[i - 28] * pow99;
		result += a[i - 29] * pow98;
		result += a[i - 30] * pow97;
		result += a[i - 31] * pow96;
		result += a[i - 32] * pow95;
		result += a[i - 33] * pow94;
		result += a[i - 34] * pow93;
		result += a[i - 35] * pow92;
		result += a[i - 36] * pow91;
		result += a[i - 37] * pow90;
		result += a[i - 38] * pow89;
		result += a[i - 39] * pow88;
		result += a[i - 40] * pow87;
		result += a[i - 41] * pow86;
		result += a[i - 42] * pow85;
		result += a[i - 43] * pow84;
		result += a[i - 44] * pow83;
		result += a[i - 45] * pow82;
		result += a[i - 46] * pow81;
		result += a[i - 47] * pow80;
		result += a[i - 48] * pow79;
		result += a[i - 49] * pow78;
		result += a[i - 50] * pow77;
		result += a[i - 51] * pow76;
		result += a[i - 52] * pow75;
		result += a[i - 53] * pow74;
		result += a[i - 54] * pow73;
		result += a[i - 55] * pow72;
		result += a[i - 56] * pow71;
		result += a[i - 57] * pow70;
		result += a[i - 58] * pow69;
		result += a[i - 59] * pow68;
		result += a[i - 60] * pow67;
		result += a[i - 61] * pow66;
		result += a[i - 62] * pow65;
		result += a[i - 63] * pow64;
		result += a[i - 64] * pow63;
		result += a[i - 65] * pow62;
		result += a[i - 66] * pow61;
		result += a[i - 67] * pow60;
		result += a[i - 68] * pow59;
		result += a[i - 69] * pow58;
		result += a[i - 70] * pow57;
		result += a[i - 71] * pow56;
		result += a[i - 72] * pow55;
		result += a[i - 73] * pow54;
		result += a[i - 74] * pow53;
		result += a[i - 75] * pow52;
		result += a[i - 76] * pow51;
		result += a[i - 77] * pow50;
		result += a[i - 78] * pow49;
		result += a[i - 79] * pow48;
		result += a[i - 80] * pow47;
		result += a[i - 81] * pow46;
		result += a[i - 82] * pow45;
		result += a[i - 83] * pow44;
		result += a[i - 84] * pow43;
		result += a[i - 85] * pow42;
		result += a[i - 86] * pow41;
		result += a[i - 87] * pow40;
		result += a[i - 88] * pow39;
		result += a[i - 89] * pow38;
		result += a[i - 90] * pow37;
		result += a[i - 91] * pow36;
		result += a[i - 92] * pow35;
		result += a[i - 93] * pow34;
		result += a[i - 94] * pow33;
		result += a[i - 95] * pow32;
		result += a[i - 96] * pow31;
		result += a[i - 97] * pow30;
		result += a[i - 98] * pow29;
		result += a[i - 99] * pow28;
		result += a[i - 100] * pow27;
		result += a[i - 101] * pow26;
		result += a[i - 102] * pow25;
		result += a[i - 103] * pow24;
		result += a[i - 104] * pow23;
		result += a[i - 105] * pow22;
		result += a[i - 106] * pow21;
		result += a[i - 107] * pow20;
		result += a[i - 108] * pow19;
		result += a[i - 109] * pow18;
		result += a[i - 110] * pow17;
		result += a[i - 111] * pow16;
		result += a[i - 112] * pow15;
		result += a[i - 113] * pow14;
		result += a[i - 114] * pow13;
		result += a[i - 115] * pow12;
		result += a[i - 116] * pow11;
		result += a[i - 117] * pow10;
		result += a[i - 118] * pow9;
		result += a[i - 119] * pow8;
		result += a[i - 120] * pow7;
		result += a[i - 121] * pow6;
		result += a[i - 122] * pow5;
		result += a[i - 123] * pow4;
		result += a[i - 124] * pow3;
		result += a[i - 125] * pow2;
		result += a[i - 126] * pow1;
		result += a[i - 127];
	}
	for (; i >= 0; i--) {
		result *= x;
		result += a[i];
	}
	return result;
}

/* Loop Spiltting, parallelism*2 */
int poly_evalS(int *a, int degree, int x) {
	int t1 = a[degree];
	int t2 = a[degree - 1];
	int i;
	int temp = x * x;
	for (i = degree - 2; i > 0; i -= 2) {
		t1 *= temp;
		t2 *= temp;
		t1 += a[i];
		t2 += a[i - 1];
	}
	if (i == 0) {
		t1 *= temp;
		t2 *= x;
		t1 += a[0];
	} else {
		t1 *= x;
	}
	t1 += t2;
	return t1;
}
/* parallelism*3 */
int poly_eval2S(int *a, int degree, int x) {
	int i, result;
	if (degree == 10) {
		int pow2 = x * x * x;
		// Use 3 varible to keep the value.
		int t3 = a[10] * pow2;
		int t2 = a[9] * pow2;
		int t1 = a[8] * pow2;
		// Mutiple and add.
		t3 += a[7];
		t2 += a[6];
		t1 += a[5];
		t3 *= pow2;
		t2 *= pow2;
		t1 *= pow2;
		t3 += a[4];
		t2 += a[3];
		t1 += a[2];
		t3 *= pow2;
		t2 *= pow2;
		t1 = t1 * x * x;
		t3 += a[1];
		t3 *= x;
		// Plus the 3 parts together.
		t2 += a[0];
		t1 += t3;
		t1 += t2;
		result = t1;
	}
	// In other conditions, Loop Unrolling*4;
	else {
		result = a[degree];
		int pow1 = x;
		int pow2 = pow1 * x;
		int pow3 = pow2 * x;
		int pow4 = pow3 * x;
		for (i = degree - 1; i >= 3; i -= 4) {
			result *= pow4;
			result += a[i - 0] * pow3;
			result += a[i - 1] * pow2;
			result += a[i - 2] * pow1;
			result += a[i - 3];
		}
		for (; i >= 0; i--) {
			result *= x;
			result += a[i];
		}
	}

	return result;
}
/* This table contains a list, where each entry gives the name
 of an evaluation function, along with a string describing
 the function.
 Use this table to create a list with a number of different solutions.
 Then pick the best ones for your actual solution
 */

peval_fun_rec peval_fun_tab[] = {

/* Your first entry should be the function with best CPE */
{ poly_eval96, "Best CPE" },
/* Your second entry should be the function with best
 performance on degree 10 */
{ poly_eval2S, "Best for 10" },

{ poly_eval, "poly_eval: Reference implementation" },

/* This is the null terminator that marks the end of the function list.
 Leave this here. */
{ NULL, "" } };

