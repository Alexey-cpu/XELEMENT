#include "XElement.h"

//------------------------------------------------------------------------------------------------------------------------------------------------
// XELEMENT
//------------------------------------------------------------------------------------------------------------------------------------------------
XElement::XElement( std::string _Name, std::string _Value, map< std::string, std::string > _Attributes ) :
    m_Name( _Name ),
    m_Value(_Value),
    m_Attributes(_Attributes){}

XElement::~XElement()
{
    for( auto i = m_Elements.begin() ; i != m_Elements.end() ; i++ )
    {
        shared_ptr<XElement> child = dynamic_pointer_cast<XElement>( *i );

        if( child != nullptr )
            child->m_Parent = nullptr;
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

void XElement::set_name( std::string _Name )
{
    m_Name = _Name;
}

void XElement::add_element( shared_ptr< XElement > _Object )
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

void XElement::erase_element( std::function< bool(shared_ptr< XElement >) > _Predicate )
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

void XElement::erase_element( string _Name )
{
    this->erase_element
            (
                [&_Name](shared_ptr< XElement > _Element)
                {
                    return _Element != nullptr && _Element->get_name() == _Name;
                }
            );
}

void XElement::add_attribute( string _Name, string _Value )
{
    m_Attributes[_Name] = _Value;
}

void XElement::erase_attribute(string _Name)
{
    if( m_Attributes.find(_Name) == m_Attributes.end() )
        return;

    m_Attributes.erase(_Name);
}

void XElement::clear()
{
    m_Elements.clear();
}

shared_ptr< XElement > XElement::find_element( std::function< bool(shared_ptr< XElement >) > _Predicate ) const
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

    return iterator != m_Elements.end() ? dynamic_pointer_cast<XElement>( *iterator ) : nullptr;
}

shared_ptr< XElement > XElement::find_element( string _Name ) const
{
    return
    this->find_element
            (
                [&_Name](shared_ptr< XElement > _Element)
                {
                    return _Element != nullptr && _Element->get_name() == _Name;
                }
            );
}

string XElement::find_attribute( string _Name ) const
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

shared_ptr< XElement > XElement::Create( string _Name, string _Value, map< string, string > _Attributes )
{
    return shared_ptr< XElement >( new XElement( _Name, _Value, _Attributes ) );
}

string XElement::to_string( string _Prefix, string _Postfix ) const
{
    // serialize attributes and value
    string output = _Prefix + "<" + get_name();

    for( auto i = m_Attributes.begin() ; i != m_Attributes.end() ; i++ )
    {
        string name = ( i->second[0] == '"' && i->second[ i->second.size()-1 ] == '"' ) ? i->second : "\"" + i->second + "\"";
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
        shared_ptr< XElement > type = dynamic_pointer_cast<XElement>( *i );

        if( !type.get() )
            continue;

        output.append( type->to_string( _Prefix + "\t",  "\n" ) );
    }

    return output.append( _Prefix + "</" + get_name() + ">" + _Postfix );
}

bool XElement::check_symbol( char& _Input )
{
    return _Input != '<' && _Input != '>' && _Input != '\n' && _Input != '\t';
}

string XElement::parse_element_name(string& _Input)
{
    string name = string();

    for( size_t i = 0 ; i < _Input.size() ; i++ )
    {
        if( _Input[i] != ' ' && check_symbol( _Input[i] ) )
        {
            name += _Input[i];
        }

        if( _Input[i] == ' ' )
        {
            return name;
        }
    }

    return name;
}

void XElement::parse_element_attributes( XElement* _XElement, string& _Input )
{
    if( _XElement == nullptr )
        return;

    string attribute;
    string name;
    string value;
    name.reserve(256);
    value.reserve(256);
    attribute.reserve(256);
    int counter  = 0;
    bool trigger = false;

    for( size_t i = 0 ; i < _Input.size() ; i++ )
    {
        if( trigger && _Input[i] != '<' && _Input[i] != '>' && _Input[i] != '/' && _Input[i] != '"' )
        {
            attribute += _Input[i];
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
            continue;

        // get ready to parse attribute
        size_t j = 0;

        // parse attribute name
        for( j = 0 ; j < attribute.size() && attribute[j] != '=' ; j++ )
        {
            if( attribute[j] != ' ' )
                name.push_back( attribute[j] );
        }

        // parse attribute value
        j++;
        for( ; j < attribute.size() ; j++ )
            value.push_back( attribute[j] );

        _XElement->add_attribute( name, value );

        // clear
        name.clear();
        value.clear();
        attribute.clear();
        counter = 0;
        trigger = false;
    }
}

bool XElement::to_file( shared_ptr< XElement > _Instance, string _Directory, string _Filename, string _Extention )
{
    if(_Instance == nullptr)
        return false;

    // open file
    ofstream file;
    file.open( _Directory + "/" + _Filename + '.' + _Extention );

    if( !file )
        return false;

    file.clear();
    file << _Instance->to_string();
    file.close();

    return true;
}

shared_ptr< XElement > XElement::parse( shared_ptr< ISymbolProvider > _SymbolProvider )
{
    if( _SymbolProvider == nullptr || !_SymbolProvider->valid() )
        return nullptr;

    // auxiliary variables
    char   input;
    string name               = string();
    string tag                = string();
    string value              = string();
    int counter               = 0;
    bool trigger              = false;
    shared_ptr<XElement> root = XElement::Create( string() );
    XElement* instance        = nullptr;

    // read file
    while ( true )
    {
        input = _SymbolProvider->symbol();

        if( _SymbolProvider->end() )
            break;

        if( input == '<' ) // start parsing tag
        {
            counter++;

            if( instance != nullptr ) // setup value
                instance->set_value( value );
        }

        if( input == '>' ) // start parsing value
        {
            counter--;

            if( trigger == true )
            {
                if( instance != nullptr && instance->get_parent() != nullptr )
                {
                    instance = instance->get_parent();
                }

                trigger = false;
            }
            else
            {
                // parse tag
                name = parse_element_name( tag );

                // process instance
                if( instance == nullptr )
                {
                    instance = root.get();
                    instance->set_name( name );
                    instance->set_value( value );

                    parse_element_attributes( instance, tag );
                }
                else
                {
                    shared_ptr< XElement > xelement = XElement::Create( name, value );
                    instance->add_element( xelement );
                    instance = xelement.get();

                    parse_element_attributes( instance, tag );
                }
            }

            tag   = string();
            value = string();
        }

        if( input == '/' )
        {
            trigger = true;
        }

        if( counter > 0 && check_symbol( input ) )
        {
            tag += input;
        }

        if( counter == 0 && check_symbol( input ) )
        {
            value += input;
        }
    }

    return root;
}

shared_ptr< XElement > XElement::from_file( string _Path )
{
    return parse( shared_ptr< ISymbolProvider >( new FileSymbolProvider( _Path ) ) );
}

shared_ptr< XElement > XElement::from_file( string _Directory, string _Filename, string _Extention )
{
    return from_file( _Directory + "/" + _Filename + '.' + _Extention );
}

shared_ptr< XElement > XElement::from_string( string _String )
{
    return parse( shared_ptr< ISymbolProvider >( new StringSymbolProvider( _String ) ) );
}


