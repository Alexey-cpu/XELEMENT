#include "json.h"

/*
// JBool
JBool::JBool(WrappedType _Data) : JTypeWrapper(_Data){}

JBool::JBool(string _Name, WrappedType _Data) : JTypeWrapper(_Name, _Data){}

JBool::~JBool(){}

string JBool::to_string( string _Prefix, string _Postfix  ) const
{
    return _Prefix + ( ( std::to_string( get_id() ) != get_name() ) ? "\""  + get_name() + "\"" + " : " : "" ) + ( m_Data ? "true" : "false" ) + _Postfix;
}

shared_ptr< JBool > JBool::Create(WrappedType _Data)
{
    return shared_ptr< JBool >( new JBool(_Data) );
}

shared_ptr< JBool > JBool::Create(string _Name, WrappedType _Data)
{
    return shared_ptr< JBool >( new JBool(_Name, _Data) );
}

// JNull
JNull::JNull(WrappedType _Data) : JTypeWrapper(_Data){}

JNull::JNull(string _Name, WrappedType _Data) : JTypeWrapper(_Name, _Data){}

JNull::~JNull(){}

string JNull::to_string( string _Prefix, string _Postfix ) const
{
    return _Prefix + ( ( std::to_string( get_id() ) != get_name() ) ? "\""  + get_name() + "\"" + " : " : "" ) + "null" + _Postfix;
}

shared_ptr< JNull > JNull::Create(WrappedType _Data)
{
    return shared_ptr< JNull >( new JNull(_Data) );
}

shared_ptr< JNull > JNull::Create(string _Name, WrappedType _Data)
{
    return shared_ptr< JNull >( new JNull(_Name, _Data) );
}

// JNumber
JNumber::JNumber(WrappedType _Data) : JTypeWrapper(_Data){}

JNumber::JNumber(string _Name, WrappedType _Data) : JTypeWrapper(_Name, _Data){}

JNumber::~JNumber(){}

string JNumber::to_string( string _Prefix, string _Postfix  ) const
{
    return _Prefix + ( ( std::to_string( get_id() ) != get_name() ) ? "\""  + get_name() + "\"" + " : " : "" ) + std::to_string(m_Data) + _Postfix;
}

shared_ptr< JNumber > JNumber::Create(WrappedType _Data)
{
    return shared_ptr< JNumber >( new JNumber(_Data) );
}

shared_ptr< JNumber > JNumber::Create(string _Name, WrappedType _Data)
{
    return shared_ptr< JNumber >( new JNumber(_Name, _Data) );
}

// JString
JString::JString(WrappedType _Data) : JTypeWrapper(_Data){}

JString::JString(string _Name, WrappedType _Data) : JTypeWrapper(_Name, _Data){}

JString::~JString(){}

string JString::to_string( string _Prefix, string _Postfix  ) const
{
    return _Prefix + ( ( std::to_string( get_id() ) != get_name() ) ? "\""  + get_name() + "\"" + " : " : "" ) + "\""  + m_Data + "\"" + _Postfix;
}

shared_ptr< JString > JString::Create(WrappedType _Data)
{
    return shared_ptr< JString >( new JString(_Data) );
}

shared_ptr< JString > JString::Create(string _Name, WrappedType _Data)
{
    return shared_ptr< JString >( new JString(_Name, _Data) );
}

// JObject
JObject::JObject( string _Name ) : JCompositeObject( _Name ){}

JObject::JObject( string _Name, shared_ptr< SerializableObject > _Object ) : JCompositeObject( _Name )
{
    add( _Object );
}

JObject::JObject( string _Name, vector< shared_ptr< SerializableObject > > _Objects ) : JCompositeObject( _Name )
{
    add( _Objects );
}

JObject::~JObject(){}

void JObject::erase( string _Name )
{
    m_Data.erase( _Name );
}

shared_ptr< SerializableObject > JObject::find( string _Name )
{
    return m_Data.at( _Name );
}

typeof( JObject::m_Data.begin() ) JObject::begin()
{
    return m_Data.begin();
}

typeof( JObject::m_Data.begin() ) JObject::end()
{
    return m_Data.end();
}

// static API
shared_ptr< JObject > JObject::Create( string _Name )
{
    return shared_ptr< JObject >( new JObject( _Name ) );
}

shared_ptr< JObject > JObject::Create( string _Name, shared_ptr< SerializableObject > _Object )
{
    return shared_ptr< JObject >( new JObject( _Name, _Object ) );
}

shared_ptr< JObject > JObject::Create( string _Name, vector< shared_ptr< SerializableObject > > _Objects )
{
    return shared_ptr< JObject >( new JObject( _Name, _Objects ) );
}

// virtual methods override
void JObject::add( shared_ptr< SerializableObject > _Object )
{
    if( !_Object.get() || _Object->get_id() == get_id() )
        return;

    _Object->set_parent(this);

    if( m_Data.find( _Object->get_name() ) != m_Data.end() )
        return;

    m_Data.insert( { _Object->get_name(), _Object } );
}

void JObject::add( vector< shared_ptr< SerializableObject > > _Objects )
{
    for( auto i = _Objects.begin() ; i != _Objects.end() ; i++ )
        add(*i);
}

size_t JObject::size()
{
    return m_Data.size();
}

string JObject::to_string(string _Prefix, string _Postfix) const
{
    string output = _Prefix;

    if( has_parent() )
        output.append( ( ( std::to_string( get_id() ) != get_name() ) ? "\""  + get_name() + "\"" + " : " : "" ) + "\n" );

    output.append( _Prefix + "{\n" );

    for( auto i = m_Data.begin() ; i != m_Data.end() ; i++ )
    {
        shared_ptr< SerializableObject > type = dynamic_pointer_cast<SerializableObject>(i->second);

        if( !type.get() )
            continue;

        output.append( type->to_string( _Prefix + "\t",  i == prev( m_Data.end() ) ? "\n" : ",\n"  ) );
    }

    return output.append( _Prefix + "}" + _Postfix);
}

// JArray
JArray::JArray() : JCompositeObject(){}

JArray::JArray( string _Name ) : JCompositeObject(_Name){}

JArray::~JArray(){}

void JArray::erase( size_t _Index )
{
    m_Data.erase( _Index );
}

shared_ptr< SerializableObject > JArray::find( size_t _Index )
{
    return m_Data.exists(_Index) ? *m_Data.find( _Index ) : nullptr;
}

typeof( JArray::m_Data.begin() ) JArray::begin()
{
    return m_Data.begin();
}

typeof( JArray::m_Data.begin() ) JArray::end()
{
    return m_Data.end();
}

shared_ptr< JArray > JArray::Create()
{
    return shared_ptr< JArray >( new JArray() );
}

shared_ptr< JArray > JArray::Create( string _Name )
{
    return shared_ptr< JArray >( new JArray(_Name) );
}

shared_ptr< JArray > JArray::Create( string _Name, vector< shared_ptr<SerializableObject> > _Objects )
{
    shared_ptr< JArray > array = shared_ptr< JArray >( new JArray(_Name) );
    array->add(_Objects);
    return array;
}

void JArray::add( shared_ptr<SerializableObject> _Object )
{
    if( !_Object.get() || _Object->get_id() == get_id() )
        return;

    _Object->set_parent(this);

    if( m_Data.find( m_Data.size() ) != m_Data.end() )
        return;

    m_Data.insert( _Object );
}

void JArray::add( vector< shared_ptr<SerializableObject> > _Objects )
{
    for( size_t i = 0 ; i < _Objects.size() ; i++ )
        add( _Objects[i] );
}

size_t JArray::size()
{
    return m_Data.size();
}

string JArray::to_string(string _Prefix, string _Postfix) const
{
    string output = _Prefix;

    if( has_parent() )
        output.append( ( ( std::to_string( get_id() ) != get_name() ) ? "\""  + get_name() + "\"" + " : " : "" ) );

    output.append( "\n" + _Prefix + "[\n" );

    for( auto i = m_Data.begin() ; i != m_Data.end() ; i++ )
    {
        shared_ptr< SerializableObject > type = dynamic_pointer_cast<SerializableObject>(*i);

        if( !type.get() )
            continue;

        string name = type->get_name();
        type->set_name("");
        output.append( type->to_string( _Prefix + "\t", i == prev( m_Data.end() ) ? "\n" : ",\n" ) );
        type->set_name(name);
    }

    return output.append( _Prefix + "]" + _Postfix);
}

// JSerializer
shared_ptr< SerializableObject > JSerializer::create_instance( string _Name, string _Value )
{
    string name = __remove_symbol__( _Name, '"' );

    if( __string_contains__( _Value, '"' ) )
        return JString::Create(name, _Value);

    if( _Value == "null" )
    {
        return JNull::Create(name, nullptr);
    }
    else if( _Value == "true" )
    {
        return JBool::Create(name, true);
    }
    else if( _Value == "false" )
    {
        return JBool::Create(name, false);
    }

    return dynamic_pointer_cast<SerializableObject>( JNumber::Create(name, stold(_Value) ) );
}

void JSerializer::read_symbol( char _Symbol )
{
    // reading data
    if( _Symbol != '{' && _Symbol != '}' && _Symbol != '[' && _Symbol != ']' && _Symbol != ',' )
        m_Data += _Symbol;

    // push new instance into the stack
    if( _Symbol == '{' )
    {
        m_Objects.push( pair< char , shared_ptr<SerializableObject> >( '}', JObject::Create("") ) );
    }
    else if( _Symbol == '[' )
    {
        m_Objects.push( pair< char , shared_ptr<SerializableObject> >( ']', JArray::Create("") ) );
    }

    // get current instance
    if( !m_Objects.empty() )
        m_Instance = dynamic_pointer_cast<JCompositeObject>( m_Objects.top().second );

    // initialize instance
    if( ( _Symbol == ',' || _Symbol == '{' || _Symbol == '[' || _Symbol == '}' || _Symbol == ']' ) && !m_Data.empty() )
    {
        if( m_Instance != nullptr )
        {
            auto words = __split__(m_Data, ":");

            if( !words[0].empty() && words[1].empty() )
            {
                m_Instance->set_name( __remove_symbol__( words[0], '"' ) );
            }
            else if( !words[0].empty() && !words[1].empty() )
            {
                m_Instance->add( create_instance( words[0], words[1] ) );
            }
        }

        m_Data.clear();
    }

    // finish current instance
    if( !m_Objects.empty() && m_Objects.top().first == _Symbol )
    {
        shared_ptr<JCompositeObject> instance = dynamic_pointer_cast<JCompositeObject>( m_Objects.top().second );

        m_Objects.pop();

        if( !m_Objects.empty() )
        {
            m_Instance = dynamic_pointer_cast<JCompositeObject>( m_Objects.top().second );
            m_Instance->add(instance);
        }
    }
}

shared_ptr< JCompositeObject > JSerializer::Instance()
{
    return m_Instance;
}

shared_ptr< SerializableObject > JSerializer::read( string _Path )
{
    // open file
    ifstream file;
    file.open( _Path );
    char input;

    if( !file )
        return nullptr;

    while ( true )
    {
        file >> input;

        if( file.eof() )
            break;

        read_symbol(input);
    }

    // close file
    file.close();

    return Instance();
}

shared_ptr< SerializableObject > JSerializer::read( string _Directory, string _Filename )
{
    return read( get_path( _Directory, _Filename, "json" ) );
}

bool JSerializer::write( shared_ptr< SerializableObject > _Instance, string _Directory, string _Filename )
{
    ofstream file;
    file.open( get_path( _Directory, _Filename, "json" ) );

    if( !file )
        return false;

    file << _Instance->to_string();
    file.close();

    return true;
}
*/
