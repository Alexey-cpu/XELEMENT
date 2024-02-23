#ifndef XELEMENT_H
#define XELEMENT_H

// standart includes
#include <memory>
#include "vector"
#include "stack"
#include "queue"
#include "map"
using namespace std;

// custom includes
#include "Utils.h"
using namespace STRING_EXTENSION;

// symbol providers
class ISymbolProvider
{
public:

    // constructors
    ISymbolProvider(){}

    // virtual destructor
    virtual ~ISymbolProvider(){}

    virtual char symbol() = 0;
    virtual bool valid() = 0;
    virtual bool end() = 0;
};

class FileSymbolProvider : public ISymbolProvider
{
protected:

    ifstream m_File;

public:

    // constructors
    FileSymbolProvider( string _Path )
    {
        m_File.open( _Path );
    }

    // virtual destructor
    virtual ~FileSymbolProvider()
    {
        m_File.close();
    }

    // virtual methods override
    virtual bool valid() override
    {
        return m_File.is_open();
    }

    virtual bool end() override
    {
        return m_File.eof();
    }

    virtual char symbol() override
    {
        char output;
        m_File.get(output);
        return output;
    }
};

class StringSymbolProvider : public ISymbolProvider
{
protected:

    const string m_String  = string();
    size_t       m_Counter = 0;

public:

    // constructors
    StringSymbolProvider( string _String ) : m_String(_String){}

    // virtual destructor
    virtual ~StringSymbolProvider(){}

    // virtual methods override
    virtual bool valid() override
    {
        return !m_String.empty();
    }

    virtual bool end() override
    {
        return m_Counter >= m_String.size();
    }

    virtual char symbol() override
    {
        return m_String[m_Counter++];
    }
};

// XML object
class XElement final
{
protected:

    // info
    string                              m_Name   = std::string();
    string                              m_Value  = std::string();
    XElement*                           m_Parent = nullptr;
    std::list< shared_ptr< XElement > > m_Elements;
    mutable map< string, string >       m_Attributes;

    // constructors
    XElement(
            std::string _Name = std::string(),
            std::string _Value = std::string(),
            map< std::string, std::string > _Attributes = map< string, string >() );

    // service methods
    static bool check_symbol( char& _Input );

    static string parse_element_name(string& _Input);

    static void parse_element_attributes( XElement* _XElement, string& _Input );

public:

    // virtual destructor
    ~XElement();

    // getters
    std::string get_name() const;
    XElement* get_parent() const;

    template< typename __type >
    __type get_value( string _Name = std::string() ) const
    {
        if( _Name == std::string() || _Name == this->get_name() )
            return __from_string__< __type >( m_Value );

        shared_ptr< XElement > xelement = find_element( _Name );
        return xelement != nullptr ? __from_string__< __type >( xelement->m_Value ) : __from_string__< __type >( string() );
    }

    // setters
    void set_name( std::string _Name );

    template< typename __type >
    void set_value( __type _Value, string _Name = std::string() )
    {
        if( _Name == string() || _Name == get_name() )
        {
            m_Value = __to_string__<__type>( _Value );
        }
        else
        {
            shared_ptr< XElement > xelement = find_element( _Name );

            if( xelement != nullptr )
                xelement->set_value<__type>( _Value );
        }
    }

    // predicates
    bool has_element( string _Name )
    {
        return find_element( _Name ) != nullptr;
    }

    bool has_attribute( string _Name )
    {
        return m_Attributes.find(_Name) != m_Attributes.end();
    }

    // public methods
    void add_element( shared_ptr< XElement > _Object );
    void add_attribute( string _Name, string _Value = std::string() );
    void erase_element( std::function< bool( shared_ptr< XElement > ) > _Predicate );
    void erase_element(string _Name);
    void erase_attribute(string _Name);
    void clear();
    shared_ptr< XElement > find_element( std::function< bool(shared_ptr< XElement >) > _Predicate ) const;
    shared_ptr< XElement > find_element( string _Name ) const;
    string find_attribute( string _Name ) const;
    typeof( m_Elements.begin() ) begin();
    typeof( m_Elements.begin() ) end();
    size_t size();
    bool empty();
    string to_string(
            std::string _Prefix = std::string(),
            std::string _Postfix = std::string() ) const;

    // static API
    static shared_ptr< XElement > read( shared_ptr< ISymbolProvider > _SymbolProvider );

    static shared_ptr< XElement > from_string( string _Path );

    static shared_ptr< XElement > from_file( string _Path );

    static shared_ptr< XElement > from_file(
            string _Directory,
            string _Filename,
            string _Extention = "xml" );

    static bool to_file(
            shared_ptr< XElement > _Instance,
            string _Directory, string _Filename,
            string _Extention = "xml" );

    static shared_ptr< XElement > Create(
            std::string _Name = std::string(),
            std::string _Value = std::string(),
            map< string, string > _Attributes = map< string, string >() );
};

// XML serialization interface
class IXSerializable
{
public:

    // constructors
    IXSerializable(){}

    // virtual destructor
    virtual ~IXSerializable(){}

    virtual shared_ptr<XElement> serialize() = 0;
    virtual bool deserialize( shared_ptr<XElement> _Element ) = 0;
};

#endif // XELEMENT_H
