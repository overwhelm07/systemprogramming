#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define CHOP(x) x[strlen(x) - 1] = ' '

typedef struct _booktype
{
	char *book;
	char *author;
	char *publisher;
	char *year;
}booktype;

typedef struct _node
{
	booktype Data;
	struct _node *next;
}node;

void menulist();
void borrow_book();
int main(){

	int sockfd;
	int len, val;

	struct sockaddr_un address;

	int result;
	//client socket create
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

	//server socket name direction
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, "server_socket");
	len = sizeof(address);

	result = connect(sockfd, (struct sockaddr *)&address, len);
	//connect() 서버로 접속 시도
	if (result == -1){
		perror("client 1");
		exit(1);
	}
	/*
	//read and write
	write(sockfd, &ch, 1);
	read(sockfd, &ch, 1);
	printf("char from server = %c\n", ch);
	*/
	while(1){
		char ch;
		menulist();
		fprintf(stdout, "Menu Select = ");
		fscanf(stdin, "%d", &val);
		getchar();

		switch(val){

				case 1:{//책목록


				}
				case 2:{//책대여
				ch='2';
				write(sockfd, &ch, 1);
	char name[20];
	int bookLen, tmp;
	printf("빌릴 책이름을 입력하세요: ");
	fgets(name, 20, stdin);
	CHOP(name);
	bookLen = strlen(name);
	write(sockfd, &bookLen, 1);//1

	write(sockfd, &name, bookLen);//2
	read(sockfd, &tmp, 1);
	if (tmp == 1){
		printf("책 %s가 정상적으로 대여가 완료되었습니다.\n", name);
	}
	else{
		printf("요청하신 책이 없습니다. 다른 책을 대여해보세요.\n");
	}

				continue;
				}
				case 3:{//책반납
				}
				case 4:{//내가빌린책
				}
				case 5:{//종료하기
					printf("Program Exit!!\n");
					return 0;
				}
				default: return 0;
		}

	}



	close(sockfd);
	exit(0);
}
void borrow_book(){
	/*
	char name[20];
	int len2, tmp;
	printf("빌릴 책이름을 입력하세요: ");
	fgets(name, 20, stdin);
	CHOP(name);
	len2 = strlen(name);
	write(sockfd, &len2, 1);//1

	write(sockfd, &name, len2);//2
	read(sockfd, &tmp, 1);
	if (tmp == 1){
		printf("책 %s가 정상적으로 대여가 완료되었습니다.\n", name);
	}
	else{
		printf("요청하신 책이 없습니다. 다른 책을 대여해보세요.\n");
	}*/
}

void menulist()
{
	printf("\n\n");
	printf("\t\t정헌 도서관에 오신것을 환영합니다.!!!!!!!!!!!\n\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "\t\t\t------------------\n");
	fprintf(stdout, "\t\t\t| 1. 책목록보기    |\n");
	fprintf(stdout, "\t\t\t| 2. 책대여하기    |\n");
	fprintf(stdout, "\t\t\t| 3. 책반납하기    |\n");
	fprintf(stdout, "\t\t\t| 4. 내가빌린책    |\n");
	fprintf(stdout, "\t\t\t| 5. 종료하기      |\n");
	fprintf(stdout, "\t\t\t------------------\n");
	fprintf(stdout, "\n");
}
