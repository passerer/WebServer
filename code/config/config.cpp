#include "config.h"  
  
using namespace std;  
  
  
Config::Config( string filename, string delimiter,  
               string comment )  
               : delimiter_(delimiter), comment_(comment)  
{  
    std::ifstream in( filename.c_str() );  
    if( !in ) throw File_not_found( filename );   
    in >> (*this);  
}  
  
Config::Config()  
: delimiter_( string(1,'=') ), comment_( string(1,'#') )  
{    }  
  
bool Config::KeyExists( const string& key ) const  
{  
    mapci p = contents_.find( key );  
    return ( p != contents_.end() );  
}  
  
void Config::Trim( string& inout_s )  
{  
    static const char whitespace[] = " \n\t\v\r\f";  
    inout_s.erase( 0, inout_s.find_first_not_of(whitespace) );  
    inout_s.erase( inout_s.find_last_not_of(whitespace) + 1U );  
}  
  
  
std::ostream& operator<<( std::ostream& os, const Config& cf )  
{  
    for( Config::mapci p = cf.contents_.begin();  
        p != cf.contents_.end();  
        ++p )  
    {  
        os << p->first << " " << cf.delimiter_ << " ";  
        os << p->second << std::endl;  
    }  
    return os;  
}  
  
void Config::Remove( const string& key )  
{  
    contents_.erase( contents_.find( key ) );  
    return;  
}  
  
std::istream& operator>>( std::istream& is, Config& cf )  
{  

    typedef string::size_type pos;  
    const string& delim  = cf.delimiter_;  
    const string& comm   = cf.comment_;    
    const pos skip = delim.length();      
    string nextline = "";  
    while( is || nextline.length() > 0 )  
    {  
        string line;  
        if( nextline.length() > 0 )  
        {  
            line = nextline;  
            nextline = "";  
        }  
        else  
        {  
            std::getline( is, line );  
        }  
        line = line.substr( 0, line.find(comm) );  
        pos delimPos = line.find( delim );  
        if( delimPos < string::npos )  
        {  
            string key = line.substr( 0, delimPos );  
            line.replace( 0, delimPos+skip, "" );  
            bool terminate = false;  
            while( !terminate && is )  
            {  
                std::getline( is, nextline );  
                terminate = true;  
                string nlcopy = nextline;  
                Config::Trim(nlcopy);  
                if( nlcopy == "" ) continue;  
                nextline = nextline.substr( 0, nextline.find(comm) );  
                if( nextline.find(delim) != string::npos )  
                    continue; 
                nlcopy = nextline;  
                Config::Trim(nlcopy);  
                if( nlcopy != "" ) line += "\n";  
                line += nextline;  
                terminate = false;  
            }  
            Config::Trim(key);  
            Config::Trim(line);  
            cf.contents_[key] = line;  
        }  
    } 
    return is;  
}  
bool Config::FileExist(std::string filename)  
{  
    bool exist= false;  
    std::ifstream in( filename.c_str() );  
    if( in )   
        exist = true;  
    return exist;  
}  
  
void Config::ReadFile( string filename, string delimiter,  
                      string comment )  
{  
    delimiter_ = delimiter;  
    comment_ = comment;  
    std::ifstream in( filename.c_str() ); 
    if( !in ) throw File_not_found( filename );  
    in >> (*this);  
}  
