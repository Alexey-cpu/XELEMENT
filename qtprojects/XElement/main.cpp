// backend
#include "XElement.h"

// Qt
#include <QDebug>

int main()
{
    std::string directory = "C:/Qt_projects/XELEMENT/tests";
    std::string filename  = "Test";

    std::cout << "directory: " << directory << "\n";
    std::cout << "filename : " << filename  << "\n";

    std::shared_ptr<XElement> xelement = XElement::from_file( directory, filename );

    if( xelement != nullptr )
    {
        std::cout << "document has been read !!! \n\n";
        std::cout << xelement->to_string();
    }

    return 0;
}
