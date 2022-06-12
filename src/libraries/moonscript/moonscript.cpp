/**
 * Copyright (c) 2022 LOVE Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

#include "moonscript.h"

extern "C" LUALIB_API int luaopen_moonscript(lua_State *l);

namespace love
{
namespace moonscript
{

#ifdef LOVE_ENABLE_FILESYSTEM
const char moonscript_package_loader[] =
"local moonscript = require \"moonscript\"\n"
"\n"
"-- Add package loader for love.filesystem moon scripts.\n"
"table.insert(package.loaders, 2, function (name)\n"
"  local name_path = name:gsub(\"%.\", \"/\")\n"
"  local file_path, text\n"
"  for path in love.filesystem.getRequirePath():gsub(\"%.lua\", \".moon\"):gmatch(\"[^;]+\") do\n"
"    file_path = path:gsub(\"?\", name_path)\n"
"    text = love.filesystem.read(file_path)\n"
"    if text then\n"
"      break\n"
"    end\n"
"  end\n"
"  if text then\n"
"    res, err = moonscript.loadstring(text, \"@\"..file_path)\n"
"    if not res then\n"
"      error(file_path..\": \"..err)\n"
"    end\n"
"    return res\n"
"  end\n"
"  return nil, \"Could not find moon file\"\n"
"end)\n";
#endif

int __open(lua_State * L)
{
	lua_pushcfunction(L, luaopen_moonscript);
	lua_call(L, 0, 0);

#ifdef LOVE_ENABLE_FILESYSTEM
	if (luaL_loadbuffer(L, (const char *)moonscript_package_loader, sizeof(moonscript_package_loader) - 1, "moonscript_package_loader.lua") == 0)
	{
		lua_call(L, 0, 0);
	}
#endif

	return 0;
}

} // moonscript
} // love
