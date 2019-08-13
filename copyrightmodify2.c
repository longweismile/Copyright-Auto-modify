#include<iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include<vector>
#include<algorithm>
#include<ctime>

using namespace std;
void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);
int ModifyCopyright2(string path);
string charToStr(char * contentChar);
void modifyContentInFile(char *fileName,int lineNum,char *content);
#define ROW 20
int main(int argc,char* argv[])
{
	//string filename = "/home/weilon01/Marathon/test.txt";
	//ModifyCopyright2(filename);
	string filename = argv[1];
	string line;
	vector<string> strs;
	ifstream in(filename);
	if(in)
	{
		while (getline(in,line)) {
			strs.push_back(line);
			cout<<line<<endl;
		}
	}
	else {
		cout<<"No such file"<<endl;
	}
	for(int i = 0;i < strs.size();++i)
	{
		if(strs[i] != "")
		{
			ModifyCopyright2(strs[i]);
		}
	}
	return 0;
}

int ModifyCopyright2(string path)
{
	ifstream read_line;
	string line;
	string str;
	read_line.open(path,ios::binary);
	for(int i = 0;i < ROW;++i)
	{
		if(getline(read_line,line))
		  str = str + line + '\n';
		else {
			break;
		}
	}
	vector<string> lines;
	string c = "\n";
	SplitString(str,lines,c);
	int modifyline = 0;//the line need to change
	for(;modifyline < ROW;++modifyline)
	{
		if(lines[modifyline].find("COPYRIGHT") != lines[modifyline].npos)
		{
			break;
		}
	}
	if(modifyline == ROW)
	{
		cout<<"Do not need to modify"<<endl;
		return 0;
	}
	vector<string> strs;
	vector<string> modifystrs;
	c = " ";
	SplitString(lines[modifyline],strs,c);

	int Insertpos = 0;//position need to insert
	for(int i = 0;i < strs.size();++i)
	{
		if(strs[i] == "COPYRIGHT")
		{
			++i;
			Insertpos = i;
			vector<string>::iterator it = strs.begin() + i;
			while(*it != "ARM")
			{
				modifystrs.push_back(*it);
				it = strs.erase(it);
			}
			break;
		}
	}
	string oldcopyright = "";
	for(int i = 0;i < modifystrs.size();++i)
	{
		oldcopyright += modifystrs[i];
	}
	modifystrs.clear();
	for(int i = 0;i < oldcopyright.size();)
	{
		int pos = i;
		while(oldcopyright[pos] != ','&&oldcopyright[pos] != '-'&&pos < oldcopyright.size())
		{
			pos++;
		}
		string temp = oldcopyright.substr(i,pos-i);
		modifystrs.push_back(temp);
		if(pos != oldcopyright.size())
		{
			temp = oldcopyright.substr(pos,1);
			modifystrs.push_back(temp);
		}
		i = pos+1;
	}
	//modify date
	string preyearstr = modifystrs.back();
	int preyear = atoi(preyearstr.c_str());
	//get current date
	time_t now = time(0);
	tm *ltm = localtime(&now);
	int currentyear = ltm->tm_year + 1900;
	//modify year
	if(preyear == currentyear)
	{
		return 0;
	}
	else if(preyear == currentyear - 1)
	{
		int k = modifystrs.size() - 2;
		if(k > 0)
		{
			if(modifystrs[k] == ",")
			{
				modifystrs[k+1] = to_string(preyear) + "-" + to_string(currentyear);
			}
			if(modifystrs[k] == "-")
			{
				modifystrs[k+1] = to_string(currentyear);
			}
		}
		else {
			modifystrs[0] = to_string(preyear) + "-" + to_string(currentyear);
		}
	}
	else {
		string addstr = "," + to_string(currentyear);
		modifystrs.push_back(addstr);
	}
	string currentcopyright = "";
	for(int i = 0;i < modifystrs.size();++i)
	{
		currentcopyright += modifystrs[i];
	}
	for(int i = 0;i < currentcopyright.size();++i)
	{
		if(i < currentcopyright.size()&&currentcopyright[i] == ',')
		  currentcopyright.insert(i+1,1,' ');
	}
	strs.insert(strs.begin() + Insertpos,currentcopyright);
	string res = "";
	for(int i = 0;i < strs.size();++i)
	{
		res = res + strs[i] + " ";
	}
	modifyContentInFile((char*)(path.data()),modifyline+1,(char*)(res.data()));
	return 0;
}


string charToStr(char * contentChar)
{
	string tempStr;
	for (int i=0;contentChar[i]!='\0';i++)
	{
		tempStr+=contentChar[i];
	}
	return tempStr;
}

void modifyContentInFile(char *fileName,int lineNum,char *content)
{
	ifstream in;
	char line[1024]={'\0'};
	in.open(fileName);
	int i=0;
	string tempStr;
	while(in.getline(line,sizeof(line)))
	{
		i++;
		if(lineNum!=i)
		{
			tempStr+=charToStr(line);
		}
		else
		{
			tempStr+=charToStr(content);
		}
		tempStr+='\n';
	}
	in.close();
	ofstream out;
	out.open(fileName);
	out.flush();
	out<<tempStr;
	out.close();
}

void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while(std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2-pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if(pos1 != s.length())
	  v.push_back(s.substr(pos1));
}
