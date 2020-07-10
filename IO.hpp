#pragma once

#ifndef EngineIO_H
#define EngineIO_H

namespace Engine
{
	//Important: Don't try to use IO in the constructor of a global object may result in an error or unexpected behavoir
	namespace IO
	{
		const char* projectPath;
	}
}

#endif