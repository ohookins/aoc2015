#include <iostream>
#include <string>
#include <CommonCrypto/CommonCrypto.h>

const std::string SECRET_KEY = "iwrupvqb";

// Wrapper around MacOS framework function for MD5 generation.
// This will always generate a deprecation warning when compiling
// for obvious reasons.
std::string MD5(std::string Input)
{
    unsigned char md5[CC_MD5_DIGEST_LENGTH];
    CC_MD5(Input.c_str(), Input.size(), md5);

    std::string Encoding = "0123456789abcdef";
    std::string Result;

    // This is the simplest way I could figure out how to
    // generate a hex string from the md5 without messing
    // with recasting and weird stdlib packages.
    for (int i = 0; i < CC_MD5_DIGEST_LENGTH; i++)
    {
        Result.push_back(Encoding[md5[i] >> 4]);
        Result.push_back(Encoding[md5[i] & 0x0F]);
    }

    return Result;
}

// brute force generation of MD5s until we get a collision
void bruteForceMD5(std::string CollisionPrefix)
{
    int i = 0;
    while(i < INT_MAX)
    {
        std::string TestString = SECRET_KEY + std::to_string(i);
        std::string Result = MD5(TestString);

        if (Result.substr(0, CollisionPrefix.size()) == CollisionPrefix)
        {
            std::cout << Result << " had prefix collision with test string " << i << std::endl;
            return;
        }

        i++;
    }
}

int main(int argc, char** argv)
{
    bruteForceMD5("00000");
    bruteForceMD5("000000");
}