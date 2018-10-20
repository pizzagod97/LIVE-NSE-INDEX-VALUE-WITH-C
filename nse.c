

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<pthread.h>
#include <openssl/ssl.h>

char key[100000];
char *key_word = "nseindex";
int kill=-1;
void * key_search()
{
int delim=1;
for(int i=0;i<strlen(key);i++)
if(key[i]==key_word[0])
{
int z=0;
for(;z<strlen(key_word);z++)
{
if(key[i+z]!=key_word[z])
break;
}

if(z==strlen(key_word))
{
//puts(key);
char buf[20];
int op=0;
while(key[i++]!='>');
for(int l=i;key[l]!='<';l++)
buf[op++]=key[l];
buf[op]='\0';
system("date");
printf("%s : ",key_word);
printf("%s 		",buf);
kill++;
}
}


}



int main(int argc, char** argv)
{
	

	pthread_t th;
	pthread_t tk;
	struct sockaddr_in servaddr;
	struct hostent *hp;
	int sock_id;
	char request[] = "GET /index.html HTTP/1.1\r\n"
		"Host: money.rediff.com \r\n"
		"User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:61.0) Gecko/20100101 Firefox/61.0 \r\n"
		"\r\n";
               

         

          sock_id = socket(AF_INET,SOCK_STREAM,0);
	
	memset(&servaddr,0,sizeof(servaddr));
	
	
	hp = gethostbyname("money.rediff.com");
		
	memcpy((char *)&servaddr.sin_addr.s_addr, (char *)hp->h_addr, hp->h_length);
	
	
	servaddr.sin_port = htons(443);
	servaddr.sin_family = AF_INET;
// first connect to the remote as usual, but use the port 443 instead of 80

// initialize OpenSSL - do this once and stash ssl_ctx in a global var
SSL_load_error_strings ();
SSL_library_init ();
SSL_CTX *ssl_ctx = SSL_CTX_new (SSLv23_client_method ());

// create an SSL connection and attach it to the socket
SSL *conn = SSL_new(ssl_ctx);
SSL_set_fd(conn, sock_id);


// now proceed with HTTP traffic, using SSL_read instead of recv() and
// SSL_write instead of send(), and SSL_shutdown/SSL_free before close()	
	
	connect(sock_id, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	// perform the SSL/TLS handshake with the server - when on the
// server side, this would use SSL_accept()
int err = SSL_connect(conn);
if (err != 1)
   abort(); // handle error
	
SSL_write(conn,request,sizeof(request));
	int stat = 0;
	char buf;
	int count =0;
	int f=0;
	char *str="data-currency-value";
	while(1)
	{
	SSL_read(conn,&buf,1);
	//putchar(buf);
        key[f++] = buf;	
       if((buf==13) || (buf=='\n'))
       {
       key[f]='\0';
       f=0;
       pthread_create(&tk,NULL,(void *)&key_search,NULL);     
       pthread_join(th,NULL);
 //      puts("recieved thread");
 if(kill==0)
	 exit(0);
       }	       
	if(buf=='\n')
		count++;
	else
		count=0;
	if(count==10)
	{
		exit(0);
		
	}
	}

	return 0;


}
