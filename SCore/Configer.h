#ifndef CONFIGER_H_
#define CONFIGER_H_

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<string>
#include<vector>
#include<iostream>
#include<memory>
#include<algorithm>
#include<functional>
#include<cstdint>
#include<map>

class SWidget;
using SWidgetList = std::vector<SWidget*>;

namespace SGUI
{
	enum Orientation
	{
		Horizontal,
		Vertical
	};
	/** @brief 对齐方式, 可以使用 | 进行组合 */
	enum Alignment
	{
		AlignTop			= 1 << 1,	/**< 上对齐*/
		AlignBottom		= 1 << 2,	/**< 下对齐*/
		AlignLeft		= 1 << 3,	/**< 左对齐*/
		AlignRight		= 1 << 4,	/**< 右对齐*/
		AlignHCenter	= 1 << 5,	/**< 水平居中*/
		AlignVCenter	= 1 << 6,	/**< 垂直居中*/
		AlignCenter		= AlignHCenter | AlignVCenter	/**< AlignHCenter | AlignVCenter*/
	};
	using Alignments = int;

	enum GlobalColor:Uint32
	{
		Black	= 0x000000'FF,
		White	= 0xFFFFFF'FF,
		Red		= 0xFF0000'FF,
		Green	= 0x00FF00'FF,
		Blue	= 0x0000FF'FF,
		Transparent = 0x00000000
	};
#define RandomColor SColor(rand() % 255, rand() % 255, rand() % 255)
}


#define slog SDL_Log
#define sclog std::cout<<"[info:"<<__LINE__<<"] "


#define BIND_LAMBDA(body) [=]()body
#define BIND_0(object,func) std::bind(func,(object))
#define BIND_1(object,func) std::bind(func,(object),std::placeholders::_1)
#define BIND_2(object,func) std::bind(func,(object),std::placeholders::_1,std::placeholders::_2)
#define BIND_3(object,func) std::bind(func,(object),std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)


#define _CRTDBG_MAP_ALLOC
#include<crtdbg.h>
//Vs中检测内存泄漏代码，C/C++通用
#ifdef _DEBUG

#ifdef  __cplusplus
#include<iostream>
#define new   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define _CRTDBG_MAP_ALLOC
#include<malloc.h>
#include<crtdbg.h>
#include<stdlib.h>
#endif //  __cplusplus

#else
#include<malloc.h>
#endif


#endif