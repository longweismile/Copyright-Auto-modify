#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;
void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);
int ModifyCopyRight(string path);
int main(int argc,char* argv[])
{
	string filename = argv[1];
	string line;
	vector<string> strs;
	ifstream in(filename);
	if(in)
	{
		while (getline(in,line)) {
			strs.push_back(line);
		}
	}
	else {
		cout<<"No such file"<<endl;
	}
	for(int i = 0;i < strs.size();++i)
	{
		if(strs[i] != "")
		{
			ModifyCopyRight(strs[i]);
		}
	}
}

int ModifyCopyRight(string path)
{
	std::ifstream t(path);
	std::string str((std::istreambuf_iterator<char>(t)),
				std::istreambuf_iterator<char>());
	vector<string> strs;
	string c = " ";
	SplitString(str,strs,c);
	vector<string> modifystrs;
	int Insertpos = 0;
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
	if(modifystrs.empty())
	{
		cout<<"Do not need to change"<<endl;
		cout<<path<<endl;
		return 0;
	}
	string oldverstr = "";
	for(int i = 0;i < modifystrs.size();++i)
	{
		oldverstr += modifystrs[i];
	}
	modifystrs.clear();
	for(int i = 0;i < oldverstr.size();)
	{
		int pos = i;
		while(oldverstr[pos] != ','&&oldverstr[pos] != '-'&&pos < oldverstr.size())
		{
			pos++;
		}
		string temp = oldverstr.substr(i,pos-i);
		modifystrs.push_back(temp);
		if(pos != oldverstr.size())
		{
			temp = oldverstr.substr(pos,1);
			modifystrs.push_back(temp);
		}
		i = pos+1;
	}
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
	string Insertstr = "";
	for(int i = 0;i < modifystrs.size();++i)
	{
		Insertstr += modifystrs[i];
	}
	for(int i = 0;i < Insertstr.size();++i)
	{
		if(Insertstr[i] == ',')
		  Insertstr.insert(i+1,1,' ');
	}
	strs.insert(strs.begin() + Insertpos,Insertstr);
	string res = "";
	for(int i = 0;i < strs.size();++i)
	  res = res + strs[i] + " ";
	ofstream out(path);
	out<<res<<endl;
	out.close();
	return 0;
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

