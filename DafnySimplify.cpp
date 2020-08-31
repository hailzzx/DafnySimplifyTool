#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <cstdlib>
#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int cont = 0;
int cnt = 0;
int row_num = 0;
int i;
int j;
int r;
char ch[20], c[20];
bool replaced;
size_t assert_pos;
size_t invariant_pos;
string str;
string str1("Modified_");
string str2("assert");
string str3("invariant");
string str4("//assert");
string str5("//invariant");
string in_dfy;
string out_dfy;
string token;
string comment_line;
string dafny_path("C:\\Users\\hailz\\Project_dafny\\code\\dafny\\Dafny.exe ");

ifstream infile;  //Input file stream
ofstream outfile; //Output filw stream
fstream file1;

bool fexists(const char *filename) //Verify if file exsits
{
    std::ifstream ifile(filename);
    return (bool)ifile;
}

bool comment_detect(string &line, const string comment_str = "//") //Function to detect if line is commented
{
    int n_comment_start = line.find_first_of(comment_str);
    if (n_comment_start != string::npos)
        return true;
    else
        return false;
}

void KeyWordsCount() //Function used to count the sum of "assert" and "invariant" occurrences
{
    while (infile >> token)
    {
        if ("invariant" == token || "assert" == token)
            cont++;
    }
    infile.close();
    infile.open(in_dfy);
}

void RenameFile() //Function to rename the final output file
{
    string last_file = to_string(cont - 1) + str1 + in_dfy;
    string modified_file = str1 + in_dfy;
    const char *old_name = last_file.c_str();
    const char *new_name = modified_file.c_str();

    if (fexists(new_name) != 1)
    {
        rename(old_name, new_name);
    }
    else
        remove(new_name);
    rename(old_name, new_name);
}

void DeleteTempFile()
{
    string dfy("dfy");
    string Texe("exe");
    string Tpdb("pdb");
    string Tdll("dll");

    int n = in_dfy.length();
    char dfy_exe[n + 1];
    char dfy_pdb[n + 1];
    char dfy_dll[n + 1];
    strcpy(dfy_exe, in_dfy.c_str());
    strcpy(dfy_pdb, in_dfy.c_str());
    strcpy(dfy_dll, in_dfy.c_str());
    dfy_exe[n - 1] = 'e';
    dfy_exe[n - 2] = 'x';
    dfy_exe[n - 3] = 'e';
    dfy_pdb[n - 1] = 'b';
    dfy_pdb[n - 2] = 'd';
    dfy_pdb[n - 3] = 'p';
    dfy_dll[n - 1] = 'l';
    dfy_dll[n - 2] = 'l';
    dfy_dll[n - 3] = 'd';
    for (int j = 0; j < cont - 1; j++) //Remove the temp files.dfy generated in iteration cycles
    {
        string jj = to_string(j);
        string remove_file_dfy = jj + str1 + in_dfy;
        const char *remove_dfy = remove_file_dfy.c_str();
        remove(remove_dfy);
    }
    for (int j = 0; j < cont; j++) //Remove the temp files .exe generated in iteration cycles
    {
        string jj = to_string(j);
        string remove_file_exe = jj + str1 + dfy_exe;
        const char *remove_exe = remove_file_exe.c_str();
        remove(remove_exe);
    }
    for (int j = 0; j < cont; j++) //Remove the temp files .pdb generated in iteration cycles
    {
        string jj = to_string(j);
        string remove_file_pdb = jj + str1 + dfy_pdb;
        const char *remove_pdb = remove_file_pdb.c_str();
        remove(remove_pdb);
    }
    for (int j = 0; j < cont; j++) //Remove the temp files .dll generated in iteration cycles
    {
        string jj = to_string(j);
        string remove_file_dll = jj + str1 + dfy_dll;
        const char *remove_dll = remove_file_dll.c_str();
        remove(remove_dll);
    }
}

void AddComment()
{
    size_t assert_pos = str.find(str2);    //find the position of string "assert"
    size_t invariant_pos = str.find(str3); //find the position of string "invariant"
    if (assert_pos != string::npos && replaced == 0 && comment_detect(str) != 1 && r < row_num)
    {
        str.replace(assert_pos, str2.length(), "//assert"); //replace the string "assert" with "//assert"
        replaced = 1;
    }
    else if (invariant_pos != string::npos && replaced == 0 && comment_detect(str) != 1 && r < row_num)
    {
        str.replace(invariant_pos, str3.length(), "//invariant"); //replace the string "invariant" with "//invariant"
        replaced = 1;
    }
    if (replaced == 1)
    {
        r = row_num;
    }
    else
    {
        row_num++;
    }
}

void DeleteComment(string str, int row)
{
    file1.open(str);
    ofstream outfile1("delete_temp.dfy", ios::out | ios::trunc);
    if (file1.fail())
    {
        cout << "fail";
    }
    else
    {
        while (!file1.eof())
        {
            getline(file1, comment_line);
            if (cnt == row)
            {
                size_t cmt_assert_pos = comment_line.find(str4);
                size_t cmt_invariant_pos = comment_line.find(str5);
                if (cmt_assert_pos != string::npos)
                {
                    comment_line.replace(cmt_assert_pos, str4.length(), "assert");
                }
                else if (cmt_invariant_pos != string::npos)
                {
                    comment_line.replace(cmt_invariant_pos, str5.length(), "invariant");
                }
                cout << comment_line;
            }
            outfile1 << comment_line << endl;
            cnt++;
        }
        outfile1.close();
        file1.close();
        ofstream outfile2(str, ios::out | ios::trunc);
        fstream file2("delete_temp.dfy");
        while (!file2.eof())
        {
            getline(file2, comment_line);
            outfile2 << comment_line << endl;
        }
        cnt = 0;
        outfile2.close();
        file2.close();
        system("del delete_temp.dfy");
    }
}

int main()
{
    cout << "Dafny program modification" << '\n';
    cout << "Please type the file name: ";
    cin >> in_dfy;       //Type the file name to decide open which dafny file
    infile.open(in_dfy); //Open the initial test dafny file
    while (infile.fail())
    {
        cout << "Fail to open the file" << '\n';
        cout << "Please type the file name again: ";
        cin >> in_dfy; //Type the file name again until correctly open the file
        infile.open(in_dfy);
    }
    KeyWordsCount(); //Function used to count the sum of "assert" and "invariant" occurrences
    for (int i = 0; i < cont; i++)
    {
        string ii = to_string(i);
        string out_dfy = ii + str1 + in_dfy; //Name the output file of each cycle
        outfile.open(out_dfy);

        while (getline(infile, str)) //getline function get string from input file
        {
            AddComment();
            outfile << str << '\n';                              //Write strings row by row into the output file
            cout << r << "  " << row_num << "  " << str << '\n'; //Monitor //result checker in terminal
        }
        string result_path = dafny_path + out_dfy;
        const char *modified_path = result_path.c_str();
        infile.close();
        outfile.close();
        int result = system(modified_path);
        if (result != 0)
        {
            DeleteComment(out_dfy, r);
        }
        row_num = 0;
        replaced = 0;
        infile.open(out_dfy);
    }
    infile.close();
    outfile.close();
    DeleteTempFile();
    RenameFile();
    return 0;
}
