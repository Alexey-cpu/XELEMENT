// backend
#include "XElement.h"
#include <fstream>
#include <iostream>

int xelement_test_1( std::string _Path, std::string _Filename )
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

int main()
{
    std::string path = "C:/Qt_Projects/XELEMENT/tests";

    xelement_test_1(path, "Test-1.xml");

    return 0;
}
