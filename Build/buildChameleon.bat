set props=..\Source\Chameleon\Properties

copy %props%\AssemblyInfo.cs %props%\AssemblyInfo.cs.bak /Y
subwcrev .. %props%\AssemblyInfo_template.cs %props%\AssemblyInfo.cs
%WINDIR%\Microsoft.NET\Framework\v4.0.30319\msbuild.exe ChameleonBuild.proj
copy  %props%\AssemblyInfo.cs.bak %props%\AssemblyInfo.cs /Y
del %props%\AssemblyInfo.cs.bak

VersionExtractor.exe