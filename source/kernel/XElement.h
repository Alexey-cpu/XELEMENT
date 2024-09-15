#ifndef XELEMENT_H
#define XELEMENT_H

// standart includes
#include <functional>
#include <filesystem>
#include <string>
#include <memory>
#include <map>
#include <list>

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
    std::string                              m_Name   = std::string();
    std::string                              m_Value  = std::string();
    XElement*                                m_Parent = nullptr;
    std::list< std::shared_ptr< XElement > > m_Elements;
    std::map< std::string, std::string >     m_Attributes = std::map< std::string, std::string >();
    std::string                              m_Prolog     = "<?xml version=\"1.0\"?>\n<?mso-application progid=\"Excel.Sheet\"?>\n";

    // constructors
    XElement(
            std::string _Name = std::string(),
            std::string _Value = std::string(),
            std::map<
            std::string,  // name
            std::string > // value
                        _Attributes = std::map< std::string, std::string >(),
            std::string _Prolog     = std::string() );

    static std::shared_ptr< XElement > find_element_recursuve(
        const XElement* _Object,
        std::function< bool( std::shared_ptr< XElement > ) > _Predicate );

    static void find_children_recursuve(
        const XElement*                                      _Object,
        std::function< bool( std::shared_ptr< XElement > ) > _Predicate,
        std::list< std::shared_ptr< XElement > >&            _Output );

public:

    enum FORMAT
    {
        COMPACT,
        BEAUTIFUL
    };

    // destructor
    ~XElement();

    // getters
    std::string get_name() const;
    std::string get_prolog() const;
    XElement*   get_parent() const;

    template< typename __type >
    __type get_value( std::string _Name = std::string() ) const;

    // setters
    template< typename __type >
    void set_value( __type, std::string _Value = std::string() );
    void set_name( std::string _Value);
    void set_prolog( std::string _Value);

    // predicates
    bool has_element( std::string _Name);
    bool has_attribute( std::string _Name);

    // public methods
    void add_element( std::shared_ptr< XElement > _Object );
    void add_attribute( std::string _Name, std::string _Value = std::string() );
    void erase_element( std::function< bool( std::shared_ptr< XElement > ) > );
    void erase_element(std::string _Name);
    void erase_attribute(std::string _Name);
    void clear();
    std::shared_ptr< XElement > find_element( std::function< bool(std::shared_ptr< XElement >) > _Predicate ) const;
    std::shared_ptr< XElement > find_element( std::string _Name ) const;
    std::shared_ptr< XElement > find_element_recursuve( std::string _Name ) const;
    std::shared_ptr< XElement > find_element_recursuve( std::function< bool(std::shared_ptr< XElement >) > _Predicate ) const;
    std::list< std::shared_ptr< XElement > > find_elements_recursuve( std::function< bool(std::shared_ptr< XElement >) > _Predicate ) const;
    std::string find_attribute( std::string _Name ) const;
    typeof( m_Elements.begin() ) begin();
    typeof( m_Elements.begin() ) end();
    size_t size() const;
    bool empty() const;
    std::string to_string(
        std::string _Prefix  = std::string(),
        std::string _Postfix = std::string(),
        FORMAT      _Format  = FORMAT::BEAUTIFUL ) const;

    // static API
    static std::shared_ptr< XElement > read( std::shared_ptr< ISymbolProvider > _SymbolProvider );

    static std::shared_ptr< XElement > from_string( std::string _Path );

    static std::shared_ptr< XElement > from_file( std::string _Path );

    #ifdef _GLIBCXX_FILESYSTEM

    static std::shared_ptr< XElement > from_file( std::filesystem::path _Path );

    #endif

    static bool to_file(
            std::shared_ptr< XElement > _Instance,
            std::string                 _Path,
            FORMAT                      _Format = FORMAT::BEAUTIFUL );

#ifdef _GLIBCXX_FILESYSTEM

    static bool to_file(
        std::shared_ptr< XElement > _Instance,
        std::filesystem::path       _Path,
        FORMAT                      _Format = FORMAT::BEAUTIFUL );

#endif

    static std::shared_ptr< XElement > Create(
            std::string                              _Name          = std::string(),
            std::string                              _Value         = std::string(),
            std::map< std::string, std::string >     _Attributes    = std::map< std::string, std::string >(),
            std::list< std::shared_ptr< XElement > > _ChildElements = std::list< std::shared_ptr< XElement > >(),
            std::shared_ptr< XElement >              _Parent        = nullptr );
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
