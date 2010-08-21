#ifndef _Exception_H_
#define _Exception_H_
#include "Prerequisites.h"
namespace RCP
{
	class Exception
	{
	protected:
		int line;
		std::string time;
		std::string description;
		std::string source;
		std::string file;
	public :
		Exception(int l, const std::string& t, const std::string& d, const std::string& s, const std::string& f):
		   line(l),time(t),description(d),source(s),file(f)
		{
		}
	};
#ifndef THROW_EXCEPTION
#define THROW_EXCEPTION(desc) {throw Exception(__LINE__, __TIME__ , desc,__FUNCTION__ , __FILE__);}
#endif//THROW_EXCEPTION
}
#endif//_Exception_H_
