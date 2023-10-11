#ifndef JSON_H
#define JSON_H

/*
#include "kernel.h"

// JSON type wrapper
template< typename __type >
class JTypeWrapper : public SerializableObject
{
protected:
    __type m_Data;
    typedef __type WrappedType;

    // constructors
    JTypeWrapper(WrappedType _Data) : SerializableObject(), m_Data(_Data){}
    JTypeWrapper(string _Name, WrappedType _Data) : SerializableObject(_Name), m_Data(_Data){}

public:
    virtual ~JTypeWrapper(){}

    // public properties
    __type Data() const
    {
        return m_Data;
    }

    void Data( __type _Data )
    {
        m_Data = _Data;
    }

    // virtual properties override
    const std::type_info& TypeInfo() const override
    {
        return typeid (m_Data);
    }
};

// JSON composite object
class JCompositeObject : public SerializableObject
{
protected:

    // constructors
    JCompositeObject() : SerializableObject(){}
    JCompositeObject( string _Name ) : SerializableObject(_Name){}

public:

    // virtual destructor
    virtual ~JCompositeObject(){}

    // virtual methods
    virtual void add( shared_ptr< SerializableObject > _Object ) = 0;
    virtual void add( vector< shared_ptr< SerializableObject > > _Objects ) = 0;
    virtual size_t size() = 0;
};

// JBool
class JBool : public JTypeWrapper< bool >
{
protected:
    JBool(WrappedType _Data);
    JBool(string _Name, WrappedType _Data);

public:
    virtual ~JBool();

    // virtual methods override
    string to_string( string _Prefix = "", string _Postfix = ""  ) const override;

    // static API
    static shared_ptr< JBool > Create(WrappedType _Data);
    static shared_ptr< JBool > Create(string _Name, WrappedType _Data);
};

// JNull
class JNull : public JTypeWrapper< nullptr_t >
{
protected:
    JNull(WrappedType _Data);
    JNull(string _Name, WrappedType _Data);

public:
    virtual ~JNull();

    // virtual methods override
    string to_string( string _Prefix = "", string _Postfix = ""  ) const override;

    // static API
    static shared_ptr< JNull > Create(WrappedType _Data);
    static shared_ptr< JNull > Create(string _Name, WrappedType _Data);
};

// JNumber
class JNumber : public JTypeWrapper< long double >
{
protected:
    JNumber(WrappedType _Data);
    JNumber(string _Name, WrappedType _Data);

public:
    virtual ~JNumber();

    // virtual methods override
    string to_string( string _Prefix = "", string _Postfix = ""  ) const override;

    // static API
    static shared_ptr< JNumber > Create(WrappedType _Data);
    static shared_ptr< JNumber > Create(string _Name, WrappedType _Data);
};

// JString
class JString : public JTypeWrapper< string >
{
protected:
    JString(WrappedType _Data);
    JString(string _Name, WrappedType _Data);

public:
    virtual ~JString();

    // virtual methods override
    string to_string( string _Prefix = "", string _Postfix = ""  ) const override;

    // static API
    static shared_ptr< JString > Create(WrappedType _Data);
    static shared_ptr< JString > Create(string _Name, WrappedType _Data);
};

// JObject
class JObject final : public JCompositeObject
{
protected:
    std::map< string, shared_ptr< SerializableObject > > m_Data;

    // constructors
    JObject( string _Name );
    JObject( string _Name, shared_ptr< SerializableObject > _Object );
    JObject( string _Name, vector< shared_ptr< SerializableObject > > _Objects );

public:

    virtual ~JObject();

    // public methods
    void erase( string _Name );

    shared_ptr< SerializableObject > find( string _Name );

    typeof( m_Data.begin() ) begin();

    typeof( m_Data.begin() ) end();

    // static API
    static shared_ptr< JObject > Create( string _Name );
    static shared_ptr< JObject > Create( string _Name, shared_ptr< SerializableObject > _Object );
    static shared_ptr< JObject > Create( string _Name, vector< shared_ptr< SerializableObject > > _Objects );

    // virtual methods override
    void add( shared_ptr< SerializableObject > _Object ) override;
    void add( vector< shared_ptr< SerializableObject > > _Objects ) override;
    size_t size() override;
    string to_string(string _Prefix = "", string _Postfix = "") const override;
};

// JArray
class JArray final : public JCompositeObject
{
protected:

    //     std::map< unsigned int, shared_ptr< SerializableObject > >
    DynamicArray< shared_ptr< SerializableObject > > m_Data;

    JArray();
    JArray( string _Name );

public:

    virtual ~JArray();

    // public methods
    void erase( size_t _Index );

    shared_ptr< SerializableObject > find( size_t _Index );

    typeof( m_Data.begin() ) begin();
    typeof( m_Data.begin() ) end();

    // static API
    static shared_ptr< JArray > Create();
    static shared_ptr< JArray > Create( string _Name );
    static shared_ptr< JArray > Create( string _Name, vector< shared_ptr<SerializableObject> > _Objects );

    // virtual methods override
    void add( shared_ptr<SerializableObject> _Object ) override;
    void add( vector< shared_ptr<SerializableObject> > _Objects ) override;
    size_t size() override;
    string to_string(string _Prefix = "", string _Postfix = "") const override;

    typeof( m_Data.begin() ) begin() const
    {
        return m_Data.begin();
    }

    typeof( m_Data.end() ) end() const
    {
        return m_Data.end();
    }
};

// JSerializer
class JSerializer : public Serializer
{
protected:
    string m_Data;
    stack< pair< char, shared_ptr< SerializableObject > > > m_Objects;
    shared_ptr< JCompositeObject > m_Instance = nullptr;

    // serive methods
    shared_ptr< SerializableObject > create_instance( string _Name, string _Value );

    void read_symbol( char _Symbol );

public:

    shared_ptr< JCompositeObject > Instance();

    virtual shared_ptr< SerializableObject > read( string _Path ) override;

    shared_ptr< SerializableObject > read( string _Directory, string _Filename) override;

    bool write( shared_ptr< SerializableObject > _Instance, string _Directory, string _Filename ) override;
};
*/
#endif // JSON_H
