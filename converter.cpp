#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <regex>

using namespace std;

map<string, string> class_map;
void analize_line(string line);
int method_counter = 0;

int main(int argc, char *args[])
{
    if (argc < 2)
    {
        cout << "provide path to GDScript file";
        return 0;
    }
    string path = args[1];
    string line;
    ifstream myfile(path);
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            analize_line(line);
        }
        myfile.close();
        for (const auto &p : class_map)
        {
            cout << "class_map['" << p.first << "'] = " << p.second << '\n';
        }
    }
    else
        cout << "Unable to open file";

    return 0;
}

regex base_class("^extends.*");
regex method("^func.*");

void analize_line(string line)
{
    if (regex_match(line, base_class))
    {
        class_map.insert(pair<string, string>("base_class", line.substr(8)));
    }
    else if (regex_match(line, method))
    {
        class_map.insert(pair<string, string>("m_" + to_string(++method_counter), line.substr(5)));
    }
}