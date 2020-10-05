#ifndef CONFIG_H
#define CONFIG_H

#include <string>  
#include <map>  
#include <iostream>  
#include <fstream>  
#include <sstream>  
  

class Config {  

protected:  
    std::string delimiter_;  
    std::string comment_;    
    std::map<std::string,std::string> contents_;  
  
    typedef std::map<std::string,std::string>::iterator mapi;  
    typedef std::map<std::string,std::string>::const_iterator mapci;  
    // Methods  
public:  
  
    Config( std::string filename,std::string delimiter = "=",std::string comment = "#" );  
    Config();  
    template<class T> T Read( const std::string& in_key ) const; 
    template<class T> T Read( const std::string& in_key, const T& in_value ) const;  
    template<class T> bool ReadInto( T& out_var, const std::string& in_key ) const;  
    template<class T>  
    bool ReadInto( T& out_var, const std::string& in_key, const T& in_value ) const;  
    bool FileExist(std::string filename);  
    void ReadFile(std::string filename,std::string delimiter = "=",std::string comment = "#" );  
  
    bool KeyExists( const std::string& in_key ) const;  
  
    template<class T> void Add( const std::string& in_key, const T& in_value );  
    void Remove( const std::string& in_key );  
  
    std::string GetDelimiter() const { return delimiter_; }  
    std::string GetComment() const { return comment_; }  
    std::string SetDelimiter( const std::string& in_s )  
    { std::string old = delimiter_;  delimiter_ = in_s;  return old; }    
    std::string SetComment( const std::string& in_s )  
    { std::string old = comment_;  comment_ =  in_s;  return old; }  
  
    friend std::ostream& operator<<( std::ostream& os, const Config& cf );  
    friend std::istream& operator>>( std::istream& is, Config& cf );  
  
protected:  
    template<class T> static std::string T_as_string( const T& t );  
    template<class T> static T string_as_T( const std::string& s );  
    static void Trim( std::string& inout_s );  

public:  
    struct File_not_found {  
        std::string filename;  
        File_not_found( const std::string& filename_ = std::string() )  
            : filename(filename_) {} };  

    struct Key_not_found { 
        std::string key;  
        Key_not_found( const std::string& key_ = std::string() )  
            : key(key_) {} };  
};  
  
  
template<class T>  
std::string Config::T_as_string( const T& t )  
{  
    std::ostringstream ost;  
    ost << t;  
    return ost.str();  
}  
  
template<class T>  
T Config::string_as_T( const std::string& s )  
{  
    T t;  
    std::istringstream ist(s);  
    ist >> t;  
    return t;  
}  
  
template<>  
inline std::string Config::string_as_T<std::string>( const std::string& s )  
{  
    return s;  
}  
  
template<>  
inline bool Config::string_as_T<bool>( const std::string& s )  
{  
    bool b = true;  
    std::string sup = s;  
    for( std::string::iterator p = sup.begin(); p != sup.end(); ++p )  
        *p = toupper(*p);  // make string all caps  
    if( sup==std::string("FALSE") || sup==std::string("F") ||  
        sup==std::string("NO") || sup==std::string("N") ||  
        sup==std::string("0") || sup==std::string("NONE") )  
        b = false;  
    return b;  
}  
  
template<class T>  
T Config::Read( const std::string& key ) const  
{  
    mapci p = contents_.find(key);  
    if( p == contents_.end() ) throw Key_not_found(key);  
    return string_as_T<T>( p->second );  
}  
  
template<class T>  
T Config::Read( const std::string& key, const T& value ) const  
{ 
    mapci p = contents_.find(key);  
    if( p == contents_.end() ) return value;  
    return string_as_T<T>( p->second );  
}  
  
template<class T>  
bool Config::ReadInto( T& var, const std::string& key ) const  
{  
    mapci p = contents_.find(key);  
    bool found = ( p != contents_.end() );  
    if( found ) var = string_as_T<T>( p->second );  
    return found;  
}  
  
template<class T>  
bool Config::ReadInto( T& var, const std::string& key, const T& value ) const  
{  
    mapci p = contents_.find(key);  
    bool found = ( p != contents_.end() );  
    if( found )  
        var = string_as_T<T>( p->second );  
    else  
        var = value;  
    return found;  
}  
  
template<class T>  
void Config::Add( const std::string& in_key, const T& value )  
{  
    std::string v = T_as_string( value );  
    std::string key=in_key;  
//    trim(key);  
//    trim(v);  
    contents_[key] = v;  
    return;  
}  

#endif // CONFIG_H