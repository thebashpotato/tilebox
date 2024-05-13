local dap = require("dap")

dap.adapters.lldb = {
	type = "executable",
	command = "lldb-vscode",
	name = "lldb",
}

dap.configurations.cpp = {
	{
		name = "Launch tilebox core scratch",
		type = "lldb",
		request = "launch",
		program = function()
			return vim.fn.getcwd() .. "/build/tilebox-core/examples/tilebox-core-peripheral-logger-example"
		end,
		cwd = "${workspaceFolder}",
		stopOnEntry = false,
	},
}
