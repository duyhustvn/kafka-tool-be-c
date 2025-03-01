(dap-register-debug-template
 "cpptools: Debug HTTP Server"
 (list :type "cppdbg"
       :request "launch"
       :name "cpptools: Debug HTTP Server"
       :MIMode "gdb"
       :program "${workspaceFolder}/bin/main"
       :cwd "${workspaceFolder}"))
