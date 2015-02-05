@echo off

rem 用于显示 SimpleCSharpApp.exe 的返回值

SimpleCSharpApp.exe
@if "%ERRORLEVEL%" == "0" goto success

:fail
  echo 程序执行失败！
  echo 返回值 = %ERRORLEVEL%
  goto end
:success
  echo 程序执行成功！
  echo 返回值 = %ERRORLEVEL%
  goto end
:end
  echo 完成。
