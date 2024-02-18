 #include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <chrono>
#include <iostream>

#define PORT 2025
int yz;
int okk = 0;
extern int errno;
pthread_mutex_t lock;
typedef struct thData{
	int idThread;
	int cl; 
}thData;
void afiseaza();
static void *treat(void *); 
void raspunde(void *);
int maxim();
void intializare();
int nr[5];
int clienti[100];

int main ()
{
  struct sockaddr_in server;
  struct sockaddr_in client;	
  int nr, sd, i = 0;
  char msg[100];
  pthread_t th[100];
  
 
 while(1) {
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }

  int on=1;
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  
  bzero (&server, sizeof (server));
  bzero (&client, sizeof (client));
  
  server.sin_family = AF_INET;	
  server.sin_addr.s_addr = htonl (INADDR_ANY);
  server.sin_port = htons (PORT);
  
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  if (listen (sd, 10) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }
    if (pthread_mutex_init(&lock, NULL)){
   	perror("[server] mutex intializare error]\n");
   	return errno;
    }
  std::chrono::steady_clock::time_point start =         std::chrono::steady_clock::now();
  if (okk == 0) {
	  printf("Restaurantul s-a deschis!!! Cereti comanda la portul %d...\n", PORT);
	  fflush(stdout); sleep(20);
  }
  while (1)
    {
        okk = 1;
      int client;
      thData * td;  
      socklen_t length = sizeof (client);

     

      if ( (client = accept (sd, (struct sockaddr *) &client, &length)) < 0)
	    {
	     perror ("[server]Eroare la accept().\n");
	     continue;
	    }
	 
	 td = (struct thData*) malloc(sizeof(struct thData));	
	 td->idThread = i++;
	 td->cl = client;
	 
	 pthread_create(&th[i], NULL, &treat, td);	
	 
	
	if (std::chrono::steady_clock::now() - start > std::chrono::seconds(30))           break;
				
	} 

	close(sd); 
	intializare();
	
	pthread_mutex_destroy(&lock);   
  }
};				
static void *treat(void * arg)
{		

	struct thData tdL; 
	tdL= *((struct thData*)arg);	
	
	printf ("Clientul nr - %d - Bine ai venit\n", tdL.idThread);
	fflush (stdout);		 
	
	pthread_detach(pthread_self());	
	sleep(5);
	
	raspunde((struct thData*)arg);
	
	close ((intptr_t)arg);
	return(NULL);	
  		
};
void intializare(){
for (int i = 1; i<=5; i++)
	nr[i]=0;
}
int maxim() {
	int maxim = 0;
	for (int i = 1; i<=5; i++)
		if (nr[i] > maxim) maxim = nr[i];
	return maxim;
}
void afiseaza(){
	for (int i = 1; i<=5; i++)
	printf("Elementul [%d] apare de %d: \n",i,nr[i]);

}
void raspunde(void *arg)
{

	int ok = 0;
	while (ok != 1){
	sleep(5);
	if (ok == 2){ intializare();ok = 0;}
    int x, i = 0;
	struct thData tdL; 
	tdL= *((struct thData*)arg);
	
	if (read (tdL.cl, &x,sizeof(int)) <= 0)
		{
			printf("[Thread %d]\n",tdL.idThread);
			perror ("Eroare la read() de la client.\n");
			
		}
	
	printf ("Clientul cu nr %d - comanda ta a fost receptionata: %d\n",tdL.idThread, x);
	pthread_mutex_lock(&lock);
	nr[x]++;

	pthread_mutex_unlock(&lock);
	int max;
  

    
 ;
  sleep(5);
   max = maxim();
	printf("Raspuns trimis catre clientul %d pentru comanda %d\n",tdL.idThread, x);
		      
	char rasp[25];
	bzero(rasp, 25);


 
	if (nr[x] == max) 
		{strcpy(rasp, "Masa e servita");ok = 1;}
		else
		{strcpy(rasp, "Indisponibil");ok = 2;}
		
	if (write (tdL.cl, rasp, sizeof(rasp)) <= 0)
			{
		 		printf("[Thread %d] ",tdL.idThread);
		 		perror ("[Thread]Eroare la write() catre client.\n");
			}
	fflush(stdout);
	
  
	}
	sleep(5);
	yz++;
	
}
