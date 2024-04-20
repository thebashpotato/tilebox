local dap = require("dap")

dap.adapters.lldb = {
	type = "executable",
	command = "lldb-vscode",
	name = "lldb",
}

dap.configurations.cpp = {
	{
		name = "Launch window manager",
		type = "lldb",
		request = "launch",
		program = function()
			return vim.fn.getcwd() .. "/build/window-manager/tilebox-wm"
		end,
		cwd = "${workspaceFolder}",
		stopOnEntry = false,
	},
}
