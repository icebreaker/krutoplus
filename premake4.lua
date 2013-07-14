-- File System Namespace
fs = {}
fs.MATCH_FILES	= 0
fs.MATCH_DIRS	= 1
function fs.foreach(pattern, callback, match)
	function item_iterator(i, item)
		callback(item)
	end

	local items = {}
	if match == fs.MATCH_DIRS then
		items = os.matchdirs(pattern)
	else
		items = os.matchfiles(pattern)
	end

	table.foreach(items, item_iterator)
end

function fs.include(dir)
  function fs_include(dir)
    local name = path.getbasename(path.getdirectory(dir))
    printf(" Found %s ...", name)
    include(path.getdirectory(dir))
  end
  
  if _ACTION == "gmake" or _ACTION == "clean" then
    printf("Building %s configurations...", string.sub(path.getbasename(dir), 0, -2))
    fs.foreach(path.join(dir, "**/premake4.lua"), fs_include)
  end
end

function kruto(name)
    project(name)

    kind "WindowedApp"
    files { "**.hpp", "**.cpp", "*.def", "*.rc" }
    flags { "WinMain" }

    configuration { "linux", "gmake" }
      links { "GL", "SDL", "SDL_image", "SDL_mixer", "kruto+" }
end

solution "kruto+"
	objdir "build"
	targetdir "build"
	language "C++"

  if _ACTION == "clean" then
    os.rmdir("build")
  end

	configurations { "debug", "release" }
	includedirs { "src", "src/kruto" }
  libdirs { "build" }

  configuration { "release", "gmake" }
    buildoptions { "-O3" }

	configuration { "gmake" }
    buildoptions { "-fdiagnostics-show-option", "-Wall", "-Wextra", "-Werror", "-pedantic", "-ansi" }

  include("src")
  fs.include("demos")
