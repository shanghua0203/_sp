void _write(int fd, char *message, int len);

int main() {
	_write(1, "Hello, world!\n", 14);
}
