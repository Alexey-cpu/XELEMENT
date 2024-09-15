# XELEMENT 1.0.0

## Description

**XELEMENT** is C++ library for manipulating with XML files.

The library is self contained and stand-alone, all you need is to simply copy the needed **.h** and **.cpp** files into your project to get started. The library is very compact has not dependencies and provides basic functions to work with XML files.

## Availability

Open command line, and clone repo using the following git commands:

```
git clone https://github.com/Alexey-cpu/XELEMENT.git
```

## Licence

Your use or distribution of **XELEMENT** or any modified version of
**XELEMENT** implies that you agree to this License.

This library is free software; you can redistribute it and/or
modify it under the terms of the **GNU General Public
License** as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but **WITHOUT ANY WARRANTY**; without even the implied warranty of
**MERCHANTABILITY** or **FITNESS FOR A PARTICULAR PURPOSE**.  See the **GNU
General Public License** for more details.

You should have received a copy of the **GNU General Public
License** along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
USA

Permission is hereby granted to use or copy this program under the
terms of the **GNU GPL**, provided that the Copyright, this License,
and the Availability of the original version is retained on all copies.
User documentation of any code that uses this code or any modified
version of this code must cite the Copyright, this License, the
Availability note, and "Used by permission." Permission to modify
the code and to distribute modified code is granted, provided the
Copyright, this License, and the Availability note are retained,
and a notice that the code was modified is included.

## Quick start in Qt

The library is provided with pre-configured Qt projects located inside **qtprojects** folder and aimed at testing the library.
As the library has the only **.h** and **.cpp** file so you can just integrate it into your project or precompile and use as a library.

## Example

The following example function how to create and manipulate XElement object:

```
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

```

More examples are located in **XELEMENT/source/example** folder.