// 튜플수: select count(*) from table_name;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <mysql.h>
#include <unistd.h>

#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "0000"
#define DB_NAME "myspace"

#define CHOP(x) x[strlen(x) - 1] = ' '


int checkfile();
void menulist();
void Datainput();
void deleteData();
void datalist();
void run_server();

MYSQL  *connection = NULL, conn;
MYSQL_RES *sql_result;
MYSQL_ROW sql_row;

int query_stat;
int num;
char bookname[20]; char author[10]; char publisher[10]; char price[10]; char year[10]; char query[255];
int icnt;//보유중인 책 개수
int rent;//빌린 책 개수
int can_borrow;//빌릴 수 있는 책

typedef struct _booktype
{
	char book[30];
	char author[30];
	char publisher[30];
	char year[20];
	char avail;
}booktype;

int main()
{

	int Val;


	mysql_init(&conn);//연결 지시자 초기화
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);//mysql서버에 직접 접근

	if (connection == NULL){
		fprintf(stderr, "MySQL Connection Error :%s", mysql_error(&conn));
		return 1;
	}




	while (1)
	{
		icnt = checkfile();

		menulist();
		fprintf(stdout, "Menu Select = ");
		fscanf(stdin, "%d", &Val);
		getchar();

		switch (Val)
		{

		case 1:
		{
			while (1)
			{
				Datainput();
    			icnt++;
				break;
			}
			break;
		}
		case 2:{
			while (1){
				deleteData();
				icnt--;
				break;
			}
		}

		case 3:
		{
			datalist();
			break;
		}

		case 4:{
			run_server();
			break;
		}

		case 5:
		{
			printf("Program Exit!!\n");
			return 0;
		}
		default:
			return 0;
		}
	}

	return 0;
}

void menulist()
{
    //system("clear");
	printf("\n\n");
	printf("\t\t\tWELCOM TO 정헌 도서관 관리 프로그램!!!!!!!!!!!\n\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "\t\t\t------------------\n");
	fprintf(stdout, "\t\t\t| 1. 책추가하기   |\n");
	fprintf(stdout, "\t\t\t| 2. 책삭제하기   |\n");
	fprintf(stdout, "\t\t\t| 3. 책 목록      |\n");
	fprintf(stdout, "\t\t\t| 4. 서버구동     |\n");
	fprintf(stdout, "\t\t\t| 5. 종료하기     |\n");
	fprintf(stdout, "\t\t\t------------------\n");
	fprintf(stdout, "\n");
}


int checkfile()
{
	int icnt;

	query_stat = mysql_query(connection, "select count(*) from lib");
	sql_result = mysql_store_result(connection);
	sql_row = mysql_fetch_row(sql_result);
	icnt = atoi(sql_row[0]);//문자열을 int형으로 형변환
	query_stat = mysql_query(connection, "select count(*) from lib where avail ='n'");
	sql_result = mysql_store_result(connection);
	sql_row = mysql_fetch_row(sql_result);
	rent = atoi(sql_row[0]);
	can_borrow = icnt - rent;

	printf("\n\n");
	printf("\t현재 보유중인 책 = %d권 \t대출 가능한 책 = %d권 \t대출 중인 책 = %d권\n", icnt, can_borrow, rent);

	return icnt;
}

//책추가
void Datainput()
{

	mysql_free_result(sql_result);

	fprintf(stdout, "--------------------\n");
	fprintf(stdout, "책이름을 입력\n");
	fgets(bookname, 20, stdin);
	CHOP(bookname);

	fprintf(stdout, "저자 입력\n");
	fgets(author, 10, stdin);
	CHOP(author);

	fprintf(stdout, "출판사 입력\n");
	fgets(publisher, 10, stdin);
	CHOP(publisher);

	fprintf(stdout, "가격 입력\n");
	fgets(price, 10, stdin);
	CHOP(price);

	fprintf(stdout, "발행 년도입력\n");
	fgets(year, 10, stdin);
	CHOP(year);
	fprintf(stdout, "--------------------\n");

	sprintf(query, "insert into lib values('%s','%s','%s','%s','%s', 'y')", bookname, author, publisher, price, year);
	query_stat = mysql_query(connection, query);
	if (query_stat != 0){
		fprintf(stderr, "MySQL Query Error : %s", mysql_error(&conn));
	}
}

//책삭제
void deleteData(){
	char name[20];
	printf("제거할 책 이름을 입력:");
	fgets(name, 20, stdin);
	CHOP(name);
	sprintf(query, "delete from lib where book = '%s'", name);
	query_stat = mysql_query(connection, query);
	if (query_stat != 0){
		printf("에러발생!!\n");
		printf("존재하지 않는 책이거나 철자를 똑바로 입력하세용!\n");
	}
	else{
		printf("%s가 정상적으로 목록에서 제거되었습니다.", name);
	}

}

//책목록
void datalist()
{
	int num = 1;
	query_stat = mysql_query(connection, "select * from lib order by book asc");//오름차순으로정렬함

	sql_result = mysql_store_result(connection);


	fprintf(stdout, "--------------------");
	fprintf(stdout, "--------------------");
	fprintf(stdout, "--------------------");
	fprintf(stdout, "--------------------\n");
	fprintf(stdout, "%3s", "num");
	fprintf(stdout, "%13s", "Book name");
	fprintf(stdout, "%15s", "Author");
	fprintf(stdout, "%15s", "Publisher");
	fprintf(stdout, "%10s", "Price");
	fprintf(stdout, "%10s", "Year");
	fprintf(stdout, "%13s\n", "Available");
	fprintf(stdout, "--------------------");
	fprintf(stdout, "--------------------");
	fprintf(stdout, "--------------------");
	fprintf(stdout, "--------------------\n");


	while ((sql_row = mysql_fetch_row(sql_result)) != NULL){
		printf("%3d %13s \t%s \t\t%s \t\t%s \t%s \t%5s\n",num++ , sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5]);
	}
	fprintf(stdout, "--------------------");
	fprintf(stdout, "--------------------");
	fprintf(stdout, "--------------------");
	fprintf(stdout, "--------------------\n");

}

//서버실행 gcc -o test test.c -I/usr/include/mysql -L/usr/lib/mysql -lmysqlclient
void run_server(){

    int count = 0;

	int server_sockfd, client_sockfd;
	int server_len, client_len;
	char n;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;

	//remove existing socket and create new non-name sockets
	unlink("server_socket");
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

	//naming socket
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "server_socket");
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	//bind함수는 소켓에 IP주소와 포트번호를 지정해줘서 소켓통신을 할 수 있도록 준비해줌!!


	while (1){



        char ch;
        int val;


         //connecting accept
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
        //waiting client
	    listen(server_sockfd, 5);
	    printf("서버 구동중...\n");
	    char str[15];
        read(client_sockfd, &str, 50);
        printf("%s", str);

        read(client_sockfd, &ch, 1);

        val = (int)ch - 48; //아스키코드 48빼서 숫자로 형변환

        switch(val){
        case 1:{
        query_stat = mysql_query(connection, "select * from lib order by book asc");//오름차순으로정렬함

	sql_result = mysql_store_result(connection);

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL){
        booktype.book=sql_row[0];
        booktype.author=sql_row[1];
        booktype.publisher=sql_row[2];
        booktype.year=sql_row[4];
        booktype.avail=sql_row[5];
        wirte(client_sockfd, &booktype, sizeof(struct booktype));
	}

        break;

        }
        case 2:{


        char *ch;
		int len=0, tmp;
		read(client_sockfd, &len, 1);//책이름 길이 읽기 1
		ch = (char *)malloc(sizeof(len));
		read (client_sockfd, ch, len);//책이름 읽기2
		printf("%d", len);
		printf("%s\n", ch);
		sprintf(query, "update lib set avail = 'n' where book = '%s'", ch);
		query_stat = mysql_query(connection, query);
		sql_result = mysql_store_result(connection);
		if (sql_result != NULL){//성공시
			tmp = 1;
			write(client_sockfd, &tmp, 1);
		}
		else{//실패시
			tmp = 0;
			write(client_sockfd, &tmp, 1);
		}
        close(client_sockfd);
		free(ch);
		break;
        }
        case 3:{
        }
        case 4:{
        }
        case 5:{
            printf("client가 종료되었습니다.\n");
        }
        }


	}

		close(client_sockfd);


}
