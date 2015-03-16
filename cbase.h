#ifndef _CBASE_H_
#define _CBASE_H_

namespace SRenderer
{
	class CBase
	{
		virtual CBase *clone() const=0;
	};
}

#endif