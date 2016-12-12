#ifndef YCC_MAIN_H_
#define YCC_MAIN_H_

#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <bitset>

// version info
const std::string APPNAME = "ycc";
const std::string VERSION = "1.0.0";

enum OpTag
{
    EXE = 0,                // default state
    ASM,                    // -S output .asm file
    HELP,                   // print manuals to screen
    VER_INFO,               // print version info to screen
    DUMP_TOKENS,            // dump token stream
    DUMP_AST,               // dump ast
    DUMP_IR,                // dump ir list
    DUMP_SYMBOL_TABLE,      // dump symbol table
    OUTPUT                  // output file name
};

std::map<std::string, OpTag>            opMap;
std::map<std::string, std::string>      manuals;
std::bitset<32>                         options(0);
std::string                             srcFileName = "";
std::string                             dstFileName = "a.out";
bool                                    errorFlag = false;

void manualReport()
{
    std::cout << "\nUsage: ycc [option] <filename>\n";
    std::cout << "where possible options include :\n";
    for(auto elem : manuals)
    {
        std::cout << std::left << "  " << std::setw(26) << elem.first
             << std::setw(60) << elem.second << std::endl;
    }
    std::cout << std::endl;
}

void addOption(OpTag op)
{
    options.set(op);
}

bool checkOption(OpTag op)
{
    return options.test(op);
}

void errorReport(const std::string &msg)
{
    std::cerr << "ycc: fatal error: " << msg << std::endl;
    manualReport();
    errorFlag = true;
}

void commandHandle(int argc, char **argv)
{
    if(argc < 2)
    {
        errorReport("no file input");
        return ;
    }

    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            // option
            auto iter = opMap.find(argv[i]);
            if(iter == opMap.end())
            {
                std::string msg(argv[i]);
                errorReport("no such option" + msg);
                return ;
            }
            else
            {
                addOption(iter->second);
                // check output file name
                if(argv[i][1] == 'o')
                {
                    if(argv[i+1][0] != '-')
                    {
                        dstFileName = std::string(argv[++i]);
                    }
                    else
                    {
                        errorReport("requaired an output file name after \"-o\"");
                        return ;
                    }
                }
            }
        }
        else
        {
            // filename
            if(srcFileName != "")
            {
                errorReport("too much source file");
            }
            srcFileName = argv[i];
        }
    } // for
}

void init(int argc, char *argv[])
{
    opMap.insert(std::pair<std::string, OpTag>("--dump-tokens", OpTag::DUMP_TOKENS));
    opMap.insert(std::pair<std::string, OpTag>("--dump-ast", OpTag::DUMP_AST));
    opMap.insert(std::pair<std::string, OpTag>("--dump-ir", OpTag::DUMP_IR));
    opMap.insert(std::pair<std::string, OpTag>("--dump-symbol-table", OpTag::DUMP_SYMBOL_TABLE));
    opMap.insert(std::pair<std::string, OpTag>("-h", OpTag::HELP));
    opMap.insert(std::pair<std::string, OpTag>("--help", OpTag::HELP));
    opMap.insert(std::pair<std::string, OpTag>("-o", OpTag::OUTPUT));
    opMap.insert(std::pair<std::string, OpTag>("--output", OpTag::OUTPUT));
    opMap.insert(std::pair<std::string, OpTag>("-S", OpTag::ASM));
    opMap.insert(std::pair<std::string, OpTag>("--asm", OpTag::ASM));
    opMap.insert(std::pair<std::string, OpTag>("-v", OpTag::VER_INFO));
    opMap.insert(std::pair<std::string, OpTag>("--version", OpTag::VER_INFO));
    manuals.insert(std::pair<std::string, std::string>("--dump-tokens", "output token stream"));
    manuals.insert(std::pair<std::string, std::string>("--dump-ast", "output depth travelling of ast"));
    manuals.insert(std::pair<std::string, std::string>("--dump-ir", "output intermediate representations"));
    manuals.insert(std::pair<std::string, std::string>("-h, --help", "help information"));
    manuals.insert(std::pair<std::string, std::string>("-o, --output", "output file name"));
    manuals.insert(std::pair<std::string, std::string>("-S, --asm", "output asm code"));
    manuals.insert(std::pair<std::string, std::string>("-v, --version", "version info"));

    commandHandle(argc, argv);
}

#endif
