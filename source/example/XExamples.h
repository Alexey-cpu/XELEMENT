#ifndef XEXAMPLES_H
#define XEXAMPLES_H

#include "XElement.h"
#include <iostream>

inline std::chrono::high_resolution_clock::time_point tic()
{
    return std::chrono::high_resolution_clock::now();
}

template< typename __type = std::chrono::nanoseconds >
double elapsed( std::chrono::high_resolution_clock::time_point _Then, std::chrono::high_resolution_clock::time_point _Now )
{
    return (double)std::chrono::duration_cast<__type>(_Now - _Then).count();
}

int xelement_example_0()
{
    // This example shows how to create and manipulate XElement object

    // read XML file
    std::shared_ptr<XElement> root = XElement::Create("ROOT");

    std::shared_ptr<XElement> child =
    XElement::Create(
        "MAIN", // name
        "MAIN",  // value
        { // attributes
            { "ONE"  , "1" },
            { "TWO"  , "2" },
            { "THREE", "3" }
        },
        {// child elements
            XElement::Create(
                "CHILD-1",
                "CHILD-1",
                {
                    {"DESCRIPTION", "THIS IS CHILD-1"}
                },
                {
                    XElement::Create(
                        "CHILD-1-1",
                        "CHILD-1-1",
                        {
                              {"DESCRIPTION", "THIS IS CHILD-1-1"},
                              { "ONE"  , "1" },
                        },
                        {
                        }
                    ),
                    XElement::Create(
                        "CHILD-1-2",
                        "CHILD-1-2",
                        {
                            {"DESCRIPTION", "THIS IS CHILD-1-2"}
                        },
                        {
                        }
                    ),
                    XElement::Create(
                        "CHILD-1-3",
                        "CHILD-1-3",
                        {
                              {"DESCRIPTION", "THIS IS CHILD-1-3"}
                        },
                        {
                        }
                    )
                }
            )
        },
        root
    );

    // add attributes
    root->add_attribute( "ONE", "1" );
    root->add_attribute( "TWO", "1" );
    root->add_attribute( "THREE", "1" );

    auto objects =
    root->find_elements_recursuve(
        []( std::shared_ptr<XElement> _Object )->bool
        {
            return _Object != nullptr && _Object->has_attribute( "ONE" );
        } );

    auto element =
        root->find_element_recursuve( "CHILD-1-1" );

    std::cout << "found element: \n";
    if( element != nullptr )
        std::cout << element->to_string() << "\n\n";


    std::cout << "elements having attribute ONE: \n";
    for( auto& object : objects )
        std::cout << object->to_string() << "\n\n";

    // print the object
    std::cout << root->to_string();

    return 0;
}

int xelement_example_1( std::string _Path, std::string _Filename )
{
    // read XML file
    std::shared_ptr<XElement> xelement =
        XElement::from_file( _Path + "/" + _Filename );

    // write back to file
    XElement::to_file(
        xelement,
        std::filesystem::path( _Path + "/" + std::filesystem::path( _Path + "/" + _Filename ).stem().string() + "_copy" + ".xml" ),
        XElement::FORMAT::COMPACT );

    return 0;
}

int xelement_example_2( std::string _Path, std::string _Filename )
{
    auto start = tic();

    // read XML file
    std::shared_ptr<XElement> root = XElement::Create( "XElement" );

    for( size_t i = 0 ; i < 1e5 ; i++ )
    {
        XElement::Create(
            "MAIN", // name
            "MAIN",  // value
            { // attributes
                { "ONE"  , "1" },
                { "TWO"  , "2" },
                { "THREE", "3" }
            },
            {// child elements
                XElement::Create(
                    "CHILD-1",
                    "CHILD-1",
                    {
                        {"DESCRIPTION", "THIS IS CHILD-1"}
                    },
                    {
                        XElement::Create(
                            "CHILD-1-1",
                            "CHILD-1-1",
                            {
                             {"DESCRIPTION", "THIS IS CHILD-1-1"},
                             { "ONE"  , "1" },
                             },
                            {
                            }
                            ),
                        XElement::Create(
                            "CHILD-1-2",
                            "CHILD-1-2",
                            {
                                {"DESCRIPTION", "THIS IS CHILD-1-2"}
                            },
                            {
                            }
                            ),
                        XElement::Create(
                            "CHILD-1-3",
                            "CHILD-1-3",
                            {
                                {"DESCRIPTION", "THIS IS CHILD-1-3"}
                            },
                            {
                            }
                            )
                    }
                    )
            },
            root
            );
    }

    std::cout << "file form has taken " << elapsed< std::chrono::milliseconds >( start, tic() ) << " milliseconds \n";

    start = tic();

    // write back to file
    XElement::to_file(
        root,
        std::filesystem::path( _Path + "/" + _Filename ),
        XElement::FORMAT::COMPACT );

    std::cout << "file write has taken " << elapsed< std::chrono::milliseconds >( start, tic() ) << " milliseconds \n";

    start = tic();

    std::shared_ptr<XElement> xelement =
        XElement::from_file( std::filesystem::path( _Path + "/" + _Filename ) );

    std::cout << "file read has taken " << elapsed< std::chrono::milliseconds >( start, tic() ) << " milliseconds \n";

    return 0;
}

#endif // XEXAMPLES_H
