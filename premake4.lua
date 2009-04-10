-- termovtx premake file
-- To build use premake4 tool (http://industriousone.com/premake)

-- Configure paths to the dependent projects
newoption {
  trigger = "ntldir",
  description = "Provide directory to the oNTL (e.g. 'C:\\ontl\\branches\\x64\\ntl')",
  value = "path"
}
newoption {
  trigger = "ddkdir",
  description = "Provide directory to the DDK libraries (e.g. C:\\DDK\\6000 or C:\\DDK\\6000\\lib\\wnet)",
  value = "path"
}
-- for premake 4.0
newoption {
  trigger = 'arch',
  description = 'Specify the target platform',
  value = 'value',
  allowed = {
    { 'x86', '32-bit mode'},
    { 'x64', '64-bit mode'}
  }
}

-- Solution and project configuration
solution "termovtx"
  configurations { "debug", "release" }
-- uncomment for premake 4.1
--  platforms { "x32", "x64" }
  targetdir "out"
  objdir    "out"

project  "termovtx"
  language  "C++"
  kind      "WindowedApp"
  targetextension (".sys")
  flags { "ExtraWarnings", "NoPCH", "Symbols" }

  files {
    -- headers
    "*.h",
    "*.hxx",
    -- sources
    "*.cpp"
  }

  if (not _ACTION) or _ACTION == 'clean' or _OPTIONS["ntldir"] then 
    includedirs { _OPTIONS["ntldir"] } 
  else 
    print("Warning: path to the oNTL doesn't specified! See `premake4 --help` for options")
  end



function get_libdir(arch)
  -- skip '--help' action
  if (not _ACTION) or _ACTION == 'clean' then return end
  -- 'ddkdir' required
  if not _OPTIONS["ddkdir"] then
    print("Error: path to the DDK required! See `premake4 --help` for options")
    return
  end
  local ddk = _OPTIONS["ddkdir"]
  local libarch = iif(arch == 'x64', 'amd64', 'i386')
  local nt = ddk .. "/" .. libarch
  if not os.isfile(nt .. "/ntdll.lib") then
    local nt2 = ddk .. "/lib/wnet/" .. libarch
    if not os.isfile(nt2 .. "/ntdll.lib") then 
      print("Error: can't find 'ntdll.lib' in provided paths!\n" .. string.format("Looked up in '%s' and '%s'\n", nt, nt2))
      return
    end
    nt = nt2
  end
  return nt
end

configuration "windows"
  flags { "StaticRuntime", "No64BitChecks", "NoEditAndContinue", "NoManifest", "NoExceptions" }
-- disable specific warnings:
  buildoptions { "/wd4512", "/wd4201" }

  buildoptions { "/Gz", "/MT", "/GS-", "/Gy-" }
  linkoptions  { "/entry:DrvEntry", "/incremental:no", "/subsystem:native", "/driver", "/release", "/nodefaultlib:libcmt.lib", "/nodefaultlib:libcmtd.lib" }
  links { "ntoskrnl", "hal" }
-- comment if premake 4.1
  libdirs { get_libdir(_OPTIONS['arch']) }
  if _OPTIONS['arch'] == 'x64' then linkoptions { "/machine:x64" } end

-- for premake 4.1
--configuration "x32"
--  libdirs { get_libdir('x32') }
-- for premake 4.1
--configuration "x64"
--  linkoptions { "/machine:x64" }
--  libdirs { get_libdir('x64') }

configuration "debug"
  defines { "DEBUG" }

configuration "release"
  flags { "Optimize" }
  defines { "NDEBUG" }


