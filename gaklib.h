/*
		Project:		
		Module:			
		Description:	
		Author:			Martin G�ckler
		Address:		Grafrather Str 8, D-81245 M�nchen
		Phone:			+49 - 89 - 65 30 95 63
		Web:			http://www.cresd.de/

		Copyright:		(c) 1988-2014 CRESD GmbH

		Sie d�rfen die Software f�r private Zwecke oder, wenn die hiermit
		erstellte Software ebenfalls kostenlos bereitgestellt wird,
		kostenlos nutzen. Eine Nutzung mit Gewinnerzielungsabsicht, eine
		anderweitige gewerbliche Nutung oder eine Weitergabe der Quellen
		ist generell untersagt und wird zivilrechtlich verfolgt.

		You may use this software for private, non-comercial projects,
		only. Any distribution of the source is NOT allowed.

		Die Software wird ohne GEW�HRLEISTUNG vertrieben. Das Risiko bei
		der Anwendung der Software liegt voll und ganz bei Ihnen. Es gibt
		auch keinerlei Garantie, da� die Software einen bestimmten Zweck
		erf�llt, jemals erf�llt hat oder irgendwann einmal erf�llen wird.

		THIS SOFTWARE IS PROVIDED BY CRESD GmbH, Germany, Munich ``AS IS''
		AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
		TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
		PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
		CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
		USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
		ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
		OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
		SUCH DAMAGE.

		F�r eine kommerzielle Lizenz wenden Sie sich bitte an

					support@cresd.de.

		For a commercial licence, please, contact

					support@cresd.de
*/

#ifndef GAK_STD_DEFS_H
#define GAK_STD_DEFS_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	pragma option -a4
#	pragma option -pc
#endif

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

namespace gak
{
#ifdef __BORLANDC__
#	pragma option -RT+
#endif
	namespace xml
	{
		class Element;
		class Document;
	}
#ifdef __BORLANDC__
#	pragma option -RT-
#endif
	namespace vcl
	{
		struct UserOrGroup;
	}
#ifdef __BORLANDC__
#	pragma option -RT.
#endif
}


using gak::Array;
using gak::STRING;

using gak::xml::Element;
using gak::xml::Document;

using gak::vcl::UserOrGroup;

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif


#endif	//  GAK_STD_DEFS_H
 