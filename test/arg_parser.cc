#include <string>
#include "gtest/gtest.h"
#include "../src/arg_parser.h"

using namespace std;
using namespace sig;

TEST(arg_parser, parse_command_line) {
    {
        static const char* args[] = {
            "signature", nullptr
        };
        static int sz = sizeof(args)/sizeof(char*) - 1;

        Config cfg{};
        EXPECT_EQ(1, sz);
        EXPECT_FALSE(parse_command_line(sz, args, cfg));
    }
    {
        static const char* args[] = {
            "signature", "-i", "in_file_name", "-o", "out_file_name", nullptr
        };
        static int sz = sizeof(args)/sizeof(char*) - 1;

        Config cfg{};
        EXPECT_EQ(5, sz);
        EXPECT_TRUE(parse_command_line(sz, args, cfg));
        EXPECT_EQ(cfg.inputFile, "in_file_name");
        EXPECT_EQ(cfg.outputFile, "out_file_name");
        EXPECT_EQ(cfg.blockSize, 1048576);
    }
    {
        static const char* args[] = {
            "signature", "-i", "in_file_name", "-o", "out_file_name", "-b", "1Kb", nullptr
        };
        static int sz = sizeof(args)/sizeof(char*) - 1;

        Config cfg{};
        EXPECT_TRUE(parse_command_line(sz, args, cfg));
        EXPECT_EQ(cfg.inputFile, "in_file_name");
        EXPECT_EQ(cfg.outputFile, "out_file_name");
        EXPECT_EQ(cfg.blockSize, 1024);
    }
    {
        static const char* args[] = {
            "signature", "-i", "in_file_name", "-o", "out_file_name", "-b", "1kb", nullptr
        };
        static int sz = sizeof(args)/sizeof(char*) - 1;

        Config cfg{};
        EXPECT_TRUE(parse_command_line(sz, args, cfg));
        EXPECT_EQ(cfg.inputFile, "in_file_name");
        EXPECT_EQ(cfg.outputFile, "out_file_name");
        EXPECT_EQ(cfg.blockSize, 1024);
    }
    {
        static const char* args[] = {
            "signature", "-i", "in_file_name", "-o", "out_file_name", "-b", "511B", nullptr
        };
        static int sz = sizeof(args)/sizeof(char*) - 1;

        Config cfg{};
        EXPECT_FALSE(parse_command_line(sz, args, cfg));
        EXPECT_EQ(cfg.inputFile, "");
        EXPECT_EQ(cfg.outputFile, "");
        EXPECT_EQ(cfg.blockSize, 0);
    }
    {
        static const char* args[] = {
            "signature", "-i", "in_file_name", "-o", "out_file_name", "-b", "10.3mb", nullptr
        };
        static int sz = sizeof(args)/sizeof(char*) - 1;

        Config cfg{};
        EXPECT_FALSE(parse_command_line(sz, args, cfg));
        EXPECT_EQ(cfg.inputFile, "");
        EXPECT_EQ(cfg.outputFile, "");
        EXPECT_EQ(cfg.blockSize, 0);
    }
    {
        static const char* args[] = {
            "signature", "-i", "in_file_name", "-o", "out_file_name", "-b", "1.1kb", nullptr
        };
        static int sz = sizeof(args)/sizeof(char*) - 1;

        Config cfg{};
        EXPECT_TRUE(parse_command_line(sz, args, cfg));
        EXPECT_EQ(cfg.inputFile, "in_file_name");
        EXPECT_EQ(cfg.outputFile, "out_file_name");
        EXPECT_EQ(cfg.blockSize, 1127);
    }
    {
        static const char* args[] = {
            "signature", "-i", "in_file_name", "-o", "out_file_name", "-b", "11kb", nullptr
        };
        static int sz = sizeof(args)/sizeof(char*) - 1;

        Config cfg{};
        EXPECT_TRUE(parse_command_line(sz, args, cfg));
        EXPECT_EQ(cfg.inputFile, "in_file_name");
        EXPECT_EQ(cfg.outputFile, "out_file_name");
        EXPECT_EQ(cfg.blockSize, 11264);
    }
    {
        static const char* args[] = {
            "signature", "-i", "in_file_name", "-o", "out_file_name", "-b", "11.1kb", nullptr
        };
        static int sz = sizeof(args)/sizeof(char*) - 1;

        Config cfg{};
        EXPECT_TRUE(parse_command_line(sz, args, cfg));
        EXPECT_EQ(cfg.inputFile, "in_file_name");
        EXPECT_EQ(cfg.outputFile, "out_file_name");
        EXPECT_EQ(cfg.blockSize, 11367);
    }
    {
        static const char* args[] = {
            "signature", "-i", "in_file_name", "-o", "out_file_name", "-b", "123.123b", nullptr
        };
        static int sz = sizeof(args)/sizeof(char*) - 1;

        Config cfg{};
        EXPECT_FALSE(parse_command_line(sz, args, cfg));
        EXPECT_EQ(cfg.inputFile, "");
        EXPECT_EQ(cfg.outputFile, "");
        EXPECT_EQ(cfg.blockSize, 0);
    }
    {
        static const char* args[] = {
            "signature", "-i", "in_file_name", "-o", "out_file_name", "-b", "1as", nullptr
        };
        static int sz = sizeof(args)/sizeof(char*) - 1;

        Config cfg{};
        EXPECT_FALSE(parse_command_line(sz, args, cfg));
        EXPECT_EQ(cfg.inputFile, "");
        EXPECT_EQ(cfg.outputFile, "");
        EXPECT_EQ(cfg.blockSize, 0);
    }
}
