#include "..\ZipFile.h"

void main()
{
	ZipFile zf( "M:\\music\\albumˆ³k\\iBoAjLISTEN TO MY HEART.zip.mp3") ;
	zf.ReadHeader() ;
	File* pf = zf.GetChildFile( 0) ;
	pf = zf.GetFileInfo( 0) ;
	pf->GetPlayLength() ;
}
