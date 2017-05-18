#include <bits/stdc++.h>

std::vector <std::string> split(const std::string& str)
{
    std::vector <std::string> ret;
    std::string buf;
    for(std::size_t i = 0; i < str.length(); ++i)
    {
        if(str[i] == ' ' || str[i] == '\n')
        {
            if(!buf.empty())
                ret.push_back(buf);
            buf.clear();
        }
        else
            buf += str[i];
    }
    if(!buf.empty())
        ret.push_back(buf);
    return ret;
}

std::string readKeyWord()
{
    std::ifstream file("algovi_debug.h");
    if(!file.is_open())
    {
        std::cerr << "Cannot open 'algovi_debug.h'\n";
        return std::string();
    }
    std::string buf;
    if(!std::getline(file, buf))
        return std::string();
    std::vector <std::string> list = split(buf);
    if(list.size() < 2 || list[0] != "#define")
    {
        std::cerr << "Wrong keyWord line in algovi_debug.h: " << buf << "\n";
        return std::string();
    }

    return list[1];
}

bool startWith(const std::string& str, const std::string& pattern)
{
    return str.length() >= pattern.length() && str.substr(0, pattern.length()) == pattern;
}

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        std::cerr << "Wrong arguments\n";
        return 1;
    }

    std::string srcFilePath = argv[1];
    std::string dstFilePath = argv[2];
    std::ifstream srcFile(srcFilePath);
    std::ofstream dstFile(dstFilePath);

    if(!srcFile.is_open())
    {
        std::cerr << "Cannot open source file\n";
        return 1;
    }
    if(!dstFile.is_open())
    {
        std::cerr << "Cannot open destination file\n";
        return 1;
    }

    std::string keyWord = readKeyWord();
    if(keyWord.empty())
    {
        std::cerr << "No keyword\n";
        return 1;
    }

    std::string buf;
    bool debugOutput = false;
    while(std::getline(srcFile, buf))
    {
        std::vector <std::string> tokens = split(buf);
        if(tokens.empty())
        {
            dstFile << "\n";
            continue;
        }
        if(startWith(tokens.front(), keyWord + "_start"))
        {
            debugOutput = true;
        }
        else if(startWith(tokens.front(), keyWord + "_end"))
        {
            debugOutput = false;
        }
        if(debugOutput || startWith(tokens.front(), keyWord) 
                || startWith(buf, "#define \"algovi_debug.h\""))
        {
            continue;
        }
        dstFile << buf << "\n";
    }
    return 0;
}
