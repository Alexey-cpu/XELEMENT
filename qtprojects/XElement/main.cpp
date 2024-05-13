// backend
#include "XElement.h"

// Qt
#include <QDebug>

int main()
{
    std::string directory = "C:/Qt_projects/XELEMENT/tests";
    std::string filename  = "MSExcel";

    std::cout << "directory: " << directory << "\n";
    std::cout << "filename : " << filename  << "\n";

    std::shared_ptr<XElement> xelement = XElement::from_file( directory, filename );

    if( xelement != nullptr )
    {
        std::cout << "document has been read !!! \n\n";
        std::cout << xelement->to_string() << "\n";
    }

    XElement::to_file( xelement, directory, "MSExcelCopy" );

    //std::cout << "<?xml version=\"1.0\"?>\n<?mso-application progid=\"Excel.Sheet\"?>\n";

    return 0;
}
