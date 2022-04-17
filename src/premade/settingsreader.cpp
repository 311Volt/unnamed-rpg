class ConfigFile
{
    ifstream stfile;
    bool loaded;
public:
    ConfigFile() {loaded = 0;}
    ~ConfigFile() {if(loaded) stfile.close();}
    int LoadFile(const char* filename)
    {
        stfile.open(filename);
        if(!stfile.good())
    		return 0;
		loaded = 1;
		return 1;
    }
    int GetValue(string input)
    {
        stfile.clear();
        stfile.seekg(0);
    	char temp;
    	int output;
    	string filestr = "";
    	string sname = "";
    	while(!stfile.eof())
    	{
    		stfile.read(&temp, 1);
    		filestr += temp;
    		if(filestr[filestr.size()-1] == '=')
    			filestr[filestr.size()-1] = ' ';
    	}
    	stringstream ss;
    	ss.str(filestr);
    	while(ss)
    	{
    		ss>>sname;
    		if(sname == input)
    		{
    			ss>>output;
    			return output;
    		}
    		sname = "";
    	}
    	CriticalError1337("cfg: Nie znaleziono "+input);
    }
    
    int GetSectionValue(string section, string input)
    {
        stfile.clear();
        stfile.seekg(0);
        string search_tmp;
        while(1)
        {
            getline(stfile, search_tmp);
            if(search_tmp == '['+section+']')
                break;
            if(stfile.eof())
            {
                stfile.clear();
                stfile.seekg(0);
                break;
            }
        }
    	char temp;
    	int output;
    	string filestr = "";
    	string sname = "";
    	while(!stfile.eof())
    	{
    		stfile.read(&temp, 1);
    		filestr += temp;
    		if(filestr[filestr.size()-1] == '=')
    			filestr[filestr.size()-1] = ' ';
    	}
    	stringstream ss;
    	ss.str(filestr);
    	while(ss)
    	{
    		ss>>sname;
    		if(sname == input)
    		{
    			ss>>output;
    			return output;
    		}
    		sname = "";
    	}
    	CriticalError1337("cfg: Nie znaleziono "+input);
    }
    
    string GetValueS(string input)
    {
        stfile.clear();
        stfile.seekg(0);
    	char temp;
    	string output;
    	string filestr = "";
    	string sname = "";
    	while(!stfile.eof())
    	{
    		stfile.read(&temp, 1);
    		filestr += temp;
    		if(filestr[filestr.size()-1] == '=')
    			filestr[filestr.size()-1] = ' ';
    	}
    	stringstream ss;
    	ss.str(filestr);
    	while(ss)
    	{
    		ss>>sname;
    		if(sname == input)
    		{
    			ss>>output;
    			return output;
    		}
    		sname = "";
    	}
    	CriticalError1337("cfg: Nie znaleziono "+input);
    }
    
    string GetSectionValueS(string section, string input)
    {
        stfile.clear();
        stfile.seekg(0);
        string search_tmp;
        while(1)
        {
            getline(stfile, search_tmp);
            if(search_tmp == '['+section+']')
                break;
            if(stfile.eof())
            {
                stfile.clear();
                stfile.seekg(0);
                break;
            }
        }
    	char temp;
    	string output;
    	string filestr = "";
    	string sname = "";
    	while(!stfile.eof())
    	{
    		stfile.read(&temp, 1);
    		filestr += temp;
    		if(filestr[filestr.size()-1] == '=')
    			filestr[filestr.size()-1] = ' ';
    	}
    	stringstream ss;
    	ss.str(filestr);
    	while(ss)
    	{
    		ss>>sname;
    		if(sname == input)
    		{
    			ss>>output;
    			return output;
    		}
    		sname = "";
    	}
    	CriticalError1337("cfg: Nie znaleziono "+input);
    }
};
