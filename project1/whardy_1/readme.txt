Willia Hardy 2905326
Algorithms Project 1 README

Part 1 (rsa435.cc)
- The purpose of part 1 was to implement the rsa encryption using the alogirthm discussed in class
- For the encryption to be implemented, this information was needed:
    - The large primes (p and q)
        - srand() was used to constantly generate a random number each time the variables were accessed
    - The value of n (n = p * q)
        - This was simply multiplying the two large primes in the program
    - The value of phi(n) was used after the fermat test was implemented on the random generated number.
        - phi(n) has the formula phi(n) = (p-1)(q-1)
    - The value of e (secret key)
        - E was implemented using the findE() function
        - E had to be checked against phi to determine if it was relatively prime with the value of n
    - The value of d (public key)
        - d is simply the modular inverse of E. This was implemented using the modinv() method 
- These values were calculated using modular math and stored in files called p_q.txt, e_n.txt, and d_n.txt
    - The combination (e, n) and (d, n) are what makes up the private/public key, respectively.


Part 2 (sign.cpp)
-
-