/* 
 * tsh - A tiny shell program with job control
 * 
 * My student ID is 09302010035.
 * My name is Wu Yanhong.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */

/* 
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

/* Global variables */
extern char **environ; /* defined in libc */
char prompt[] = "tsh> "; /* command line prompt */
int verbose = 0; /* if true, print additional output */
int nextjid = 1; /* next job ID to allocate */
char sbuf[MAXLINE]; /* for composing sprintf messages */

struct job_t { /* The job struct */
	pid_t pid; /* job PID */
	int jid; /* job ID [1, 2, ...] */
	int state; /* UNDEF, BG, FG, or ST */
	char cmdline[MAXLINE]; /* command line */
};
struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */

/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
int builtin_cmd(char **argv, int *input_fd, int *output_fd);
void do_bgfg(char **argv, int output_fd);
void waitfg(pid_t pid, int output_fd);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv);
char *firstTok(const char *space, const char *input, const char *output,
		const char *pipe);
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs);
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(struct job_t *jobs, pid_t pid);
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid);
int pid2jid(pid_t pid);
void listjobs(struct job_t *jobs, int output_fd);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) {
	char c;
	char cmdline[MAXLINE];
	int emit_prompt = 1; /* emit prompt (default) */

	/* Redirect stderr to stdout (so that driver will get all output
	 * on the pipe connected to stdout) */
	dup2(1, 2);

	/* Parse the command line */
	while ((c = getopt(argc, argv, "hvp")) != EOF) {
		switch (c) {
		case 'h': /* print help message */
			usage();
			break;
		case 'v': /* emit additional diagnostic info */
			verbose = 1;
			break;
		case 'p': /* don't print a prompt */
			emit_prompt = 0; /* handy for automatic testing */
			break;
		default:
			usage();
		}
	}

	/* Install the signal handlers */
	Signal(SIGINT, sigint_handler); /* ctrl-c */
	Signal(SIGTSTP, sigtstp_handler); /* ctrl-z */
	Signal(SIGCHLD, sigchld_handler); /* terminated or stopped child */
	Signal(SIGQUIT, sigquit_handler); /* so parent can cleanly terminate child*/

	/* Initialize the job list */
	initjobs(jobs);

	/* Execute the shell's read/eval loop */
	while (1) {

		/* Read command line */
		if (emit_prompt) {
			printf("%s", prompt);
			fflush(stdout);
		}
		if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
			app_error("fgets error");
		if (feof(stdin)) { /* End of file (ctrl-d) */
			fflush(stdout);
			exit(0);
		}

		/* Evaluate the command line */
		eval(cmdline);
		fflush(stdout);
		fflush(stderr);
	}

	exit(0); /* control never reaches here */
}

/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
 */
void eval(char *cmdline) {
	char *argv[MAXARGS];
	int bg;
	// Initialize the input_fd to STDIN_FILENO and output_fd to STDOUT_FILENO.
	int input_fd = 0, output_fd = 1;
	pid_t pid;
	bg = parseline(cmdline, argv);
	if (argv[0] == NULL)
		return;
	int i = 0;
	// Count the number of the redirection symbol.
	int cin = 0, cout = 0;
	// The saved File Descriptor.
	int saved_output_fd = 1;
	int saved_input_fd = 0;
	// The detector of file input/output.
	int flag_in = 0, flag_out = 0;
	for (i = 0; i < MAXARGS; i++) {
		if (argv[i] == 0)
			break;
		if (!strcmp(argv[i], "<")) {
			flag_in++;
			if (cin > 0) {
				printf("Error: Ambiguous I/O redirection\n");
				return;
			}
			cin = i;
			// Save the old input file descriptor.
			saved_input_fd = dup(STDIN_FILENO);
			if (saved_input_fd<0){
				printf("Save old STDIN descriptor error.\n");
				return;
			}
			int input_fd = open(argv[i + 1], O_RDONLY, 0);
			if (input_fd < 0) {
				printf("Error: %s No such file or directory\n", argv[i + 1]);
				return;
			}
			if (dup2(input_fd, STDIN_FILENO) < 0) {
				printf("eval: Input Redirection error.\n");
				return;
			}
		}
		if (!strcmp(argv[i], ">")) {
			flag_out++;
			if (cout > 0) {
				printf("Error: Ambiguous I/O redirection\n");
				return;
			}
			cout = i;
			// Save the old output file descriptor.
			saved_output_fd = dup(STDOUT_FILENO);
			if (saved_output_fd<0){
				printf("Save old STDOUT descriptor error.\n");
				return;
			}
			int output_fd = open(argv[i + 1], O_CREAT | O_TRUNC | O_RDWR, 777);
			if (output_fd < 0) {
				printf("Error: %s No such file or directory\n", argv[i + 1]);
			}
			if (dup2(output_fd, STDOUT_FILENO) < 0) {
				printf("eval: Output Redirection error.\n");
				return;
			}
		}
	}
	//The argv which transmited to the child process should not include the redirection symbol.(?)
	if (cin != 0) {
		argv[cin] = NULL;
	}
	if (cout != 0) {
		argv[cout] = NULL;
	}
	if (nextjid >= MAXJOBS) {
		printf("Job lists exceeded.");
		return;
	}
	if (!builtin_cmd(argv, &input_fd, &output_fd)) {
		sigset_t mask;
		if (sigemptyset(&mask)<0){
			printf("Set empty sig set error.\n");
			return;
		}
		if (sigaddset(&mask, SIGCHLD)<0){
			printf("Add SIGCHLD to sig set error.\n");
			return;
		}
		// The block operation must be in the part of not building in command.	
		// Otherwise the built-in foreground process will ignore the SIGCHILD signal.
		if (sigprocmask(SIG_BLOCK, &mask, NULL)<0){
			printf("Block sig set error.\n");
			return;
		}
		if ((pid = fork()) == 0) {
			if (sigprocmask(SIG_UNBLOCK, &mask, NULL)<0){
				printf("Unblock sig set error.\n");
				return;
			}
			// Make every process in a individual process group.So that a signal can only send to the foreground process group.
			if (setpgid(0, 0) < 0) {
				printf("eval: Process group ID setting error.\n");
				return;
			}
			if (execve(argv[0], argv, environ) < 0) {
				printf("%s: Command not found.\n", argv[0]);
				exit(0);
			}
			exit(0);
		}
		if (pid<0){
			printf("Make child process error.\n");
			return;
		}
		if (!bg) {
			addjob(jobs, pid, FG, cmdline);
			if (sigprocmask(SIG_UNBLOCK, &mask, NULL)<0){
				printf("Unblock sig set error.\n");
				return;
			}
			// Wait for the foreground process complete.
			waitfg(pid, 1);
			//if (waitpid(pid,&status,0)<0)
			//unix_error("waitfg: waitpid error");
		} else {
			addjob(jobs, pid, BG, cmdline);
			if (sigprocmask(SIG_UNBLOCK, &mask, NULL)<0){
				printf("Unblock sig set error.\n");
				return;
			}
			printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);
		}
	}
	// Restore the old file descriptor.
	if (flag_in > 0) {
		if (dup2(saved_input_fd, STDIN_FILENO)<0){
			printf("Redirect old STDIN error.\n");
			return;
		}
	}
	if (flag_out > 0) {
		if (dup2(saved_output_fd, STDOUT_FILENO)<0){
			printf("Redirect old STDOUT error.\n");
			return;
		}
	}
	return;
}

/* 
 * parseline - Parse the command line and build the argv array.
 * Return true (1) if the user has requested a BG job, false 
 * if the user has requested a FG job.
 */
int parseline(const char *cmdline, char **argv) {
	static char array[MAXLINE]; /* holds local copy of command line */
	char *buf = array; /* ptr that traverses command line */
	char *delim_space; /* points to first space delimiter */
	char *delim_in; /* points to the first < delimiter */
	char *delim_out; /* points to the first > delimiter */
	char *delim_pipe; /* points to the first | delimiter */
	char *delim; /* points to the first delimiter */
	int argc; /* number of args */
	int bg; /* background job? */
	char *last_space = NULL; /* The address of the last space  */

	strcpy(buf, cmdline);
	buf[strlen(buf) - 1] = ' '; /* replace trailing '\n' with space */
	while (*buf && (*buf == ' ')) /* ignore leading spaces */
		buf++;

	/* Build the argv list */
	argc = 0;
	delim_space = strchr(buf, ' ');
	delim_in = strchr(buf, '<');
	delim_out = strchr(buf, '>');
	delim_pipe = strchr(buf, '|');
	while ((delim = firstTok(delim_space, delim_in, delim_out, delim_pipe))) {
		if (delim == delim_space) {
			argv[argc++] = buf;
			*delim = '\0';
			last_space = delim;
		} else if (delim == delim_in) {
			if ((last_space && last_space != (delim - 1)) || (!last_space)) {
				argv[argc++] = buf;
				*delim = '\0';
			}
			argv[argc++] = "<";
			last_space = 0;
		} else if (delim == delim_out) {
			if ((last_space && last_space != (delim - 1)) || (!last_space)) {
				argv[argc++] = buf;
				*delim = '\0';
			}
			argv[argc++] = ">";
			last_space = 0;
		} else if (delim == delim_pipe) {
			if ((last_space && last_space != (delim - 1)) || (!last_space)) {
				argv[argc++] = buf;
				*delim = '\0';
			}
			argv[argc++] = "|";
			last_space = 0;
		}
		buf = delim + 1;
		while (*buf && (*buf == ' ')) /* ignore spaces */
			buf++;
		delim_space = strchr(buf, ' ');
		delim_in = strchr(buf, '<');
		delim_out = strchr(buf, '>');
		delim_pipe = strchr(buf, '|');
	}
	argv[argc] = NULL;

	if (argc == 0) /* ignore blank line */
		return 1;

	/* should the job run in the background? */
	if ((bg = (*argv[argc - 1] == '&')) != 0)
		argv[--argc] = NULL;

	return bg;
}

/*
 * firstTok - Returns a pointer to the first (lowest addy) of the four pointers
 *     that isn't NULL.
 */
char *firstTok(const char *space, const char *input, const char *output,
		const char *pipe) {
	const char *possible[4];
	unsigned int min;
	int i = 1, n = 0;
	if (space == NULL && input == NULL && output == NULL && pipe == NULL)
		return NULL;
	if (space != NULL) {
		possible[n++] = space;
	}
	if (input != NULL) {
		possible[n++] = input;
	}
	if (output != NULL) {
		possible[n++] = output;
	}
	if (pipe != NULL) {
		possible[n++] = pipe;
	}
	min = (unsigned) possible[0];
	for (; i < n; i++) {
		if (((unsigned) possible[i]) < min)
			min = (unsigned) possible[i];
	}
	return (char *) min;
}

/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 */
int builtin_cmd(char **argv, int *input_fd, int *output_fd) {
	if (!strcmp(argv[0], "quit"))
		exit(0);
	if ((!strcmp(argv[0], "bg")) || (!strcmp(argv[0], "fg"))) {
		do_bgfg(argv, 1);
		return 1;
	}
	if (!strcmp(argv[0], "jobs")) {
		listjobs(jobs, 1);
		return 1;
	};
	if (!strcmp(argv[0], "&"))
		return 1;
	return 0; /* not a builtin command */
}

/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv, int output_fd) {
	if (argv[1] == NULL) {
		printf("%s: command requires PID or %%jobid argument\n", argv[0]);
		return;
	}
	//the target job.
	struct job_t* job;
	if (argv[1][0] == '%') {
		int jid = atoi(&argv[1][1]);
		if (jid == 0) {
			printf("%s: argument must be a PID or %%jobid\n", argv[0]);
			return;
		}
		if ((job = getjobjid(jobs, jid)) == NULL) {
			printf("%s: No such job\n", argv[1]);
			return;
		}
	} else {
		int pid = atoi(&argv[1][0]);
		if (pid == 0) {
			printf("%s: argument must be a PID or %%jobid\n", argv[0]);
			return;
		}
		if ((job = getjobpid(jobs, pid)) == NULL) {
			printf("(%d): No such process\n", pid);
			return;
		}
	}
	// Send the SIGCONT signal to make the job run.
	if (kill(-(job->pid), SIGCONT) < 0) {
		printf("do_bgfg: Killing error.\n");
	}
	// Change the job's state.
	if (!strcmp(argv[0], "bg")) {
		job->state = BG;
		printf("[%d] (%d) %s", job->jid, job->pid, job->cmdline);
	} else if (!strcmp(argv[0], "fg")) {
		job->state = FG;
		waitfg(job->pid, 1);
	} else
		printf("do_bgfg: Unknow error.\n");
	return;
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid, int output_fd) {
	// A busy loop.Check if the current pid is a foreground pid. S.T. If the current foreground pid is suspended.
	while (fgpid(jobs) == pid) {
		sleep(1);
	}
	return;
}

/*****************
 * Signal handlers
 *****************/

/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
void sigchld_handler(int sig) {
	pid_t pid;
	int status;
	// Reap all the child process.
	while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
		//Handle with the 3 situation in this handler together.
		if (WIFSIGNALED(status)) {
			printf("Job [%d] (%d) terminated by Signal %d\n", pid2jid(pid),
					pid,
					WTERMSIG(status));
			deletejob(jobs, pid);
		} else if (
		WIFSTOPPED(status)) {
			struct job_t *job = getjobpid(jobs, pid);
			if (job == NULL) {
				printf("sigchld: Get job error.\n");
				return;
			}
			job->state = ST;
			printf("Job [%d] (%d) stopped by Signal %d\n", pid2jid(pid), pid,
			WEXITSTATUS(status));

		} else if (
		WIFEXITED(status)) {
			deletejob(jobs, pid);
		}

		//printf("Handler reaped child %d\n", (int)pid);
	}
	return;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig) {
	pid_t pid = fgpid(jobs);
	if (pid > 0) {
		// Sent the SIGINT signal to the whole foreground process group.
		if (kill(-pid, SIGINT) < 0)
			printf("sigtint_handler error\n");
	}
	return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) {
	pid_t pid = fgpid(jobs);
	if (pid > 0) {
		// Sent the SIGSTP signal to the whole foreground process group.
		if (kill(-pid, SIGTSTP) < 0)
			printf("sigtstp_handler error\n");
	}
	return;
}

/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job) {
	job->pid = 0;
	job->jid = 0;
	job->state = UNDEF;
	job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs) {
	int i;

	for (i = 0; i < MAXJOBS; i++)
		clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs) {
	int i, max = 0;

	for (i = 0; i < MAXJOBS; i++)
		if (jobs[i].jid > max)
			max = jobs[i].jid;
	return max;
}

/* addjob - Add a job to the job list */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline) {
	int i;

	if (pid < 1)
		return 0;

	for (i = 0; i < MAXJOBS; i++) {
		if (jobs[i].pid == 0) {
			jobs[i].pid = pid;
			jobs[i].state = state;
			jobs[i].jid = nextjid++;
			if (nextjid > MAXJOBS)
				nextjid = 1;
			strcpy(jobs[i].cmdline, cmdline);
			if (verbose) {
				printf("Added job [%d] %d %s\n", jobs[i].jid, jobs[i].pid,
						jobs[i].cmdline);
			}
			return 1;
		}
	}
	printf("Tried to create too many jobs\n");
	return 0;
}

/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(struct job_t *jobs, pid_t pid) {
	int i;

	if (pid < 1)
		return 0;

	for (i = 0; i < MAXJOBS; i++) {
		if (jobs[i].pid == pid) {
			clearjob(&jobs[i]);
			nextjid = maxjid(jobs) + 1;
			return 1;
		}
	}
	return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs) {
	int i;

	for (i = 0; i < MAXJOBS; i++)
		if (jobs[i].state == FG)
			return jobs[i].pid;
	return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid) {
	int i;

	if (pid < 1)
		return NULL;
	for (i = 0; i < MAXJOBS; i++)
		if (jobs[i].pid == pid)
			return &jobs[i];
	return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *jobs, int jid) {
	int i;

	if (jid < 1)
		return NULL;
	for (i = 0; i < MAXJOBS; i++)
		if (jobs[i].jid == jid)
			return &jobs[i];
	return NULL;
}

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid) {
	int i;

	if (pid < 1)
		return 0;
	for (i = 0; i < MAXJOBS; i++)
		if (jobs[i].pid == pid) {
			return jobs[i].jid;
		}
	return 0;
}

/* listjobs - Print the job list */
void listjobs(struct job_t *jobs, int output_fd) {
	int i;
	char buf[MAXLINE];

	for (i = 0; i < MAXJOBS; i++) {
		memset(buf, '\0', MAXLINE);
		if (jobs[i].pid != 0) {
			sprintf(buf, "[%d] (%d) ", jobs[i].jid, jobs[i].pid);
			if (write(output_fd, buf, strlen(buf)) < 0) {
				fprintf(stderr, "Error writing to output file\n");
				exit(1);
			}
			memset(buf, '\0', MAXLINE);
			switch (jobs[i].state) {
			case BG:
				sprintf(buf, "Running    ");
				break;
			case FG:
				sprintf(buf, "Foreground ");
				break;
			case ST:
				sprintf(buf, "Stopped    ");
				break;
			default:
				sprintf(buf, "listjobs: Internal error: job[%d].state=%d ", i,
						jobs[i].state);
			}
			if (write(output_fd, buf, strlen(buf)) < 0) {
				fprintf(stderr, "Error writing to output file\n");
				exit(1);
			}
			memset(buf, '\0', MAXLINE);
			sprintf(buf, "%s", jobs[i].cmdline);
			if (write(output_fd, buf, strlen(buf)) < 0) {
				fprintf(stderr, "Error writing to output file\n");
				exit(1);
			}
		}
	}
	if (output_fd != STDOUT_FILENO)
		close(output_fd);
}
/******************************
 * end job list helper routines
 ******************************/

/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void) {
	printf("Usage: shell [-hvp]\n");
	printf("   -h   print this message\n");
	printf("   -v   print additional diagnostic information\n");
	printf("   -p   do not emit a command prompt\n");
	exit(1);
}

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg) {
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler) {
	struct sigaction action, old_action;

	action.sa_handler = handler;
	sigemptyset(&action.sa_mask); /* block sigs of type being handled */
	action.sa_flags = SA_RESTART; /* restart syscalls if possible */

	if (sigaction(signum, &action, &old_action) < 0)
		unix_error("Signal error");
	return (old_action.sa_handler);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) {
	if (verbose)
		printf("siquit_handler: terminating after receipt of SIGQUIT signal\n");
	exit(1);
}

