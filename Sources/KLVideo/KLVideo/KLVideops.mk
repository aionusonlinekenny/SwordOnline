
KLVideops.dll: dlldata.obj KLVideo_p.obj KLVideo_i.obj
	link /dll /out:KLVideops.dll /def:KLVideops.def /entry:DllMain dlldata.obj KLVideo_p.obj KLVideo_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del KLVideops.dll
	@del KLVideops.lib
	@del KLVideops.exp
	@del dlldata.obj
	@del KLVideo_p.obj
	@del KLVideo_i.obj
