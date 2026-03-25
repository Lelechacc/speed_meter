@echo off
setlocal enabledelayedexpansion

:: 设置输出文件名
set OUTPUT_FILE=ui_functions_for_gemini.md

echo # UI Source Code Summary > %OUTPUT_FILE%
echo Generated on: %date% %time% >> %OUTPUT_FILE%
echo. >> %OUTPUT_FILE%

:: 遍历当前目录下所有 ui_*.c 文件
for %%f in (ui_*.c) do (
    echo [Processing] %%f...
    echo ## File: %%f >> %OUTPUT_FILE%
    echo ^`^`^`c >> %OUTPUT_FILE%
    
    :: 将文件内容全部追加到 Markdown 中
    type "%%f" >> %OUTPUT_FILE%
    
    echo ^`^`^` >> %OUTPUT_FILE%
    echo. >> %OUTPUT_FILE%
    echo --- >> %OUTPUT_FILE%
    echo. >> %OUTPUT_FILE%
)

echo.
echo ======================================================
echo 搞定！已经生成了: %OUTPUT_FILE%
echo 请打开这个文件，把内容全部复制发给我。
echo ======================================================
pause