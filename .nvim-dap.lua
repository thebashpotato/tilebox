local dap = require("dap")

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
		name = "Launch tilebox core scratch",
		type = "lldb",
		request = "launch",
		program = function()
			return vim.fn.getcwd() .. "/build/tilebox/examples/tilebox-scratch-example"
		end,
		cwd = "${workspaceFolder}",
		stopOnEntry = false,
	},
	{
		name = "Launch tilebox unit tests",
		type = "lldb",
		request = "launch",
		program = function()
			return vim.fn.getcwd() .. "/build/tilebox/tests/tilebox-tests"
		end,
		cwd = "${workspaceFolder}",
		stopOnEntry = false,
	},
}
