#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <regex>

using namespace std;

void analize_line(string line);
void add_base_class(string line);
void add_new_method(string line);

string last_method;
map<string, map<string, string>> class_map;
regex base_class("^extends.*");
regex method("^func.*");

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
            for (const auto &o : p.second)
            {
                cout << "class_map['" << p.first << "']['" << o.first << "'] = " << o.second << "\n";
            }
        }
    }
    else
        cout << "Unable to open file";

    return 0;
}

void analize_line(string line)
{
    if (regex_match(line, base_class))
    {
    }
    else if (regex_match(line, method))
    {
        add_new_method(line);
    }
}

void add_base_class(string line)
{
    if (class_map.find("*base") == class_map.end())
    {
        map<string, string> base_map;
        class_map.insert(pair<string, map<string, string>>("*base", base_map));
    }
    class_map["*base"].insert(pair<string, string>("class_name", line.substr(8)));
}

void add_new_method(string line)
{
    string method_line = line.substr(5);
    int name_end = method_line.find("(");
    string method_name = method_line.substr(0, name_end);
    last_method = method_name;
    if (class_map.find(method_name) == class_map.end())
    {
        map<string, string> method_map;
        class_map.insert(pair<string, map<string, string>>(method_name, method_map));
    }
    class_map[method_name].insert(pair<string, string>("declaration", method_line));
    int args_end = method_line.find(")");
    string args = method_line.substr(name_end + 1, args_end - name_end - 1);
    class_map[method_name].insert(pair<string, string>("args", args));
}