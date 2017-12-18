#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <regex>

using namespace std;

int line_nr = 0;
void analize_line(string line);
void add_base_class(string line);
void add_new_method(string line);
void add_new_line(string line);
static inline void ltrim(std::string &s);
static inline void rtrim(std::string &s);

string last_method = "#base";
map<string, map<string, string>> class_map;
regex base_class("^extends.*");
regex method("^func.*");
regex tabbed("^\t.*");

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
            rtrim(line);
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
        add_base_class(line);
    }
    else if (regex_match(line, method))
    {
        add_new_method(line);
    }
    else if (regex_match(line, tabbed))
    {
        add_new_line(line);
    }
}

void add_base_class(string line)
{
    if (class_map.find("#base") == class_map.end())
    {
        map<string, string> base_map;
        class_map.insert(pair<string, map<string, string>>("#base", base_map));
    }
    class_map["#base"].insert(pair<string, string>("class_name", line.substr(8)));
}

void add_new_method(string line)
{
    string method_line = line.substr(5);
    int name_end = method_line.find("(");
    string method_name = method_line.substr(0, name_end);
    last_method = method_name;
    line_nr = 0;
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

void add_new_line(string line)
{
    class_map[last_method].insert(pair<string, string>("line_" + to_string(++line_nr), line));
}

// trim from start (in place)
static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
}