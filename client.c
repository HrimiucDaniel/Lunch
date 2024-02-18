#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <time.h>

extern int errno;

int port;

int main (int argc, char *argv[])
{
  int sd, nr, timer;    
  struct sockaddr_in server;  
  char buf[10];

  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  port = atoi (argv[2]);

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[client]:Eroare la socket().\n");
      return errno;
    }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons (port);
  
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

    
  printf ("Ma decid asupra comenzii...\n");
  fflush (stdout);
 int notok = 0;
  srand(time(NULL));
  while (1) 
  {
      if (notok <= 3) {

      nr = (rand()%(5-1+1))+1;
      timer = (rand()%(10-1+1))+1;
  
      printf("M-am decis, as vrea mancarea cu numarul %d\n",nr);
      fflush(stdout);


      if (write (sd,&nr,sizeof(int)) <= 0)
      {
        perror ("[client]Eroare la write() spre server.\n");
        return errno;
      }
      char msg[25];
      bzero(msg, 25);
      if (read (sd,  msg,sizeof(msg)) < 0)
      {
     
        perror ("[client]Eroare la read() de la server.\n");
        return errno;
      }
      printf ("[Restaurant]%s\n", msg);
      fflush(stdout);
      if (strcmp(msg,"Indisponibil")==0) {printf("Bine!Comand altceva!\n");notok++;sleep(timer);}
      else {printf("Satul!\n");break;}
    }else{
      printf("Schimb cantina!Mor de foame!!");
      break;
    }
  }
  close (sd);
}