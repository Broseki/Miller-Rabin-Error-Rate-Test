/*
 * Miller Rabin Error Rate Test
 * Designed by Michael Canning @ RIT CS
 *
 * References: https://www.sanfoundry.com/c-program-implement-rabin-miller-primality-test-check-number-prime/
 */

#include <stdio.h>

const int START_VAL = 105000;       // Start value of the range to test
const int END_VAL = 120000;         // End value of the range to test


/*
* Calculates (a * b) % mod
*/
long long mulmod(long long a, long long b, long long mod) {
    long long x = 0,y = a % mod;
    while (b > 0) {
        if (b % 2 == 1) {
            x = (x + y) % mod;
        }
        y = (y * 2) % mod;
        b /= 2;
    }
    return x % mod;
}

/*
* Square multiply algorithm
*/
long long modulo(long long base, long long exponent, long long mod) {
    long long x = 1;
    long long y = base;
    while (exponent > 0) {
        if (exponent % 2 == 1)
            x = (x * y) % mod;
        y = (y * y) % mod;
        exponent = exponent / 2;
    }
    return x % mod;
}

/*
* Miller Rabin Algorithm
* @param p The number to test for primality
* @param a The a variable to use in the primality test
* @return True if prime, false if composite
*/
int Miller(long long p,int a) {
    int i;
    long long s;
    if (p < 2) {                        // If p is 1 or less return 0
        return 0;
    }
    if (p != 2 && p % 2==0) {           // If P is not 2 and is even, return false
        return 0;
    }
    s = p - 1;                          // s = p - 1 
    while (s % 2 == 0) {                // Divide p by 2 until it is not even
        s /= 2;
    }
    long long temp = s;                 // Copy s into temp
    long long mod = modulo(a, temp, p);                   // Use square multiply to find (a^temp) mod p
    while (temp != p - 1 && mod != 1 && mod != p - 1) {   // While mod != 1 and mod and temp != p - 1
        mod = mulmod(mod, mod, p);                        // mod = mod * mod % p
        temp *= 2;                                        // temp = temp * 2
    }
    if (mod != p - 1 && temp % 2 == 0) {                  // If mod is not p -1 and temp is odd
        return 0;                                         // p is composite
    }
    return 1;                                             // Otherwise it is probably prime
}

/*
* Runs Miller Rabin for a range of numbers (on odd numbers only) using every possible "a"
* @param start The start of the range
* @param end The end of the range
* @param all_errors An array of floats used to store the error probability of all numbers
*/
void StatLoop(int start, int end, float all_errors[]) {
    for(int x = start; x <= end; x ++) {          // For every odd number in the range
        float composite = 0;                        // Composite count
        float prime = 0;                            // Prime count
        for(int y = 1; y < x; y++) {                // For every possible A
            if(Miller(x, y)) {                      // If Prime add to the prime counter
                prime ++;
            } else {                                // If composite add to the composite counter
                composite ++;
            }
        }
        float error;                                // Error rate
        if(composite != 0) {                        // If prime error = 0
            error = prime / (composite + prime);    // Else prime / (composite + prime)
        } else {
            error = 0;
        }
        all_errors[x - START_VAL] = error;          // Add to error list
        printf("%d -> Composite: %.0f | Prime: %.0f | Error: %.6f\r\n", x, composite, prime, error);
    }
}

/*
* Prints the top ten errors from the all_errors list
* @param all_errors The list of all of the error rate of each of the numbers tested
*/
void PrintTopTen(float all_errors[]) {
    for(int x = 0; x < 10; x ++) {                          // For 10 values
        float largest_error = -1;                           // Set the largest found to -1
        int largest_value = -1;
        for (int i = 0; i <= END_VAL - START_VAL; i++) {    // For every value in the error list
            if(all_errors[i] > largest_error) {             // If the current error is larger than the largest error found
                largest_error = all_errors[i];              // Set the largest error the the error value
                largest_value = i;                          // Set largest value to the index
            }
        }
        all_errors[largest_value] = -1;                     // Set the largest value on the array to -1
        largest_value = largest_value + START_VAL;          // Calculate the value from the index
        printf("%d -> Value: %d | Error: %.6f\r\n", x+1, largest_value, largest_error); // Print
    }
}

int main()
{
    float all_errors[END_VAL - START_VAL];        // Setup error rate list
    StatLoop(START_VAL, END_VAL, all_errors);     // Test all values in range
    printf("-------\r\n");                        // Print Separator
    PrintTopTen(all_errors);                      // Print the top ten errors
    return 0;                                     // Finish
}
