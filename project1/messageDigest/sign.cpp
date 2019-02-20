/*
    Willia Hardy
    Algorithms Project 1 Part 2
    Student ID: 2905326
*/

/***
   prepared for CS435 Project 1 part 2
**/

#include <string.h>
#include <iostream>
#include <fstream>
#include "sha256.h"
#include "BigIntegerLibrary.hh"

BigUnsigned hashMemblock(char memblock[]) {
  // Convert to string and hash using sha256
  std::string memblockString(memblock);
  std::string hashedFileContent = sha256(memblockString);

  // Turn hashed string into BigUnsigned data type
  BigUnsignedInABase hashedFileContentNum =
      BigUnsignedInABase(hashedFileContent, 16);
  return BigUnsigned(hashedFileContentNum);
}
 
int main(int argc, char *argv[])
{
   //demonstrating how sha256 works
   std::string input = "testing";
   std::string output1 = sha256(input);
   std::cout << "sha256('"<< input << "'):" << output1 << "\n";
   
   //demo bigInt works here
   BigUnsigned a = stringToBigUnsigned("124338907642982722929222542626327282");
   BigUnsigned b = stringToBigUnsigned("124338907642977775546469426263278643");
   std::cout << "big a = " <<a<<"\n";
   std::cout << "big b = " <<b<<"\n";
   std::cout << "big a*b = " <<a*b<<"\n";

   //Second part of your project starts here
   if (argc != 3 || (argv[1][0]!='s' && argv[1][0]!='v')) 
      std::cout << "wrong format! should be \"a.exe s filename\"";
   else {
      std::string filename = argv[2];
      
            
      //read the file
      std::streampos begin,end;
      std::ifstream myfile (filename.c_str(), std::ios::binary);
      begin = myfile.tellg();
      myfile.seekg (0, std::ios::end);
      end = myfile.tellg();
      std::streampos size = end-begin;
      //std::cout << "size of the file: " << size << " bytes.\n"; //size of the file
      
      myfile.seekg (0, std::ios::beg);
      char * memblock = new char[size];
      myfile.read (memblock, size); //read file; it's saved in the char array memblock
      myfile.close();
      
      std::string copyOFfile = filename+".Copy"; 
      std::ofstream myfile2 (copyOFfile.c_str(), std::ios::binary);
      myfile2.write (memblock, size); //write to a file
      myfile2.close();
      
          // Generate a SHA-256 hash of the contents of the file to be signed
       BigUnsigned hash = hashMemblock(memblock);
       std::cout << "Hashed contents from file.txt: " << hash << std::endl;
      //std::cout<<memblock;
        
      if (argv[1][0]=='s') {
         std::cout << "\n"<<"Need to sign the doc.\n";
         std::cout << "Signing the document " << filename << "..." << std::endl;

         // Load in d_n.txt since it is not in the current directory
         std::ifstream d_nfile("d_n.txt");
         std::string dString, nString;
         while (d_nfile >> dString >> nString) { }
         d_nfile.close();

         //Grab the values of d and n from the file
         BigUnsigned d = stringToBigUnsigned(dString);
         BigUnsigned n = stringToBigUnsigned(nString);
         std::cout << "d from d_n.txt: " << d << std::endl;
         std::cout << "n from d_n.txt: " << n << std::endl;

         // Sign the hashed contents
         BigUnsigned signedContents = modexp(hash, d, n);
         std::cout << "Signed contents: " << signedContents << std::endl;

         // Save contents to file.txt.signature
         std::ofstream outfile;
         outfile.open("file.txt.signature",
                     std::ofstream::out | std::ofstream::trunc);
         outfile << signedContents;
         outfile.close();

         std::cout << filename << " Signed and saved to \"file.txt.signature\"!"
                  << std::endl;
            
      }
      else {
         std::cout << "\n"<<"Need to verify the doc.\n";
         std::cout << "Verifying the signature in the file..." << std::endl;

         // read contents of signature file
         std::string signaturefilename = argv[3];
         std::ifstream sigfile(signaturefilename.c_str());
         std::string signatureString;
         while (sigfile >> signatureString) {
         }
         sigfile.close();
         BigUnsigned signature = stringToBigUnsigned(signatureString);
         std::cout << "Signature from file.txt.signature: " << signature
                  << std::endl;

         //Read contents of e_n.txt since it is not in the current directory
         std::ifstream e_nfile("e_n.txt");
         std::string eString, nString;
         while (e_nfile >> eString >> nString) {
         }
         e_nfile.close();

         //Grab the values of e and n
         BigUnsigned e = stringToBigUnsigned(eString);
         BigUnsigned n = stringToBigUnsigned(nString);
         std::cout << "e from e_n.txt: " << e << std::endl;
         std::cout << "n from e_n.txt: " << n << std::endl;

         //"Encrypt" signature gotten from file with e and n
         BigUnsigned encryptedSignature = modexp(signature, e, n);
         std::cout << "Encrypted signature: " << encryptedSignature << std::endl;

         //Compare hash with the "encrypted" signature
         if (encryptedSignature == hash)
         std::cout << "Verification passed! file.txt is unchanged after signing"
                     << std::endl;
         else
         std::cout << "Verification failed! file.txt was changed after signing"
                     << std::endl;
            
      }
      delete[] memblock;
    }
    return 0;
}