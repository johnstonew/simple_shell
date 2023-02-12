#include "main.h"

/**
 * exec_cve - finds builtins and commands
 *
 * @data: data
 * Return: 1 on success.
 */
int exec_cve(data_shell *data)
{
	int (*builtin)(data_shell *data);

	if (data->args[0] == NULL)
		return (1);

	builtin = get_builtin(data->args[0]);

	if (builtin != NULL)
		return (builtin(data));

	return (cmd_exec(data));
}
