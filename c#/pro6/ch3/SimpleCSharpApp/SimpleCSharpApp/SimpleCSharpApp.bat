@echo off

rem ������ʾ SimpleCSharpApp.exe �ķ���ֵ

SimpleCSharpApp.exe
@if "%ERRORLEVEL%" == "0" goto success

:fail
  echo ����ִ��ʧ�ܣ�
  echo ����ֵ = %ERRORLEVEL%
  goto end
:success
  echo ����ִ�гɹ���
  echo ����ֵ = %ERRORLEVEL%
  goto end
:end
  echo ��ɡ�
