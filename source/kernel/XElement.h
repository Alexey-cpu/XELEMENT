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
public:

    typedef typename std::list< std::shared_ptr< XElement > > elements_container;
    typedef typename std::map< std::string, std::string > attributes_container;
    typedef typename elements_container::const_iterator elements_const_iterator;

    enum FORMAT
    {
        COMPACT,
        BEAUTIFUL
    };

    // constructors
    XElement(
        std::string          _Name       = std::string(),
        std::string          _Value      = std::string(),
        attributes_container _Attributes = attributes_container(),
        std::string          _Prolog     = "<?xml version=\"1.0\"?>\n<?mso-application progid=\"Excel.Sheet\"?>\n" );

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
    elements_const_iterator begin();
    elements_const_iterator end();
    size_t size() const;
    bool empty() const;
    std::string to_string(
        std::string _Prefix  = std::string(),
        std::string _Postfix = std::string(),
        FORMAT      _Format  = FORMAT::BEAUTIFUL ) const;

    // static API
    static std::shared_ptr< XElement > read( std::shared_ptr< ISymbolProvider > _SymbolProvider );

    static std::shared_ptr< XElement > from_string( std::string& _Path );

    static std::shared_ptr< XElement > from_file( std::string _Path );

    #if defined(_GLIBCXX_FILESYSTEM) || defined(_FILESYSTEM_) // MVSC compiler compatability issues fixed

    static std::shared_ptr< XElement > from_file( std::filesystem::path _Path );

    #endif

    static bool to_file(
            std::shared_ptr< XElement > _Instance,
            std::string                 _Path,
            FORMAT                      _Format = FORMAT::BEAUTIFUL );

#if defined(_GLIBCXX_FILESYSTEM) || defined(_FILESYSTEM_) // MVSC compiler compatability issues fixed

    static bool to_file(
        std::shared_ptr< XElement > _Instance,
        std::filesystem::path       _Path,
        FORMAT                      _Format = FORMAT::BEAUTIFUL );

#endif

    static std::shared_ptr< XElement > Create(
            std::string                 _Name          = std::string(),
            std::string                 _Value         = std::string(),
            attributes_container        _Attributes    = attributes_container(),
            elements_container          _ChildElements = elements_container(),
            std::shared_ptr< XElement > _Parent        = nullptr );

    static std::shared_ptr< XElement > Clone(std::shared_ptr< XElement > _Element);

protected:

    // info
    std::string          m_Name       = std::string();
    std::string          m_Value      = std::string();
    XElement*            m_Parent     = nullptr;
    elements_container   m_Elements   = elements_container();
    attributes_container m_Attributes = attributes_container();
    std::string          m_Prolog     = std::string();

    static std::shared_ptr< XElement > find_element_recursuve(
        const XElement* _Object,
        std::function< bool( std::shared_ptr< XElement > ) > _Predicate );

    static void find_children_recursuve(
        const XElement*                                      _Object,
        std::function< bool( std::shared_ptr< XElement > ) > _Predicate,
        std::list< std::shared_ptr< XElement > >&            _Output );
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
