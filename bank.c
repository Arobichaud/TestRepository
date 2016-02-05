#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFER 17

//Affiche str et demande un nombre entre min et maxasdad
int getNbFromUser(char* str, int min, int max);
void transaction(int accountNo, int operation, int amount);

int main(int argc, char** argv)
{
  int accountNo;
  int operation;
  int amount;

  while(getNbFromUser("Effectuer une transaction ?\n0 = Exit\n1 = Faire Transaction\n",0,1))
  {
    accountNo = getNbFromUser("Sur quel compte ?\n0\n1\n2\n3\n4\n", 0,4);
    operation = getNbFromUser("Options:\n0 = Consulter\n1 = Retrait\n2 = Depot\n", 0,2);
    amount = getNbFromUser("De quel montant ? (max 1000$)\n", 0,1000);

    //Processus enfant execute la commande
    if(!fork())
    {
      transaction(accountNo, operation, amount);
    }
  }

  while(wait(NULL) > 0);
  return 0;
}

void transaction(int accountNo, int operation, int amount)
{
  int file;
  lseek(file, 17*accountNo, SEEK_SET);
  
  if(operation == 1 || operation == 2)
  {
   file = open("accounts.txt", O_RDWR);
   if(lockf(file, F_TLOCK, 17) == -1)
    {
      printf("Erreur compte no %d deja en utilisation\nTRANSACTION ANNULEE\n", accountNo);
      close(file);
      kill(getpid(), SIGTERM);
    }

    //TODO RETRAIT OU DEPOT MONTANT


    sleep(10);
    lockf(file, F_ULOCK, BUFFER);

  }
  else
  {
    char buffer[17];
    file = open("accounts.txt", O_RDONLY;
    read(file, buffer, 16); 
    buffer[17] = 0;
    printf("%s\n", buffer);
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
