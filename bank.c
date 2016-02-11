#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUFFER 17

//Affiche str et demande un nombre entre min et max!
int getNbFromUser(char* str, int min, int max);
void transaction(int accountNo, int operation);

int main(int argc, char** argv)
{
  int accountNo;
  int operation;

  while(getNbFromUser("Effectuer une transaction ?\n0 = Exit\n1 = Faire Transaction\n",0,1))
  {
    //TODO: afficher la liste des comptes.
    accountNo = getNbFromUser("Sur quel compte ?\n0\n1\n2\n3\n4\n", 0,4);
    operation = getNbFromUser("Options:\n0 = Consulter\n1 = Retrait\n2 = Depot\n", 0,2);

    //Processus enfant execute la commande
    if(!fork())
    {
      transaction(accountNo, operation);
    }
    wait(NULL);
  }

  return 0;
}

void transaction(int accountNo, int operation)
{
  int file;
  int amount = 0;
  
  if(operation == 1 || operation == 2)
  {
    amount = getNbFromUser("De quel montant entier?\n", 0,9999);
    file = open("accounts.txt", O_RDWR);
    lseek(file, BUFFER*accountNo, SEEK_SET);
    if(lockf(file, F_TLOCK, BUFFER) == -1)
    {
      printf("Erreur compte no %d deja en utilisation\nTRANSACTION ANNULEE\n", accountNo);
      close(file);
      kill(getpid(), SIGTERM);
    }
    amount++;
    //TODO RETRAIT OU DEPOT MONTANT
   
   lockf(file, F_ULOCK, BUFFER);
  }
  else
  {
    char stringBuffer[BUFFER];
    file = open("accounts.txt", O_RDONLY);
    lseek(file, (BUFFER+1)*accountNo, SEEK_SET);
    read(file, stringBuffer, BUFFER); 
    stringBuffer[BUFFER] = 0;
    printf("%s\n", stringBuffer);
  }

  close(file);
  //Kill le processus apres execution de transaction
  kill(getpid(), SIGTERM);      
}

int getNbFromUser(char* str, int min, int max)
{
  int nb = 0;
  do
  {
    write(1, str, strlen(str));  
    scanf("%d", &nb);
  }while(nb < min || nb > max);
  return nb;
}
