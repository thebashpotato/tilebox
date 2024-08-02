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
			return vim.fn.getcwd() .. "/build/tilebox-core/examples/tilebox-core-peripheral-logger-example"
		end,
		cwd = "${workspaceFolder}",
		stopOnEntry = false,
	},
	{
		name = "Launch tilebox core scratch",
		type = "lldb",
		request = "launch",
		program = function()
			return vim.fn.getcwd() .. "/build/tilebox-core/examples/tilebox-core-scratch-example"
		end,
		cwd = "${workspaceFolder}",
		stopOnEntry = false,
	},
	{
		name = "Launch tilebox unit tests",
		type = "lldb",
		request = "launch",
		program = function()
			return vim.fn.getcwd() .. "/build/tilebox-core/tests/tilebox-core-tests"
		end,
		cwd = "${workspaceFolder}",
		stopOnEntry = false,
	},
}
