#pragma once
#include "Vec2.h"

namespace std {

		template <>
		struct hash<IVec2>
		{
				//TODO Learn good, 
				std::size_t operator()(const IVec2& vec) const
				{
						size_t res = 17;
						res = res * 31 + hash<char>()('X');
						res = res * 31 + hash<int>()(vec.x);
						res = res * 31 + hash<char>()('Y');
						res = res * 31 + hash<int>()(vec.y);
						return res;
				}
		};
}