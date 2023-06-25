#include "simple_shell.h"

/**
 * freeData -To free data structure
 *
 * @datash: The data structure
 * Return: no return
 */
void freeData(dataShell *datash)
{
	unsigned int p;

	for (p = 0; datash->envir[p]; p++)
	{
		free(datash->envir[p]);
	}

	free(datash->envir);
	free(datash->pid);
}

/**
 * setData - To initialize data structure
 *
 * @datash: data structure
 * @argvec: argument vector
 * Return: no return
 */
void setData(dataShell *datash, char **argvec)
{
	unsigned int p;

	datash->argvec = argvec;
	datash->inp = NULL;
	datash->args = NULL;
	datash->stats = 0;
	datash->count = 1;

	for (p = 0; environ[p]; p++)
		;

	datash->envir = malloc(sizeof(char *) * (p + 1));

	for (p = 0; environ[p]; p++)
	{
		datash->envir[p] = _stringdup(environ[p]);
	}

	datash->envir[p] = NULL;
	datash->pid = auxItoa(getppid());
}

/**
 * main - an entry point
 *
 * @ac: the argument count
 * @argvec: argument vector
 *
 * Return: 0 on success.
 */
int main(int ac, char **argvec)
{
	dataShell datash;
	(void) ac;

	signal(SIGINT, getSigint);
	setData(&datash, argvec);
	shellLoop(&datash);
	freeData(&datash);
	if (datash.stats < 0)
		return (255);
	return (datash.stats);
}
