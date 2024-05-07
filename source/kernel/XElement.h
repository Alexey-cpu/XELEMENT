#ifndef XELEMENT_H
#define XELEMENT_H

// standart includes
#include <memory>
#include <vector>
#include <stack>
#include <queue>
#include <map>

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

    std::ifstream m_File;

public:

    // constructors
    FileSymbolProvider( std::string _Path )
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

    const std::string m_String  = std::string();
    size_t m_Counter = 0;

public:

    // constructors
    StringSymbolProvider( std::string _String ) : m_String(_String){}

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
    std::string                                   m_Name   = std::string();
    std::string                                   m_Value  = std::string();
    XElement*                                     m_Parent = nullptr;
    std::list< std::shared_ptr< XElement > >      m_Elements;
    mutable std::map< std::string, std::string >  m_Attributes;

    // constructors
    XElement(
            std::string _Name = std::string(),
            std::string _Value = std::string(),
            std::map< std::string, std::string > _Attributes = std::map< std::string, std::string >() );

    // service methods
    static bool check_symbol( char& _Input );

    static std::string parse_element_name(std::string& _Input);

    static void parse_element_attributes( XElement* _XElement, std::string& _Input );

public:

    // virtual destructor
    ~XElement();

    // getters
    std::string get_name() const;
    XElement* get_parent() const;

    template< typename __type >
    __type get_value( std::string _Name = std::string() ) const
    {
        if( _Name == std::string() || _Name == this->get_name() )
            return __from_string__< __type >( m_Value );

        std::shared_ptr< XElement > xelement = find_element( _Name );
        return xelement != nullptr ? __from_string__< __type >( xelement->m_Value ) : __from_string__< __type >( std::string() );
    }

    // setters
    void set_name( std::string _Name );

    template< typename __type >
    void set_value( __type _Value, std::string _Name = std::string() )
    {
        if( _Name == std::string() || _Name == get_name() )
        {
            m_Value = __to_string__<__type>( _Value );
        }
        else
        {
            std::shared_ptr< XElement > xelement = find_element( _Name );

            if( xelement != nullptr )
                xelement->set_value<__type>( _Value );
        }
    }

    // predicates
    bool has_element( std::string _Name )
    {
        return find_element( _Name ) != nullptr;
    }

    bool has_attribute( std::string _Name )
    {
        return m_Attributes.find(_Name) != m_Attributes.end();
    }

    // public methods
    void add_element( std::shared_ptr< XElement > _Object );
    void add_attribute( std::string _Name, std::string _Value = std::string() );
    void erase_element( std::function< bool( std::shared_ptr< XElement > ) > _Predicate );
    void erase_element(std::string _Name);
    void erase_attribute(std::string _Name);
    void clear();
    std::shared_ptr< XElement > find_element( std::function< bool(std::shared_ptr< XElement >) > _Predicate ) const;
    std::shared_ptr< XElement > find_element( std::string _Name ) const;
    std::string find_attribute( std::string _Name ) const;
    typeof( m_Elements.begin() ) begin();
    typeof( m_Elements.begin() ) end();
    size_t size();
    bool empty();
    std::string to_string(
            std::string _Prefix = std::string(),
            std::string _Postfix = std::string() ) const;

    // static API
    static std::shared_ptr< XElement > read( std::shared_ptr< ISymbolProvider > _SymbolProvider );

    static std::shared_ptr< XElement > from_string( std::string _Path );

    static std::shared_ptr< XElement > from_file( std::string _Path );

    static std::shared_ptr< XElement > from_file(
            std::string _Directory,
            std::string _Filename,
            std::string _Extention = "xml" );

    static bool to_file(
            std::shared_ptr< XElement > _Instance,
            std::string _Directory,
            std::string _Filename,
            std::string _Extention = "xml",
            std::string _Prolog    = "<?xml version=\"1.0\"?>\n<?mso-application progid=\"Excel.Sheet\"?>\n" ); // default prolog is MS Office Excel compatible

    static std::shared_ptr< XElement > Create(
            std::string _Name  = std::string(),
            std::string _Value = std::string(),
            std::map< std::string, std::string > _Attributes = std::map< std::string, std::string >(),
            std::shared_ptr< XElement > _Parent = nullptr,
            std::list< std::shared_ptr< XElement > > _ChildElements = std::list< std::shared_ptr< XElement > >() );
};

// XML serialization interface
class IXSerializable
{
public:

    // constructors
    IXSerializable(){}

    // virtual destructor
    virtual ~IXSerializable(){}

    virtual std::shared_ptr<XElement> serialize() = 0;
    virtual bool deserialize( std::shared_ptr<XElement> _Element ) = 0;
};

#endif // XELEMENT_H
