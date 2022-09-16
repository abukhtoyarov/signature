#include <string>
#include "gtest/gtest.h"
#include "../src/md5.h"

using namespace sig;
using namespace std;

TEST(md5, hash) {
    {
        // MD5("md5") = 1BC29B36F623BA82AAF6724FD3B16718
        constexpr const char* test = "md5";
        EXPECT_EQ(calcHash(test), "1BC29B36F623BA82AAF6724FD3B16718");
    }
    {
        // MD5("md5") = 1BC29B36F623BA82AAF6724FD3B16718
        constexpr const char test[] = "md5";
        EXPECT_EQ(calcHash(test, sizeof(test) - 1), "1BC29B36F623BA82AAF6724FD3B16718");
    }
    {
        constexpr const char test[] = "md5\x0A";
        EXPECT_EQ(calcHash(test, sizeof(test) - 1), "772AC1A55FAB1122F3B369EE9CD31549");
    }
    {
        // MD5("") = D41D8CD98F00B204E9800998ECF8427E
        constexpr const char* test = "";
        EXPECT_EQ(calcHash(test), "D41D8CD98F00B204E9800998ECF8427E");
    }
    {
        // MD5("The quick brown fox jumps over the lazy dog") = 9e107d9d372bb6826bd81d3542a419d6
        constexpr const char* test = "The quick brown fox jumps over the lazy dog";
        EXPECT_EQ(calcHash(test), "9E107D9D372BB6826BD81D3542A419D6");
    }
    {
        // MD5("The quick brown fox jumps over the lazy dog.") = e4d909c290d0fb1ca068ffaddf22cbd0
        constexpr const char* test = "The quick brown fox jumps over the lazy dog.";
        EXPECT_EQ(calcHash(test), "E4D909C290D0FB1CA068FFADDF22CBD0");
    }
    {
        // MD5("Yoda said, Do or do not. There is no try.") = e4d909c290d0fb1ca068ffaddf22cbd0
        constexpr const char* test = "Yoda said, Do or do not. There is no try.";
        EXPECT_EQ(calcHash(test), "CF4A16604182539B5CE297092A034625");
    }
    {
        std::vector<char> test(512);
        test.resize(512); // expand to min block size
        EXPECT_EQ(calcHash(test.data(), test.size()), "BF619EAC0CDF3F68D496EA9344137E8B");
    }
    {
        std::vector<char> test(512);
        test[0] = 'm';
        test[1] = 'd';
        test[2] = '5';
        EXPECT_EQ(calcHash(test.data(), test.size()), "E14D588DD1E5E88CFE8EBE525F868F86");
    }
    {
        std::vector<char> test(1024);
        EXPECT_EQ(calcHash(test.data(), test.size()), "0F343B0931126A20F133D67C2B018A3B");
    }
    {
        std::vector<char> test(1024*1024);
        EXPECT_EQ(calcHash(test.data(), test.size()), "B6D81B360A5672D80C27430F39153E2C");
    }
    {
        std::vector<char> test(1024 * 1024 * 10);
        EXPECT_EQ(calcHash(test.data(), test.size()), "F1C9645DBC14EFDDC7D8A322685F26EB");
    }
}
