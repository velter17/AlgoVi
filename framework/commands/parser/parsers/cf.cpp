#include <bits/stdc++.h>

typedef std::pair<std::string, std::string> Test;
typedef std::vector <Test> TestCollection;

std::vector <std::string> split(const std::string& str)
{
	std::stringstream stream(str);
	std::string buf;
	std::vector <std::string> ret;
	while(std::getline(stream, buf))
	{
		ret.push_back(buf);
	}
	return ret;
}

bool writeTests(const TestCollection& tests, std::string filePath)
{
	std::ofstream out(filePath);
	if(!out.is_open())
	{
		return false;
	}
	out << tests.size() << "\n";
	for(std::size_t i = 0; i < tests.size(); ++i)
	{
		const std::string& input = tests[i].first;
		const std::string& output = tests[i].second;
		std::vector <std::string> inputVector = split(input);
		std::vector <std::string> outputVector = split(output);
		out << inputVector.size() << " " << outputVector.size() << "\n";
		for(std::size_t j = 0; j < inputVector.size(); ++j)
		{
			out << inputVector[j] << "\n";
		}
		for(std::size_t j = 0; j < outputVector.size(); ++j)
		{
			out << outputVector[j] << "\n";
		}
	}
	out.close();
	return true;
}

std::string extractData(const std::string& str)
{
	std::string ret;
    int cnt = 0;
    for(int i = 0; i < str.length(); ++i)
    {
        if(str[i] == '<')
            ++cnt;
        else if(str[i] == '>')
            --cnt;
        else if(cnt == 0)
        {
            if(i > 0 && !ret.empty() && str[i-1] == '>')
                ret.push_back('\n');
            ret.push_back(str[i]);
        }
    }
    return ret;
}

char toLower(char c)
{
	if(c >= 'A' && c <= 'Z')
		c = c - 'A' + 'a';
	return c;
}

std::string readHtmlFile(const std::string& htmlFilePath)
{
	std::ifstream file(htmlFilePath);
	if(!file.is_open())
	{
		return std::string();
	}
	std::string ret, buf;
	while(std::getline(file, buf))
	{
		ret += buf + "\n";
	}
	return ret;
}

TestCollection readTests(const std::string& htmlFilePath)
{
	TestCollection collection;

	std::string htmlContent = readHtmlFile(htmlFilePath);

	std::string from_str = "pre";
    std::string to_str = "/pre";
    int prev = 0;

    int state = 0;
    // 0 - init
    // 1 - inside open tag
    // 2 - outside open tag
    // 3 - inside close tag

    std::string tagStr;
    int openTagIn[2], closeTagOut;
    int counter = 0;

    for(int i = 0; i < htmlContent.length(); ++i)
    {
        if(htmlContent[i] == '<')
        {
            if(state == 0)
            {
                openTagIn[counter] = i;
                state = 1;
            }
            else if(state == 2)
            {
                state = 3;
            }
        }
        else if(htmlContent[i] == '>')
        {
            if(state == 1)
            {
                if(tagStr == from_str)
                {
                    state = 2;
                }
                else
                    state = 0;
            }
            else if(state == 3)
            {
                if(tagStr == to_str)
                {
                    if(counter == 1)
                    {
                        collection.emplace_back(extractData(htmlContent.substr(openTagIn[0],closeTagOut-openTagIn[0]+1)),
                        						extractData(htmlContent.substr(openTagIn[1], i-openTagIn[1]+1)));
                        counter = 0;
                    }
                    else
                    {
                        counter++;
                        closeTagOut = i;
                    }
                    state = 0;
                }
                else
                    state = 2;
            }
            tagStr.clear();
        }
        else if(state == 1 || state == 3)
            tagStr.push_back(toLower(htmlContent[i]));
	}
	return collection;
}

int main(int argc, char** argv)
{
	std::string testsPath = argv[1];
	std::string htmlFilePath = argv[2];
	TestCollection data = readTests(htmlFilePath);
	if(!writeTests(data, testsPath))
	{
		return 1;
	}
    return 0;
}
