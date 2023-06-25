#include "simple_shell.h"

/**
 * checkEnv - To check if the typed variable is an env variable
 *
 * @h: The head of linked list
 * @inp: input string
 * @data: data structure
 * Return: no return
 */
void checkEnv(r_var **h, char *inp, dataShell *data)
{
	int row, chr, p, lval;
	char **_envr;

	_envr = data->envir;
	for (row = 0; _envr[row]; row++)
	{
		for (p = 1, chr = 0; _envr[row][chr]; chr++)
		{
			if (_envr[row][chr] == '=')
			{
				lval = _stringlen(_envr[row] + chr + 1);
				addRvarNode(h, p, _envr[row] + chr + 1, lval);
				return;
			}

			if (inp[p] == _envr[row][chr])
				p++;
			else
				break;
		}
	}

	for (p = 0; inp[p]; p++)
	{
		if (inp[p] == ' ' || inp[p] == '\t' || inp[p] == ';' || inp[p] == '\n')
			break;
	}

	addRvarNode(h, p, NULL, 0);
}

/**
 * checkVars - To check if the typed variable is $$ or $?
 *
 * @h: head of the linked list
 * @inp: input string
 * @st: last status of the Shell
 * @data: data structure
 * Return: no return
 */
int checkVars(r_var **h, char *inp, char *st, dataShell *data)
{
	int j, lst, lpd;

	lst = _stringlen(st);
	lpd = _stringlen(data->pid);

	for (j = 0; inp[j]; j++)
	{
		if (inp[j] == '$')
		{
			if (inp[j + 1] == '?')
				addRvarNode(h, 2, st, lst), j++;
			else if (inp[j + 1] == '$')
				addRvarNode(h, 2, data->pid, lpd), j++;
			else if (inp[j + 1] == '\n')
				addRvarNode(h, 0, NULL, 0);
			else if (inp[j + 1] == '\0')
				addRvarNode(h, 0, NULL, 0);
			else if (inp[j + 1] == ' ')
				addRvarNode(h, 0, NULL, 0);
			else if (inp[j + 1] == '\t')
				addRvarNode(h, 0, NULL, 0);
			else if (inp[j + 1] == ';')
				addRvarNode(h, 0, NULL, 0);
			else
				checkEnv(h, inp + j, data);
		}
	}

	return (j);
}

/**
 * replacedInput - To replace string into variables
 *
 * @head: The head of the linked list
 * @inp: input string
 * @newInput: new input string (replaced)
 * @nlen: new length
 * Return: replaced string
 */
char *replacedInput(r_var **head, char *inp, char *newInput, int nlen)
{
	r_var *indx;
	int i, j, k;

	indx = *head;
	for (j = i = 0; i < nlen; i++)
	{
		if (inp[j] == '$')
		{
			if (!(indx->lenVar) && !(indx->lenVal))
			{
				newInput[i] = inp[j];
				j++;
			}
			else if (indx->lenVar && !(indx->lenVal))
			{
				for (k = 0; k < indx->lenVar; k++)
					j++;
				i--;
			}
			else
			{
				for (k = 0; k < indx->lenVal; k++)
				{
					newInput[j] = indx->vvalue[k];
					i++;
				}
				j += (indx->lenVar);
				i--;
			}
			indx = indx->nextNode;
		}
		else
		{
			newInput[i] = inp[j];
			j++;
		}
	}

	return (newInput);
}

/**
 * repVar - To call functions to replace string into vars
 *
 * @inp: input string
 * @datash: data structure
 * Return: replaced string
 */
char *repVar(char *inp, dataShell *datash)
{
	r_var *head, *indx;
	char *stats, *newInput;
	int olen, nlen;

	stats = auxItoa(datash->stats);
	head = NULL;

	olen = checkVars(&head, inp, stats, datash);

	if (head == NULL)
	{
		free(stats);
		return (inp);
	}

	indx = head;
	nlen = 0;

	while (indx != NULL)
	{
		nlen += (indx->lenVal - indx->lenVar);
		indx = indx->nextNode;
	}

	nlen += olen;

	newInput = malloc(sizeof(char) * (nlen + 1));
	newInput[nlen] = '\0';

	newInput = replacedInput(&head, inp, newInput, nlen);

	free(inp);
	free(stats);
	freeRvarList(&head);

	return (newInput);
}
