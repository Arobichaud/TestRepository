#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUFFER 17
#define AMOUNTBUFFER 5
//Affiche str et demande un nombre entre min et max!
int getNbFromUser(char* str, int min, int max);
void transaction(int accountNo, int operation);
int getAmountFromAccount(int file);
int doTransactionAndValidation(int* amountFromAccount, int amountForTransaction);
void updateAccountInformation(int file, int finalAmount);

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
  int amountForTransaction = 0;
  int amountFromAccount = 0;
  
  if(operation == 1 || operation == 2)
  {
    amountForTransaction = getNbFromUser("De quel montant entier?\n", 0,9999);
    file = open("accounts.txt", O_RDWR);
    lseek(file, (BUFFER+1)*accountNo, SEEK_SET);
    if(lockf(file, F_TLOCK, BUFFER) == -1)
    {
      printf("Erreur compte no %d deja en utilisation\nTRANSACTION ANNULEE\n", accountNo);
      close(file);
      kill(getpid(), SIGTERM);
    }
    amountFromAccount++;
    if (operation == 1)
    {
      amountForTransaction *= -1;
    }

    amountFromAccount = getAmountFromAccount(file);
   
    if (doTransactionAndValidation(&amountFromAccount, amountForTransaction))
    {
      printf("Valide\n");
      updateAccountInformation(file, amountFromAccount);
    }
    else
    {
      printf("Invalide - Transaction annulee\n");
    }
    

    //TODO updateAccountInformation    
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

int getAmountFromAccount (int file)
{
  char amountString[AMOUNTBUFFER];
  
  lseek(file, (BUFFER-AMOUNTBUFFER), SEEK_CUR);
  read(file, amountString, AMOUNTBUFFER);
  amountString[AMOUNTBUFFER] = 0;

  return atoi(amountString);
}

int doTransactionAndValidation(int* amountFromAccount, int amountForTransaction)
{
  *amountFromAccount += amountForTransaction;
  
  if (*amountFromAccount < 0 || *amountFromAccount > 9999)
  {
    return 0;
  }

  return 1;
}

void updateAccountInformation(int file, int finalAmount)
{
  char finalAmountString[AMOUNTBUFFER];
  sprintf(finalAmountString, "%4d", finalAmount);
  lseek(file, -AMOUNTBUFFER, SEEK_CUR);
  write(file, finalAmountString, (AMOUNTBUFFER-1));
}

int getNbFromUser (char* str, int min, int max)
{
  int nb = 0;
  do
  {
    write(1, str, strlen(str));  
    scanf("%d", &nb);
  }while(nb < min || nb > max);
  return nb;
}
