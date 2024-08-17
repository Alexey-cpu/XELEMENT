#ifndef XELEMENT_H
#define XELEMENT_H

// custom includes
#include "Utils.h"

// standart includes
#include <memory>
#include <map>
#include <list>
#include <functional>

#if __cplusplus >= 202002L
#include <filesystem>
#endif

// ISymbolProvider
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

    // nested types
    class XElementTagParser
    {
        std::string m_Attribute;
        std::string m_Name;
        std::string m_Value;

    public:
        XElementTagParser();
        ~XElementTagParser();
        std::string parse_element_name(std::string& _Input);
        void parse_element_attributes( XElement* _XElement, std::string& _Input );
    };

public:

    // destructor
    ~XElement();

    // getters
    std::string get_name() const;
    XElement* get_parent() const;

    template< typename __type >
    __type get_value( std::string _Name = std::string() ) const;

    // setters
    template< typename __type >
    void set_value( __type _Value, std::string _Name = std::string() );
    void set_name( std::string _Name );

    // predicates
    bool has_element( std::string _Name );
    bool has_attribute( std::string _Name );

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

#ifdef _GLIBCXX_FILESYSTEM

    static std::shared_ptr< XElement > from_file( std::filesystem::path _Path );

    static bool to_file(
        std::shared_ptr< XElement > _Instance,
        std::filesystem::path       _Path,
        std::string                 _Prolog = "<?xml version=\"1.0\"?>\n<?mso-application progid=\"Excel.Sheet\"?>\n" );

#endif

    static std::shared_ptr< XElement > Create(
            std::string _Name  = std::string(),
            std::string _Value = std::string(),
            std::map< std::string, std::string >     _Attributes    = std::map< std::string, std::string >(),
            std::list< std::shared_ptr< XElement > > _ChildElements = std::list< std::shared_ptr< XElement > >(),
            std::shared_ptr< XElement > _Parent = nullptr );
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
