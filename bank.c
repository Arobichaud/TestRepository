#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

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
    accountNo = getNbFromUser("Sur quel compte ?\n1\n2\n3\n4\n5\n", 1,5);
    operation = getNbFromUser("Options:\n0 = Retrait\n1 = Depot\n", 0,1);
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
  //TODO LOCK LIGNE ACCOUNT
  
  //faire modif

  //WAIT x temp pour faire un overlap entre fonctions
  sleep(10);

  //TODO UNLOCK LIGNE ACCOUNT
  sleep(1);
  //Kill le processus apres execution de transaction
  write(1,"Child dead\n", 11);
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
