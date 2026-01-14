#include <stdio.h>

int main() {
	int i = 1;
	switch (i++) {
	case 1: printf("%d", ++i);
	case 2: printf("%d", i++);
	}
	return 0;
}