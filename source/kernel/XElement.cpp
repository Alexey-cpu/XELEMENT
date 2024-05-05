#include "XElement.h"

XElement::XElement( std::string _Name, std::string _Value, std::map< std::string, std::string > _Attributes ) :
    m_Name( _Name ),
    m_Value(_Value),
    m_Attributes(_Attributes){}

XElement::~XElement()
{
    for( auto i = m_Elements.begin() ; i != m_Elements.end() ; i++ )
    {
        std::shared_ptr<XElement> child = std::dynamic_pointer_cast<XElement>( *i );

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
    return _Input != '<' && _Input != '>' && _Input != '/' && _Input != '\n' && _Input != '\t';
}

std::string XElement::parse_element_name( std::string& _Input )
{
    std::string name = std::string();

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

void XElement::parse_element_attributes( XElement* _XElement, std::string& _Input )
{
    if( _XElement == nullptr )
        return;

    std::string attribute;
    std::string name;
    std::string value;
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

std::shared_ptr< XElement > XElement::read( std::shared_ptr< ISymbolProvider > _SymbolProvider )
{
    if( _SymbolProvider == nullptr || !_SymbolProvider->valid() )
        return nullptr;

    // auxiliary variables
    char   input;
    std::string name               = std::string();
    std::string tag                = std::string();
    std::string value              = std::string();
    int counter                    = 0;
    bool trigger                   = false;
    std::shared_ptr<XElement> root = XElement::Create( STRINGIFY( XElement ) );
    XElement* instance             = nullptr;

    // read file
    while ( true )
    {
        input = _SymbolProvider->symbol();

        if( _SymbolProvider->end() )
            break;

        if( input == '<' ) // start parsing tag
        {
            counter++;

            if( instance != nullptr )
            {
                instance->set_value( value );
            }
        }

        if( input == '>' ) // start parsing value
        {
            counter--;

            if( trigger == true )
            {
                // parse tag
                name = parse_element_name( tag );

                if( instance != nullptr )
                {
                    if( instance->get_parent() != nullptr )
                    {
                        if( name != instance->get_name() )
                        {
                            std::shared_ptr< XElement > xelement = XElement::Create( name, value );
                            instance->add_element( xelement );

                            // parse instance element attribute
                            parse_element_attributes( xelement.get(), tag );
                        }
                        else
                        {
                            instance = instance->get_parent();
                        }
                    }
                }
                else
                {
                    // create instance and add it to the root
                    std::shared_ptr< XElement > xelement = XElement::Create( name, value );
                    root->add_element( xelement );
                    instance = xelement.get();

                    // parse instance element attribute
                    parse_element_attributes( instance, tag );
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
                    // create instance
                    std::shared_ptr< XElement > xelement = XElement::Create( name, value );
                    root->add_element( xelement );
                    instance = xelement.get();

                    parse_element_attributes( instance, tag );
                }
                else
                {
                    std::shared_ptr< XElement > xelement = XElement::Create( name, value );
                    instance->add_element( xelement );
                    instance = xelement.get();

                    parse_element_attributes( instance, tag );
                }
            }

            tag   = std::string();
            value = std::string();
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

std::shared_ptr< XElement > XElement::from_string( std::string _String )
{
    return read( std::shared_ptr< ISymbolProvider >( new StringSymbolProvider( _String ) ) );
}

bool XElement::to_file(
        std::shared_ptr< XElement > _Instance,
        std::string _Directory,
        std::string _Filename,
        std::string _Extention )
{
    if(_Instance == nullptr)
        return false;

    // open file
    std::ofstream file;
    file.open( _Directory + "/" + _Filename + '.' + _Extention );

    if( !file )
        return false;

    file.clear();
    file << _Instance->to_string();
    file.close();

    return true;
}

std::shared_ptr< XElement > XElement::Create(
        std::string _Name,
        std::string _Value,
        std::map< std::string, std::string > _Attributes )
{
    return std::shared_ptr< XElement >( new XElement( _Name, _Value, _Attributes ) );
}
