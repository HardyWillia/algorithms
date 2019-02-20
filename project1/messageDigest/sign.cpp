/*
    Willia Hardy
    Algorithms Project 1 Part 2
    Student ID: 2905326
*/

#include "BigIntegerLibrary.hh"
#include "sha256.h"
#include <iostream>
#include <fstream>
#include <string.h>

// Function to hash the char array and return the SHA256 hash
// in a BigUnsigned data type
BigUnsigned hashMemblock(char memblock[]) {
  // Convert to string and hash using sha256
  std::string memblockString(memblock);
  std::string hashedFileContent = sha256(memblockString);

  // Turn hashed string into BigUnsigned data type
  BigUnsignedInABase hashedFileContentNum =
      BigUnsignedInABase(hashedFileContent, 16);
  return BigUnsigned(hashedFileContentNum);
}

int main(int argc, char *argv[]) {
   if (argc != 3 || (argv[1][0]!='s' && argv[1][0]!='v')) 
      std::cout << "wrong format! should be \"a.exe s filename\"";
    
    else {
    std::string filename = argv[2];

    // Read the file
    std::streampos begin, end;
    std::ifstream myfile(filename.c_str(), std::ios::binary);
    begin = myfile.tellg();
    myfile.seekg(0, std::ios::end);
    end = myfile.tellg();
    std::streampos size = end - begin;

    myfile.seekg(0, std::ios::beg);
    char *memblock = new char[size];
    myfile.read(memblock, size);
    myfile.close();

    // Generate a SHA-256 hash of the contents of the file to be signed
    BigUnsigned hash = hashMemblock(memblock);
    std::cout << "Hashed contents from file.txt: " << hash << std::endl;

    //Sign the file with the private key stored in d_n.txt
    if (argv[1][0] == 's') {
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

    } else {
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