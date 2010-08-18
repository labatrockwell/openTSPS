
namespace tpp {

/*! \file assert.hpp
        \brief Implements a better 'Assert'
 */

#ifndef REVIVER_ASSERT_HPP
#define REVIVER_ASSERT_HPP


/*! \def MyAssertFunction
    \brief Function used by 'Assert' function in _DEBUG mode.
   
    Details.
*/
//Fix for Visual Studio C++ needing full definition of MyAssertFunction
#if defined _MSC_VER
	extern bool MyAssertFunction( bool b, char* desc, int line, char* file) {return 1;};
#else
	extern bool MyAssertFunction( bool b, char* desc, int line, char* file);
#endif

#if defined( _DEBUG )
#define Assert( exp, description ) tpp::MyAssertFunction( (int)(exp), description, __LINE__, __FILE__ )
#else
#define Assert( exp, description )
#endif


#endif

}
