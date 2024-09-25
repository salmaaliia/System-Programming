#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/ioctl.h>
#include <limits.h>


#define RESET   "\033[0m"
#define BLACK   "\033[0;30m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN    "\033[0;36m"
#define WHITE   "\033[0;37m"


void
usage ()
{
  fprintf (stderr, "Usage: my_ls [options] [directory]\n");
  fprintf (stderr, "Options:\n");
  fprintf (stderr, "  -a  Show all files (including hidden)\n");
  fprintf (stderr, "  -l  Use long listing format\n");
  fprintf (stderr, "  -t  Sort by time modified\n");
  fprintf (stderr, "  -u  Sort by time accessed\n");
  fprintf (stderr, "  -c  Sort by time changed\n");
  fprintf (stderr, "  -i  Print inode number\n");
  fprintf (stderr, "  -f  Do not sort\n");
  fprintf (stderr, "  -d  List directories themselves, not their contents\n");
  fprintf (stderr, "  -1  One file per line\n");
}

int list_all = 0;
int list_long = 0;
int time_sort = 0;
int access_time = 0;
int change_time = 0;
int _inode = 0;
int single_col = 0;
int no_sort = 0;
int list_dir = 0;
int c_ttime = 0, m_ttime = 1, a_ttime = 0;
long total_size = 0;

typedef struct
{
  char *name;
  struct stat fileState;
  char *linkedName;
} fileData;

int
compare_mtime (const void *a, const void *b)
{
  return ((fileData *) b)->fileState.st_mtime -
    ((fileData *) a)->fileState.st_mtime;
}

int
compare_atime (const void *a, const void *b)
{
  return ((fileData *) b)->fileState.st_atime -
    ((fileData *) a)->fileState.st_atime;
}

int
compare_ctime (const void *a, const void *b)
{
  return ((fileData *) b)->fileState.st_ctime -
    ((fileData *) a)->fileState.st_ctime;
}

int
compare_name (const void *a, const void *b)
{
  return strcmp (((fileData *) a)->name, ((fileData *) b)->name);
}

int (*compFunc) (const void *a, const void *b);

//void print_ls(fileData *files, int cnt);

//void traverse(int argc, char** argv);

void print_permissions (int mode);

void print_normal_format (fileData * files, int cnt);

void print_long_format (fileData file);

void print_ls (fileData * files, int cnt);

void traverse (int argc, char **argv);


int
main (int argc, char **argv)
{
  compFunc = compare_name;
  int opt;
  static char dot[] = ".", *dotav[] = { dot, NULL };

  while ((opt = getopt (argc, argv, "latucifd1")) != -1)
    {
      switch (opt)
	{
	case '1':
	  single_col = 1;
	  break;
	case 'l':
	  list_long = 1;
	  single_col = 0;
	  break;
	case 't':
	  time_sort = 1;
	  break;
	case 'u':
	  access_time = 1;
	  change_time = 0;
	  break;
	case 'c':
	  change_time = 1;
	  access_time = 0;
	  break;
	case 'i':
	  _inode = 1;
	  break;
	case 'f':
	  no_sort = 1;
	  list_long = 0;
	  break;
	case 'a':
	  list_all = 1;
	  break;
	case 'd':
	  list_dir = 1;
	  break;
	default:
	case '?':
	  usage ();
	  exit (1);
	}
    }

  if (!no_sort)
    {
      if (time_sort)
	{
	  if (access_time)
	    {
	      compFunc = compare_atime;
	      c_ttime = 0, m_ttime = 0, a_ttime = 1;
	    }
	  else if (change_time)
	    {
	      compFunc = compare_ctime;
	      c_ttime = 1, m_ttime = 0, a_ttime = 0;
	    }
	  else
	    {
	      compFunc = compare_mtime;
	      c_ttime = 0, m_ttime = 1, a_ttime = 0;
	    }
	}
      else if (access_time)
	{
	  if (list_long)
	    {
	      compFunc = compare_name;
	      c_ttime = 0, m_ttime = 1, a_ttime = 0;
	    }
	  else
	    {
	      compFunc = compare_atime;
	      c_ttime = 0, m_ttime = 0, a_ttime = 1;
	    }
	}
      else if (change_time)
	{
	  if (list_long)
	    {
	      compFunc = compare_name;
	      c_ttime = 0, m_ttime = 1, a_ttime = 0;
	    }
	  else
	    {
	      compFunc = compare_ctime;
	      c_ttime = 1, m_ttime = 0, a_ttime = 0;
	    }
	}
      else
	{
	  compFunc = compare_name;
	  c_ttime = 0, m_ttime = 1, a_ttime = 0;
	}
    }

  argc -= optind;
  argv += optind;

  if (argc)
    traverse (argc, argv);
  else
    traverse (1, dotav);
}

void
traverse (int argc, char **argv)
{

  if (list_dir)
    {
      fileData *dirs = NULL;
      for (int i = 0; i < argc; i++)
	{
	  dirs = realloc (dirs, sizeof (fileData) * (i + 1));
	  struct stat st;

	  if (!dirs)
	    {
	      perror ("realloc failed");
	      exit (EXIT_FAILURE);

	    }
	  dirs[i].name = strdup (argv[i]);

	  if (lstat (argv[i], &dirs[i].fileState) == -1)
	    {
	      perror ("stat failed");
	      continue;
	    }


	}

      print_ls (dirs, argc);
      free (dirs);
      return;
    }

  for (int i = 0; i < argc; i++)
    {
      struct dirent *entry;
      DIR *dp = opendir (argv[i]);
      fileData *files = NULL;
      int cnt = 0;
      total_size = 0;

      if (dp == NULL)
	{
	  fprintf (stderr, "Cannot open directory: %s\n", argv[i]);
	  continue;
	}

      errno = 0;
      while ((entry = readdir (dp)) != NULL)
	{
	  if (entry == NULL && errno != 0)
	    {
	      perror ("readdir failed");
	      exit (1);
	    }

	  if ((!no_sort && !list_all) && entry->d_name[0] == '.')
	    continue;

	  files = realloc (files, sizeof (fileData) * (cnt + 1));
	  if (!files)
	    {
	      perror ("realloc failed");
	      exit (EXIT_FAILURE);
	    }

	  files[cnt].name = strdup (entry->d_name);
	  char filepath[1024];
	  snprintf (filepath, sizeof (filepath), "%s/%s", argv[i],
		    entry->d_name);

	  struct stat sb;
	  if (lstat (filepath, &sb) == -1)
	    {
	      perror ("stat failed");
	      free (files[cnt].name);
	      continue;
	    }
	  files[cnt].fileState = sb;

	  mode_t mode = sb.st_mode;
	  if (S_ISLNK (mode))
	    {
	      char *buf;
	      ssize_t nbytes, bufsiz;

	      bufsiz = sb.st_size + 1;
	      if (sb.st_size == 0)
		bufsiz = PATH_MAX;

	      buf = malloc (bufsiz);
	      if (buf == NULL)
		{
		  perror ("malloc");
		  exit (EXIT_FAILURE);
		}

	      nbytes = readlink (filepath, buf, bufsiz);
	      if (nbytes == -1)
		{
		  perror ("readlink");
		  exit (EXIT_FAILURE);
		}
	      buf[nbytes] = '\0';
	      files[cnt].linkedName = strdup (buf);
	      free (buf);

	    }

	  total_size += files[cnt].fileState.st_blocks;
	  cnt++;
	}

      closedir (dp);
      if (argc > 1)
	printf ("%s:\n", argv[i]);

      print_ls (files, cnt);
      free (files);

      if (i < argc - 1)
	printf ("\n");
    }
}

void
print_ls (fileData * files, int cnt)
{
  if (cnt > 0)
    {
      if (!no_sort)
	qsort (files, cnt, sizeof (fileData), compFunc);

      if (list_long)
	{
	  printf ("total %ld\n", (total_size / 2) - 4);
	  for (int j = 0; j < cnt; j++)
	    {
	      if (_inode)
		printf ("%ld ", files[j].fileState.st_ino);
	      print_long_format (files[j]);
	      free (files[j].name);
	    }
	}
      else if (single_col)
	{
	  for (int j = 0; j < cnt; j++)
	    {
	      if (_inode)
		printf ("%ld ", files[j].fileState.st_ino);
	      printf ("%s\n", files[j].name);
	      free (files[j].name);
	    }

	}
      else
	print_normal_format (files, cnt);

      //           free(files);
    }

}

void
print_permissions (int mode)
{
  char str[11];
  strcpy (str, "----------");

  // Owner permissions
  if (mode & S_IRUSR)
    str[0] = 'r';
  if (mode & S_IWUSR)
    str[1] = 'w';
  if (mode & S_IXUSR)
    str[2] = 'x';

  // Group permissions
  if (mode & S_IRGRP)
    str[3] = 'r';
  if (mode & S_IWGRP)
    str[4] = 'w';
  if (mode & S_IXGRP)
    str[5] = 'x';

  // Other permissions
  if (mode & S_IROTH)
    str[6] = 'r';
  if (mode & S_IWOTH)
    str[7] = 'w';
  if (mode & S_IXOTH)
    str[8] = 'x';

  // Special permissions
  if (mode & S_ISUID)
    str[2] = (mode & S_IXUSR) ? 's' : 'S';	// Setuid
  if (mode & S_ISGID)
    str[5] = (mode & S_IXGRP) ? 's' : 'S';	// Setgid
  if (mode & S_ISVTX)
    str[8] = (mode & S_IXOTH) ? 't' : 'T';	// Sticky bit

  str[9] = '\0';		// Null-terminate the string
  printf ("%s ", str);
}


void
print_normal_format (fileData * files, int cnt)
{
  // int columns = 6;  // Number of columns for output
  int max_width = 0;

  // Calculate the maximum width for formatting
  for (int i = 0; i < cnt; i++)
    {
      int width = strlen (files[i].name);
      if (width > max_width)
	{
	  max_width = width;
	}
    }

  struct winsize w;
  ioctl (STDOUT_FILENO, TIOCGWINSZ, &w);
  int terminal_width = w.ws_col;
  int spacing = max_width + 2;
  int columns = terminal_width / spacing;

  if (columns < 1)
    {
      columns = 1;
    }

  for (int i = 0; i < cnt; i++)
    {
      mode_t mode = files[i].fileState.st_mode;
      if (S_ISDIR (mode))
	printf (CYAN "%-*s" RESET, spacing, files[i].name);
      else if (S_ISLNK (mode))
	printf (MAGENTA "%-*s" RESET, spacing, files[i].name);
      else if ((mode & S_IXUSR) || (mode & S_IXOTH) || (mode & S_IXOTH))
	printf (RED "%-*s" RESET, spacing, files[i].name);
      else
	printf ("%-*s", spacing, files[i].name);
      if ((i + 1) % columns == 0 || i == cnt - 1)
	{
	  printf ("\n");	// New line after specified columns
	}
    }
}


void
print_long_format (fileData file)
{
  char timebuf[80];
  struct passwd *pw = getpwuid (file.fileState.st_uid);
  struct group *gr = getgrgid (file.fileState.st_gid);

  struct tm *timeInfo;

  if (m_ttime)
    timeInfo = localtime (&file.fileState.st_mtime);
  else if (a_ttime)
    timeInfo = localtime (&file.fileState.st_atime);
  else if (c_ttime)
    timeInfo = localtime (&file.fileState.st_ctime);

  mode_t mode = file.fileState.st_mode;

  printf ("%c",
	  S_ISREG (mode) ? '-' :
	  S_ISDIR (mode) ? 'd' :
	  S_ISCHR (mode) ? 'c' :
	  S_ISBLK (mode) ? 'b' :
	  S_ISFIFO (mode) ? 'p' :
	  S_ISLNK (mode) ? 'l' : S_ISSOCK (mode) ? 's' : '?');

  // Print permissions
  print_permissions (file.fileState.st_mode);

  // Format the time
  strftime (timebuf, sizeof (timebuf), "%b %d %H:%M", timeInfo);

  // Print the rest of the file details
  printf ("%2lu %-10s %-10s %5lld %s ",
	  file.fileState.st_nlink,
	  pw ? pw->pw_name : "unknown",
	  gr ? gr->gr_name : "unknown",
	  (long long) file.fileState.st_size, timebuf);
  if (S_ISDIR (mode))
    printf (CYAN "%s" RESET, file.name);
  else if (S_ISLNK (mode))
    printf (MAGENTA "%s" RESET, file.name);
  else if ((mode & S_IXUSR) || (mode & S_IXOTH) || (mode & S_IXOTH))
    printf (RED "%s" RESET, file.name);
  else
    printf ("%s", file.name);

  if (S_ISLNK (mode))
    printf (" -> %s", file.linkedName);

  printf ("\n");
}

