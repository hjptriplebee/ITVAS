rem 删除RE-ID工程中的编译文件

del RE-ID.sdf /f /s /q /a 
del RE-ID.v12.suo /f /s /q /a 
rd x64 /s /q
rd .\RE-ID\x64 /s /q
rd .\reid_match\x64 /s /q
rd .\reid_persondetection\x64 /s /q
rd .\reid_tracking\x64 /s /q
rd .\test\x64 /s /q
rd .\test_qil\x64 /s /q
rd .\test_maml\x64 /s /q
rd .\test_huangjp\x64 /s /q
rd .\test_fengjh\x64 /s /q
rd .\common_util\x64 /s /q
rd .\common_util_ui\x64 /s /q
rd ipch /s /q

rem /s 的意思是包括删除其下的子文件夹 
rem /q 安静模式。删除全局通配符时，不要求确认