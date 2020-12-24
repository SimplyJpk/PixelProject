#pragma once
#include "Vec2.h"

namespace std {

		template <>
		struct hash<IVec2>
		{
				size_t operator()(const IVec2& k)const
				{
						return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
				}

				bool operator()(const IVec2& a, const IVec2& b)const
				{
						return a.x == b.x && a.y == b.y;
				}

				//? This bad?
				//? std::size_t operator()(const IVec2& vec) const
				//? {
				//? 		size_t res = 17;
				//? 		//res = res * 31 + hash<char>()('X');
				//? 		res = res * 31 + hash<int>()(vec.x);
				//? 		res = res * 31 + hash<char>()('Y');
				//? 		res = res * 31 + hash<int>()(vec.y);
				//? 		return res;
				//? }
		};
}