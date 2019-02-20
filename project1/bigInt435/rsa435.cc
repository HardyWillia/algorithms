// You need to complete this program for a part of your first project.

/*
   Willia Hardy
   Algorithms Project 1 Part 1
   Student ID: 2905326
*/

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <cmath>

// 'BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"

//Function to implement the Fermat test
bool fermatTest(BigUnsigned f){
   BigUnsigned f1(1);
   BigUnsigned f2(2);
   return (modexp(f1, f - 1, f) == 1) && (modexp(f2, f - 1, f) == 1);
}

BigUnsigned getRandom(BigUnsigned p){
   BigUnsigned x = BigUnsigned(1);
   for(int i = 0; p >= i; i++){
      x = x * 10 + rand();
   }

   return x;
}

//Use Fermat's test to generate two large prime numbers (p, q) each with size greater than or equal to 512 bits (a number with 155 decimal digits)
BigUnsigned getLargePrime(){
   BigUnsigned x = getRandom(155);

   while(!fermatTest(x)){
      x = getRandom(155);
   }

   return x;
}

//Use the Extended Euclidean Algorithm to check if it is relatively prime
bool relativelyPrime(BigUnsigned x, BigUnsigned y){
   return gcd(x, y) == 1;
}

BigUnsigned findE(BigUnsigned phi){
   
   BigUnsigned e = getRandom(100);
   while(!relativelyPrime(e, phi)){
      e = getRandom(100);
   }

   return e;
}

int main(){
	/* The library throws 'const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a 'try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */

   srand(time(NULL));

	try {

		//Test cases that aren't necessary right now     
      /* std::cout << "a couple of test cases for 3460:435/535 Algorithms!!!\n";
      // BigUnsigned big1 = BigUnsigned(1);
      // for (int i=0;i<400;i++) {
      //    big1 = big1*10 +rand();
      // }
      // std::cout << "my big1 !!!\n";
      // std::cout << big1;
      // BigUnsigned big2 = BigUnsigned(1);
      // for (int i=0;i<400;i++) {
          big2 = big2*10 +rand();
       }
      std::cout << "my big2 !!!\n";
      std::cout << big2;
      std::cout << "my big3 = big1*big2 !!!\n";
      BigUnsigned big3 = big1*big2;
      std::cout <<big3;
      std::cout << "my big3/big2 !!!\n";
       std::cout <<big3/big2;
      */
      
      std::cout << "Finding two LARGE prime numbers (please wait about 3 minutes)..." << std::endl;

      //Get first prime and store it into p
      BigUnsigned p = getLargePrime();
      std::cout << "p: " << p << std::endl;
      
      //Get second prime and store it into q
      BigUnsigned q = getLargePrime();
      std::cout << "q: " << q << std::endl;

      //Save p and q in a file named p_q.txt, one integer per line and making sure no white space is saved
      std::cout << "Outputting to file..." << std::endl;
      std::ofstream p_q("p_q.txt");
      p_q << p << "\n" << q;
      p_q.close();

      //Compute the value for n 
      BigUnsigned n = p * q;

      //Compute the value for phi(n)
      BigUnsigned phi = (p - 1) * (q - 1);
      if (phi % 2 == 0)
      {
         phi++;
      }

      //Find the value for e (secret key)
      BigUnsigned e = findE(phi);
      std::cout << e << std::endl;

      //Save the key pair for e in a file called e_n.txt
      std::cout << "Outputting the value of e to a file..." << std:: endl;
      std::ofstream e_n("e_n.txt");
      e_n << e << "\n" << n;
      e_n.close();

      //Find the value for d (public key)
      BigUnsigned d = modinv(e, phi);
      std::cout << d << std::endl;

      //Save the key pair for d in a file called d_n.txt
      std::cout << "Outputting the value of d to a file..." << std::endl;
      std::ofstream d_n("d_n.txt");
      d_n << d << "\n" << n;
      d_n.close();

      std::cout << "RSA Encryption is complete!" << std::endl;
      
	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}

	return 0;
}