#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <term.h>
#include <curses.h>
#include <unistd.h>

static struct termios initial_settings, new_settings;
static int peek_character = -1;

void init_keyboard();
void close_keyboard();

int kbhit();
int readch();

int main()
{
  int ch = 0;
  
  init_keyboard();
  while(ch != '1'){
    printf("looping\n");
    sleep(1);
    if(kbhit()){
      ch=readch();
      printf("Se ha pulsado %c\n",ch);
    }
  }
  close_keyboard();
  return 0;
}

void init_keyboard()//Configuracion de la terminarl al principo del programa
{
  tcgetattr(0, &initial_settings);
  new_settings = initial_settings;
  new_settings.c_lflag &= ~ICANON;
  new_settings.c_lflag &= ~ECHO;
  new_settings.c_lflag &= ~ISIG;
  new_settings.c_cc[VMIN] =1;
  new_settings.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &new_settings);  
}
void close_keyboard()//Configuracion de la terminal al final del programa
{
  tcsetattr(0, TCSANOW, &initial_settings);
}

int kbhit() //COmprobacion de tecla presionada
{
  char ch;
  int nread;
  
  if(peek_character != -1)
    return 1;
  new_settings.c_cc[VMIN] = 0;
  tcsetattr(0, TCSANOW, &new_settings);
  nread = read(0, &ch,1);
  tcsetattr(0, TCSANOW, &new_settings);
  if (nread == 1){
    peek_character = ch;
    return 1;
  }
  return 0;
}

int readch()//Lee el caracter presionado
{
  char ch;
  
  if(peek_character != 1){
    ch = peek_character;
    peek_character = -1;
    return ch;
   }
   read(0,&ch,1);
   return ch;
}