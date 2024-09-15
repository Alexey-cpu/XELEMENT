#ifndef XEXAMPLES_H
#define XEXAMPLES_H

#include "XElement.h"
#include <iostream>

int xelement_example_0()
{
    // This example shows how to create and manipulate XElement object

    // read XML file
    std::shared_ptr<XElement> root = XElement::Create("ROOT");

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
                              {"DESCRIPTION", "THIS IS CHILD-1-1"}
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
        std::filesystem::path( _Path + "/" + std::filesystem::path( _Path + "/" + _Filename ).stem().string() + "_copy" + ".xml" ) );

    return 0;
}


#endif // XEXAMPLES_H
