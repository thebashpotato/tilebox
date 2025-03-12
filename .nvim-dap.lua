local dap = require("dap")

-- sudo pacman -S lldb
dap.adapters.lldb = {
	type = "executable",
	command = "/usr/bin/lldb-dap",
	name = "lldb",
}

dap.configurations.cpp = {
	{
		name = "Launch tilebox core peripheral logger",
		type = "lldb",
		request = "launch",
		program = function()
			return vim.fn.getcwd() .. "/build/tilebox/examples/tilebox-peripheral-logger-example"
		end,
		cwd = "${workspaceFolder}",
		stopOnEntry = false,
	},
	{
		name = "Launch tilebox unit tests",
		type = "lldb",
		request = "launch",
		program = function()
			return vim.fn.getcwd() .. "/build/tilebox/tests/tilebox_tests"
		end,
		cwd = "${workspaceFolder}",
		stopOnEntry = false,
	},
}
