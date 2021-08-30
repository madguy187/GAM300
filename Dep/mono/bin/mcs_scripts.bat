@echo off
"%~dp0\mono.exe" %MONO_OPTIONS% "%~dp0\..\lib\mono\4.5\mcs.exe" %* -r:..\EclipseScriptsAPI.dll -t:library -out:..\EclipseScripts.dll src\Assets\Scripts\*.cs > mcs_script_output.txt 2>&1


