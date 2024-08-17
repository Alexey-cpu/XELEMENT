#include "XElement.h"
#include <iostream>
#include <sstream>
#include <fstream>

#ifndef STRINGIFY
#define STRINGIFY(INPUT) #INPUT
#endif

#ifndef CONCATENATE
#define CONCATENATE(X,Y) X##_##Y
#endif

// __from_string__
template< typename __type > __type __from_string__( std::string _Input );

template<> inline float __from_string__< float >( std::string _Input )
{
    try
    {
        return std::stof( _Input );
    }
    catch(...)
    {
        return 0.0;
    }
}

template<> inline double __from_string__<double>( std::string _Input )
{
    try
    {
        return std::stod( _Input );
    }
    catch(...)
    {
        return 0.0;
    }
}

template<> inline long double __from_string__<long double>( std::string _Input )
{
    try
    {
        return std::stold( _Input );
    }
    catch(...)
    {
        return 0.0;
    }
}

template<> inline short __from_string__<short>( std::string _Input )
{
    try
    {
        return std::stoi( _Input );
    }
    catch(...)
    {
        return 0.0;
    }
}

template<> inline int __from_string__<int>( std::string _Input )
{
    try
    {
        return std::stoi( _Input );
    }
    catch(...)
    {
        return 0.0;
    }
}

template<> inline long __from_string__<long>( std::string _Input )
{
    try
    {
        return std::stol( _Input );
    }
    catch(...)
    {
        return 0.0;
    }
}

template<> inline long long __from_string__<long long>( std::string _Input )
{
    try
    {
        return std::stoll( _Input );
    }
    catch(...)
    {
        return 0.0;
    }
}

template<> inline unsigned short __from_string__<unsigned short>( std::string _Input )
{
    try
    {
        return std::stoul( _Input );
    }
    catch(...)
    {
        return 0.0;
    }
}

template<> inline unsigned int __from_string__<unsigned int>( std::string _Input )
{
    try
    {
        return std::stoul( _Input );
    }
    catch(...)
    {
        return 0.0;
    }
}

template<> inline unsigned long __from_string__<unsigned long>( std::string _Input )
{
    try
    {
        return std::stoul( _Input );
    }
    catch(...)
    {
        return 0.0;
    }
}

template<> inline unsigned long long __from_string__<unsigned long long>( std::string _Input )
{
    try
    {
        return std::stoull( _Input );
    }
    catch(...)
    {
        return 0.0;
    }
}

template<> inline bool __from_string__<bool>( std::string _Input )
{
    try
    {
        return _Input == "true" || _Input == "1" ? true : false;
    }
    catch(...)
    {
        return false;
    }
}

template<> inline std::string __from_string__< std::string >( std::string _Input )
{
    return _Input;
}

template<> inline char __from_string__<char>( std::string _Input )
{
    return _Input.empty() ? ' ' : _Input[0];
}

// __to_string__
template< typename __type > std::string __to_string__( __type _Input );

template<> inline std::string __to_string__<bool>( bool _Input )
{
    return std::to_string( _Input );
}

template<> inline std::string __to_string__< float >( float _Input )
{
    return std::to_string( _Input );
}

template<> inline std::string __to_string__<double>( double _Input )
{
    return std::to_string( _Input );
}

template<> inline std::string __to_string__<long double>( long double _Input )
{
    return std::to_string( _Input );
}

template<> inline std::string __to_string__<short>( short _Input )
{
    return std::to_string( _Input );
}

template<> inline std::string __to_string__<int>( int _Input )
{
    return std::to_string( _Input );
}

template<> inline std::string __to_string__<long>( long _Input )
{
    return std::to_string( _Input );
}

template<> inline std::string __to_string__<long long>( long long _Input )
{
    return std::to_string( _Input );
}

template<> inline std::string __to_string__<unsigned short>( unsigned short _Input )
{
    return std::to_string( _Input );
}

template<> inline std::string __to_string__<unsigned int>( unsigned int _Input )
{
    return std::to_string( _Input );
}

template<> inline std::string __to_string__<unsigned long>( unsigned long _Input )
{
    return std::to_string( _Input );
}

template<> inline std::string __to_string__<unsigned long long>( unsigned long long _Input )
{
    return std::to_string( _Input );
}

template<> inline std::string __to_string__< std::string >( std::string _Input )
{
    return _Input;
}

// FileSymbolProvider
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

// StringSymbolProvider
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

// XElement::XElementTagParser
XElement::XElementTagParser::XElementTagParser()
{
    m_Attribute.reserve(512);
    m_Value.reserve(512);
    m_Name.reserve(512);
}

XElement::XElementTagParser::~XElementTagParser(){}

std::string XElement::XElementTagParser::parse_element_name(std::string& _Input)
{
    m_Name.clear();

    for( size_t i = 0 ; i < _Input.size() ; i++ )
    {
        if( _Input[i] != ' ' && XElement::check_symbol( _Input[i] ) )
        {
            m_Name += _Input[i];
        }

        if( _Input[i] == ' ' )
        {
            return m_Name;
        }
    }

    return m_Name;
}

void XElement::XElementTagParser::parse_element_attributes( XElement* _XElement, std::string& _Input )
{
    if( _XElement == nullptr )
    {
        return;
    }

    m_Name.clear();
    m_Value.clear();
    m_Attribute.clear();
    int  counter = 0;
    bool trigger = false;

    for( size_t i = 0 ; i < _Input.size() ; i++ )
    {
        if( trigger && _Input[i] != '<' && _Input[i] != '>' && _Input[i] != '"' )
        {
            m_Attribute += _Input[i];
        }

        if( _Input[i] == ' ' || _Input[i] == '>' )
        {
            trigger = true;
        }

        if( _Input[i] == '"' )
        {
            counter++;
        }

        if(counter < 2)
        {
            continue;
        }

        // get ready to parse attribute
        size_t j = 0;

        // parse attribute name
        for( j = 0 ; j < m_Attribute.size() && m_Attribute[j] != '=' ; j++ )
        {
            if( m_Attribute[j] != ' ' )
            {
                m_Name.push_back( m_Attribute[j] );
            }
        }

        // parse attribute value
        j++;
        for( ; j < m_Attribute.size() ; j++ )
        {
            m_Value.push_back( m_Attribute[j] );
        }

        _XElement->add_attribute( m_Name, m_Value );

        // clear
        m_Name.clear();
        m_Value.clear();
        m_Attribute.clear();
        counter = 0;
        trigger = false;
    }
}

// XElement
XElement::XElement(
        std::string _Name,
        std::string _Value,
        std::map< std::string, std::string > _Attributes ) :
    m_Name( _Name ),
    m_Value(_Value),
    m_Attributes(_Attributes){}

XElement::~XElement()
{
    for( auto i = m_Elements.begin() ; i != m_Elements.end() ; i++ )
    {
        std::shared_ptr<XElement> child =
                std::dynamic_pointer_cast<XElement>( *i );

        // don't forget to detach child from it's parent !!!
        if( child != nullptr )
        {
            child->m_Parent = nullptr;
        }
    }

    m_Elements.clear();
}

std::string XElement::get_name() const
{
    return m_Name;
}

XElement* XElement::get_parent() const
{
    return m_Parent;
}

template< typename __type > __type XElement::get_value( std::string _Name ) const
{
    if( _Name == std::string() || _Name == this->get_name() )
    {
        return __from_string__< __type >( m_Value );
    }

    std::shared_ptr< XElement > xelement = find_element( _Name );

    return xelement != nullptr ?
               __from_string__< __type >( xelement->m_Value ) :
               __from_string__< __type >( std::string() );
}

template std::string XElement::get_value( std::string _Name ) const;
template short XElement::get_value( std::string _Name ) const;
template unsigned short XElement::get_value( std::string _Name ) const;
template int XElement::get_value( std::string _Name ) const;
template unsigned int XElement::get_value( std::string _Name ) const;
template long XElement::get_value( std::string _Name ) const;
template long long XElement::get_value( std::string _Name ) const;
template unsigned long XElement::get_value( std::string _Name ) const;
template unsigned long long XElement::get_value( std::string _Name ) const;
template float XElement::get_value( std::string _Name ) const;
template double XElement::get_value( std::string _Name ) const;
template bool XElement::get_value( std::string _Name ) const;

template< typename __type >
void XElement::set_value( __type _Value, std::string _Name )
{
    if( _Name == std::string() || _Name == get_name() )
    {
        m_Value = __to_string__<__type>( _Value );
    }
    else
    {
        std::shared_ptr< XElement > xelement = find_element( _Name );

        if( xelement != nullptr )
        {
            xelement->set_value<__type>( _Value );
        }
    }
}

template void XElement::set_value( std::string _Value, std::string _Name );
template void XElement::set_value( short _Value, std::string _Name );
template void XElement::set_value( unsigned short _Value, std::string _Name );
template void XElement::set_value( int _Value, std::string _Name );
template void XElement::set_value( unsigned int _Value, std::string _Name );
template void XElement::set_value( long _Value, std::string _Name );
template void XElement::set_value( long long _Value, std::string _Name );
template void XElement::set_value( unsigned long _Value, std::string _Name );
template void XElement::set_value( unsigned long long _Value, std::string _Name );
template void XElement::set_value( float _Value, std::string _Name );
template void XElement::set_value( double _Value, std::string _Name );
template void XElement::set_value( bool _Value, std::string _Name );

void XElement::set_name( std::string _Name )
{
    m_Name = _Name;
}

bool XElement::has_element( std::string _Name )
{
    return find_element( _Name ) != nullptr;
}

bool XElement::has_attribute( std::string _Name )
{
    return m_Attributes.find(_Name) != m_Attributes.end();
}

void XElement::add_element( std::shared_ptr< XElement > _Object )
{
    if( _Object == nullptr || _Object.get() == this )
        return;

    // erase object from it's parent
    if( _Object->m_Parent != nullptr )
    {
        _Object->m_Parent->erase_element( _Object->get_name() );
        _Object->m_Parent = nullptr;
    }

    // attach object to this XElement
    _Object->m_Parent = this;
    m_Elements.push_back( _Object );
}

void XElement::erase_element( std::function< bool( std::shared_ptr< XElement >) > _Predicate )
{
    if( _Predicate == nullptr )
        return;

    auto iterator =
    std::find_if
            (
                m_Elements.begin(),
                m_Elements.end(),
                _Predicate
            );

    if( iterator == m_Elements.end() )
        return;

    (*iterator)->m_Parent = nullptr;
    m_Elements.erase(iterator);
}

void XElement::erase_element( std::string _Name )
{
    this->erase_element
            (
                [&_Name]( std::shared_ptr< XElement > _Element)
                {
                    return _Element != nullptr && _Element->get_name() == _Name;
                }
            );
}

void XElement::add_attribute( std::string _Name, std::string _Value )
{
    m_Attributes[_Name] = _Value;
}

void XElement::erase_attribute( std::string _Name )
{
    if( m_Attributes.find(_Name) == m_Attributes.end() )
        return;

    m_Attributes.erase(_Name);
}

void XElement::clear()
{
    m_Elements.clear();
}

std::shared_ptr< XElement > XElement::find_element( std::function< bool( std::shared_ptr< XElement >) > _Predicate ) const
{
    if( _Predicate == nullptr )
        return nullptr;

    auto iterator =
    std::find_if
            (
                m_Elements.begin(),
                m_Elements.end(),
                _Predicate
            );

    return iterator != m_Elements.end() ? std::dynamic_pointer_cast<XElement>( *iterator ) : nullptr;
}

std::shared_ptr< XElement > XElement::find_element( std::string _Name ) const
{
    return
    this->find_element
            (
                [&_Name]( std::shared_ptr< XElement > _Element)
                {
                    return _Element != nullptr && _Element->get_name() == _Name;
                }
            );
}

std::string XElement::find_attribute( std::string _Name ) const
{
    return m_Attributes.find(_Name) == m_Attributes.end() ?
                std::string() :
                m_Attributes[ _Name ];
}

size_t XElement::size()
{
    return m_Elements.size();
}

bool XElement::empty()
{
    return m_Elements.empty();
}

typeof( XElement::m_Elements.begin() ) XElement::begin()
{
    return m_Elements.begin();
}

typeof( XElement::m_Elements.begin() ) XElement::end()
{
    return m_Elements.end();
}

std::string XElement::to_string( std::string _Prefix, std::string _Postfix ) const
{
    // serialize attributes and value
    std::string output = _Prefix + "<" + get_name();

    for( auto i = m_Attributes.begin() ; i != m_Attributes.end() ; i++ )
    {
        std::string name = ( i->second[0] == '"' && i->second[ i->second.size()-1 ] == '"' ) ? i->second : "\"" + i->second + "\"";
        output.append( " " + i->first + "=" + name + ( i == prev(m_Attributes.end()) ? "" : " " ) );
    }

    output.append(">" + m_Value);

    // serialize child elements
    if( m_Elements.size() == 0 )
    {
        return output.append( "</" + get_name() + ">" + _Postfix );
    }

    output.append("\n");

    for( auto i = m_Elements.begin() ; i != m_Elements.end() ; i++ )
    {
        std::shared_ptr< XElement > type = std::dynamic_pointer_cast<XElement>( *i );

        if( !type.get() )
            continue;

        output.append( type->to_string( _Prefix + "\t",  "\n" ) );
    }

    return output.append( _Prefix + "</" + get_name() + ">" + _Postfix );
}

bool XElement::check_symbol( char& _Input )
{
    return _Input != '<' && _Input != '>' && _Input != '/' && _Input != '?' && _Input != '\n' && _Input != '\t';
}

std::shared_ptr< XElement > XElement::read( std::shared_ptr< ISymbolProvider > _SymbolProvider )
{
    if( _SymbolProvider == nullptr ||
            !_SymbolProvider->valid() )
    {
        return nullptr;
    }

    // auxiliary variables
    char input;
    std::shared_ptr<XElement> root =
            XElement::Create( STRINGIFY( XElement ) );

    XElement* current = root.get();

    XElementTagParser parser;

    std::string tag;
    std::string value;
    tag.reserve(512);
    tag.reserve(1024);
    bool readTag   = false;
    bool readValue = false;

    // read content
    while ( true )
    {
        input = _SymbolProvider->symbol();

        if( _SymbolProvider->end() )
        {
            break;
        }

        // start read tag
        if( input == '<' )
        {
            if( current != nullptr )
            {
                if( current->get_value<std::string>() == std::string() )
                {
                    current->set_value( value );
                }
            }

            // reset control variables
            readValue = false;
            readTag   = true;
            tag       = std::string();
        }

        // start read value
        if( input == '>' )
        {
            // read last tag symbol
            tag += input;

            // parse tag
            if( tag[0] == '<' && tag[1] == '/' ) // <Data ATTRIBUTE=""></Data>
            {
                if( current != nullptr )
                {
                    current->set_name( parser.parse_element_name( tag ) );
                    parser.parse_element_attributes( current, tag );
                    current = current->get_parent();
                }
            }
            else if ( tag[ tag.size() - 2 ] == '/' &&
                      tag[ tag.size() - 1 ] == '>' ) // <Data ATTRIBUTE="" />
            {
                if( current != nullptr )
                {
                    std::shared_ptr<XElement> element =
                            XElement::Create( STRINGIFY( XElement ) );

                    element->set_name( parser.parse_element_name( tag ) );
                    parser.parse_element_attributes( element.get(), tag );
                    current->add_element( element );
                }
            }
            else if( ( tag[0] == '<' && tag[1] == '?' ) &&
                     ( tag[ tag.size() - 2 ] == '?' && tag[ tag.size() - 1 ] == '>' ) ) // <?xml version="1.0"?>
            {
                // TODO: handle prolog here !!!
            }
            else
            {
                if( current != nullptr )
                {
                    std::shared_ptr<XElement> element =
                            XElement::Create( STRINGIFY( XElement ) );

                    element->set_name( parser.parse_element_name( tag ) );
                    parser.parse_element_attributes( element.get(), tag );
                    current->add_element( element );
                    current = element.get();
                }
            }

            // reset control variables
            readValue = true;
            readTag   = false;
            value     = std::string();
        }

        if( readValue && input != '<' && input != '>' && input != '?' && input != '\n' && input != '\t' )
        {
            value += input;
        }

        if( readTag )
        {
            tag += input;
        }
    }

    return root->size() > 1 ? root : *root->begin();
}

std::shared_ptr< XElement > XElement::from_file( std::string _Path )
{
    return read( std::shared_ptr< ISymbolProvider >( new FileSymbolProvider( _Path ) ) );
}

std::shared_ptr< XElement > XElement::from_file(
        std::string _Directory,
        std::string _Filename,
        std::string _Extention )
{
    return from_file( _Directory + "/" + _Filename + '.' + _Extention );
}

#ifdef _GLIBCXX_FILESYSTEM

std::shared_ptr< XElement > XElement::from_file( std::filesystem::path _Path )
{
    return from_file( _Path.string() );
}

bool XElement::to_file(
    std::shared_ptr< XElement > _Instance,
    std::filesystem::path       _Path,
    std::string                 _Prolog )
{
    return XElement::to_file(
        _Instance,
        _Path.parent_path().string(),
        _Path.filename().string(),
        _Path.extension().string(),
        _Prolog
    );
}

#endif

std::shared_ptr< XElement > XElement::from_string( std::string _String )
{
    return read( std::shared_ptr< ISymbolProvider >( new StringSymbolProvider( _String ) ) );
}

bool XElement::to_file(
        std::shared_ptr< XElement > _Instance,
        std::string _Directory,
        std::string _Filename,
        std::string _Extention,
        std::string _Prolog )
{
    if(_Instance == nullptr)
        return false;

    // open file
    std::ofstream file;
    file.open( _Directory + "/" + _Filename + '.' + _Extention );

    if( !file )
        return false;

    file.clear();
    file << _Prolog; // write prolog first !!!
    file << _Instance->to_string();
    file.close();

    return true;
}

std::shared_ptr< XElement > XElement::Create(
        std::string _Name,
        std::string _Value,
        std::map< std::string, std::string > _Attributes,
        std::list< std::shared_ptr< XElement > > _ChildElements,
        std::shared_ptr<XElement> _Parent )
{
    std::shared_ptr< XElement > xelement =
            std::shared_ptr< XElement >( new XElement( _Name, _Value, _Attributes ) );

    if( _Parent != nullptr )
    {
        _Parent->add_element( xelement );
    }

    for( auto i = _ChildElements.begin() ; i != _ChildElements.end() ; i++ )
    {
        std::shared_ptr< XElement > child =
                std::dynamic_pointer_cast<XElement>( *i );

        if( child != nullptr )
        {
            xelement->add_element( child );
        }
    }

    return xelement;
}
