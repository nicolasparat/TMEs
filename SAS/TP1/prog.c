#include <stdio.h>
#include <unistd.h>

int main() {
	uid_t uid = getuid();
	uid_t euid = geteuid();
	printf("UID réel  : %d\n", uid);
	printf("UID effectif : %d\n", euid);

	return 0;
}
