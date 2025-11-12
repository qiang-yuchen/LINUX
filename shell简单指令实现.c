#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int read_command(char* input);
void parse_command(char* input, char** args);
int execute_command(char** args);
int execute_cd(char** args);
int execute_builtin(char** args);

int main()
{
	char input[1024];
	char* args[64];

	printf("===shell选择===\n");
	printf("输入'exit'退出\n");
	printf("输入'help'查看帮助\n");

	while (1)
	{
		if (read_command(input) == -1)
		{
			break;
		}
		if (strlen(input) == 0)
		{
			continue;
		}
		parse_command(input, args);
		if (execute_command(args) == -1)
		{
			break;
		}
	}
	return 0;
}

int read_command(char* input)
{
	int res = scanf("%[^\n]", input);
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	if (res == EOF)
	{
		printf("\n再见\n");
		return -1;
	}
	if (res == 0)
	{
		input[0] = '\0';
	}
	return 0;
}

void parse_command(char* input, char** args)
{
	int i = 0;
	char* token;
	token = strtok(input, "\t");
	while (token != NULL && i < 64 - 1)
	{
		args[i++] = token;
		token = strtok(NULL, "\t");
	}
	args[i] = NULL;
}

int execute_command(char** args)
{
	if (args[0] == NULL)
	{
		return 0;
	}
	if (execute_builtin(args) == 1)
	{
		return 0;
	}
	pid_t pid = fork(); 
	if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
		{
			fprintf(stderr, "错误: 命令未找到 '%s'\n", args[0]);
			exit(EXIT_FAILURE);
		}
	}
	else if (pid < 0)
	{
		perror("fork失败");
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
	}
	return 0;
}

int execute_builtin(char** args)
{
	if (strcmp(args[0], "exit") == 0)
	{
		printf("再见！\n");
		exit(0);
		return 1;
	}
	else if (strcmp(args[0], "cd") == 0)
	{
		return execute_cd(args);
	}
	else if (strcmp(args[0], "help") == 0)
	{
		printf("\n=== Shell 帮助 ===\n");
		printf("内置命令:\n");
		printf("  cd [目录]    - 切换目录\n");
		printf("  pwd          - 显示当前目录\n");
		printf("  exit         - 退出shell\n");
		printf("  help         - 显示此帮助\n");
		printf("\n其他命令将作为系统命令执行\n\n");
		return 1;
	}
	else if (strcmp(args[0], "pwd") == 0)
	{
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			printf("%s\n", cwd);
		}
		else
		{
			perror("pwd");
		}
		return 1;
	}

	return 0;  // 不是内置命令
}

int execute_cd(char** args)
{
	char* target_dir;
	if (args[1] == NULL)
	{
		target_dir = getenv("HOME");
		if (target_dir == NULL)
		{
			fprintf(stderr, "cd: HOME环境变量未设置\n");
			return 1;
		}
	}
	else if (strcmp(args[1], "~") == 0)
	{
		target_dir = getenv("HOME");
		if (target_dir == NULL) {
			fprintf(stderr, "cd: HOME环境变量未设置\n");
			return 1;
		}
	}
	else {
		target_dir = args[1];
	}
	if (chdir(target_dir) != 0) {
		perror("cd");
		return 1;
	}
	return 1;
}
