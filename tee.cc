#include <stdio.h>
#include <time.h>
#include <windows.h>

static char file[4096];

static void help() {
	printf(
		"Usage: tee [options] [file]\n"
		"\n"
		"-a  Append\n"
		"-h  Show help\n"
		"-V  Show version\n"
		"\n"
		"File defaults to:\n"
		"%s\n",
		file);
}

int main(int argc, char **argv) {
	time_t rawtime;
	time(&rawtime);
	auto timeinfo = localtime(&rawtime);
	strftime(file, sizeof file, "%Y-%m-%d %H%M%S.log", timeinfo);

	bool append = 0;
	for (int i = 1; i < argc; ++i) {
		auto s = argv[i];
		if (*s != '-') {
			snprintf(file, sizeof file, "%s", s);
			continue;
		}
		while (*s == '-') ++s;
		switch (*s) {
		case '?':
		case 'h':
			help();
			return 0;
		case 'a':
			append = 1;
			break;
		case 'V':
		case 'v':
			printf("tee version 0\n");
			return 0;
		default:
			fprintf(stderr, "%s: unknown option\n", argv[i]);
			return 1;
		}
	}

	auto in = GetStdHandle(STD_INPUT_HANDLE);
	auto out = GetStdHandle(STD_OUTPUT_HANDLE);
	auto f = CreateFile(file, GENERIC_WRITE, FILE_SHARE_READ, 0, append ? OPEN_ALWAYS : CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	static char buf[1 << 16];
	for (;;) {
		DWORD n;
		auto r = ReadFile(in, buf, sizeof buf, &n, 0);
		if (!r) break;
		WriteFile(out, buf, n, 0, 0);
	}
	return 0;
}
