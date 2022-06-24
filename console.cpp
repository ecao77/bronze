#include <stdlib.h>     /* srand, rand functions for random numbers*/
#define ROW_1 2
#define ROW_2 3
#define ROW_3 4
#define ROW_4 5
#define ROW_5 6
#define ROW_6 7
#define ROW_7 8
#define ROW_8 9

#define COL_1 10
#define COL_2 11
#define COL_3 12
#define COL_4 13
#define COL_5 A0
#define COL_6 A1
#define COL_7 A2
#define COL_8 A3 
//define rows and columns as pins for 8x8 LED matrix

void(* resetFunc) (void) = 0;  // declare reset fuction at address 0 --- this is for soft reset of the console

int VRx = A4;
int VRy = A5;//init joystick pins

bool started = false;//menu state variable

int buzzerPin = 1;//init buzzer pin

int LR() { //function for joystick reading 1 for RIGHT, -1 for LEFT, 0 for none
  if(analogRead(VRy)/507 == 2) { //RIGHT
    return 1;
  }
  if(analogRead(VRy)/507 == 0) { //LEFT
    return -1;
  }
  return 0;//none
}
void deathSound(){
//this function plays the popular death sound from mario
int tempo = 38;//tempo to 38ms per 64th note
int notes[13] = {494, 0, 699, 0, 699, 699, 659,  587, 523, 330, 196,330, 262};//list of notes(frequency Hz)
int times[13] = {4, 1, 3, 4, 4, 8,8,8,4,4,4,4,16};// 1/64 note = 1 -- length of note
for(int i=0; i<13; i++){//loop through arrays
  tone(buzzerPin,notes[i],times[i]*tempo);//play given note and given tempo
  delay(times[i]*tempo+50);//50ms delay between each note for articulation
}

}
int getDirection(int currentDirection) { //function for joystick direction in snake 1 for FORWARD, 3 FOR BACKWARD, 0 for RIGHT, 2 for LEFT
  if(analogRead(VRx)/507 == 2 && currentDirection!=3) { //FORWARD unless currently going BACKWARD
    return 1;
  }
  if(analogRead(VRx)/507 == 0 && currentDirection!=1) { //BACKWARD unless currently going FORWARD
    return 3;
  }
    if(analogRead(VRy)/507 == 2 && currentDirection!=2) { //RIGHT unless currently going LEFT
    return 0;
  }
  if(analogRead(VRy)/507 == 0 && currentDirection!=0) { //LEFT unless currently going RIGHT
    return 2;
  }
  return currentDirection;//if no direction change is detected
}
unsigned int bitsToBytes(unsigned char *bits)//function converts bits to bytes -- used to convert 2D array of bits into the 1D array of bytes that the LED matrix function reads
{
  unsigned int sum = 0;
for (int i = 0; i < 8; i++)
  {
   sum<<=1;
    sum += bits[i] - '0';
   
  }
  return sum;

}

  unsigned char bits[8][8] = {//initialize 2D array; shows a smile
   {'1','1','1','1','1','1','1','1'}, 
   {'1','1','1','1','1','1','1','1'}, 
   {'1','1','0','1','1','0','1','1'},
   {'1','1','1','1','1','1','1','1'}, 
   {'1','0','1','1','1','1','0','1'},   
   {'1','1','0','0','0','0','1','1'}, 
   {'1','1','1','1','1','1','1','1'}, 
   {'1','1','1','1','1','1','1','1'}, 
  };
byte board[8];//initialize led matrix input
class Bullet{//bullet class for asteroid game
public:
int x;
int y;
bool spawn = true;//initialize bullet attributes
void fall(){//fall method called in update -- lowers y position by 1
y--;
if(y==-1){
spawn = false;//stop spawning bullet when leaves screen
}
}
};
class Enemy{//enemy class for asteroid game
public:
bool endGame = false;
int x = (rand() % 6)+1;//random number 1 to 6
int y = 0;//sky(highest point)
void fall(){//fall method called in update -- lowers y position by 1
y++;
if(y==8){//if player loses(when asteroid hits bottom)
  deathSound();
resetFunc(); //call reset
}
}
void die(){//die method for enemy
x = (rand() % 6)+1;//random number 1 to 6
y = 0;
tone(buzzerPin, 300, 500);//explosion sound effect
}
};
class Asteroid{//main asteroid class
public:
int x = 3;//0 most left 5 most right
bool toggle = true;
Bullet bullets[20] = {};//20 bullets stored in memory at time
Enemy enemy;
int bulletIndex = 0;
int bulletLen = 0;//initialize game attributes
void shoot(){//shoot method
tone(buzzerPin, 500, 250);//sound effect
Bullet bullet;
bullet.x = x+1;
bullet.y = 5;//spawn bullet
bullets[bulletIndex] = bullet;
bulletIndex++;
bulletLen++;//store bullet in game bullet array
if(bulletLen == 19){
  bulletLen = 0;
}
if(bulletIndex == 19){
bulletIndex = 0;
}//when bullet list is filled --- resed index counter
};
void update(){//update method
  if(toggle){
  enemy.fall();//asteroid falls every other update call

  toggle = false;
  }
  else{
    toggle = true;
  }
  x+=LR();//update player position by joystick input
  if(x==6){//clamp x values
    x=5;
  }
  if(x==-1){
    x=0;
  }
        for(int i=0; i<8;i++){
        for(int j=0; j<8; j++){
          bits[i][j] = '1';//set every bit to 1(off); reset board
        }
      } 
      if(!enemy.endGame){//if game is still going
      bits[7][x] = '0';
      bits[7][x+1] = '0';
      bits[7][x+2] = '0';
      bits[6][x+1] = '0';//player character
      
      bits[enemy.y][enemy.x] = '0';//enemy
      
      for(int i=0; i<bulletLen; i++){//loop through bullets
      if(bullets[i].spawn){//if the bullet should be spawned
     
      if((bullets[i].y-1==enemy.y && enemy.x==bullets[i].x)||(bullets[i].y==enemy.y && bullets[i].x==enemy.x)){//test for collision with bullet
        enemy.die();
        bullets[i].spawn = false;//destroy enemy and bullet
      }

      bits[bullets[i].y][bullets[i].x] = '0'; //draw bullet
      
      }
      }
      } 
            for(int i=0; i<8;i++){//set led matrix input to the bit 2D array
      board[i] = bitsToBytes(bits[i]);
    } 
}
};
Asteroid asteroid;//initialize asteroid game
class Apple{//apple class
public:
int x=rand() % 8;//random x and y values for random location on led matrix
int y = rand() % 8;
void updatePos(){//update position method
x=rand() % 8;
y = rand() % 8;//new random x and y values for random location on led matrix
}
};
Apple apple;//initialize apple object
class Snake {       // The class for snake game
  public:            
    int d=2;
        /*
    0: right
    1: top
    2: left
    3: down
    (rotates counterclockwise like increasing angles)
    direction * pi/2 = angle in radians
    */
    int len = 4;
    bool alive = true;//initialize attributes
    int body[64][2]={{3,4},{3,5},{4,5},{4,6}}; //initalize starting body array
    void update(){//update Method
      int head[2];//y,x
      switch(d){//new head
      case 0:
//right
      head[0]=body[0][0];
      head[1]=body[0][1]+1;//add one to x position of current head
      break;
      case 1:
//up
head[0]=body[0][0]-1;//subtract one from current y position of current head
head[1]=body[0][1];
      break;
      case 2:
      //left
    head[1]= body[0][1]-1;//subtract one from current x position of current head
    head[0]= body[0][0];
      break;
      case 3:
      //down
     head[0] = body[0][0]+1;//ad one to y position of current head
     head[1] = body[0][1];
      break;
    }
    //if collision detected with board edge
    if(head[0]<0||head[0]>7||head[1]<0||head[1]>7){
     alive = false;
    }
    for(int i=0; i<len-1; i++){//loop through body to check collision
      if(head[0]==body[i][0]&&head[1]==body[i][1]){//detect collision between head and body
        alive=false;//kill snake
        break;//exit loop
      }
    }
    
    if(alive){
  
    if(head[0]==apple.y && head[1]==apple.x){//apple collision check
      tone(buzzerPin, 659, 500); //signal apple eat with noise(E5 for half a second)
      len++;
      int tempBody[len][2];//new body array to grow the array
      for(int i=0; i<len; i++){
        tempBody[i][0] = body[i][0];
        tempBody[i][1] = body[i][1];//copy array to new body array
      }
      for(int i=1; i<len; i++){
        body[i][0] = tempBody[i-1][0];
        body[i][1] = tempBody[i-1][1];//copy every element except for tail -- leave head open
      }
      body[0][0] = head[0];
      body[0][1] = head[1];//set new calculated head to real head
      apple.updatePos();//recalculate apple position
    }
    else{
    //copy snake excluding tail and leave head open
    for(int i=len-2; i>=0; i--){
body[i+1][0] = body[i][0];
body[i+1][1] = body[i][1];
    }
    body[0][0] = head[0];
    body[0][1] = head[1];//recalculate new head
    }
      for(int i=0; i<8;i++){
        for(int j=0; j<8; j++){
          bits[i][j] = '1';//set every bit to 1(off) to reset board
        }
      }   
      for(int i=0; i<len; i++){
        bits[body[i][0]][body[i][1]] = '0';//set each body pixel to 0(on)
      }
      //set apple pixel to on
      bits[apple.y][apple.x] ='0';
            for(int i=0; i<8;i++){
      board[i] = bitsToBytes(bits[i]);//set led matrix input to set 2D array of bits
    }
    }
    }
    
};

Snake snake;//initalize snake game object

const byte rows[] = {
    ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8
};
const byte col[] = {
  COL_1,COL_2, COL_3, COL_4, COL_5, COL_6, COL_7, COL_8
};//initialize pins in arrays for led matrix

void setup() 
{
  pinMode(0, INPUT);//button pin(input)
        tone(buzzerPin, 262, 750);
        delay(750);
        tone(buzzerPin, 330, 750);
        delay(750);
        tone(buzzerPin, 392, 750);
        delay(750);
        tone(buzzerPin, 262, 250);
        delay(250);
        tone(buzzerPin, 330, 250);
        delay(250);
        tone(buzzerPin, 392, 1500);//startup music--- c major chord repeated at different speeds
        delay(1500);

    
    //setup board
    for(int i=0; i<8;i++){
      board[i] = bitsToBytes(bits[i]);
    }
    // Set all used pins to OUTPUT
    // This is very important! If the pins are set to input
    // the display will be very dim.
    for (byte i = 2; i <= 13; i++)
        pinMode(i, OUTPUT);
    pinMode(A0, OUTPUT); pinMode(A1, OUTPUT); pinMode(A2, OUTPUT); pinMode(A3, OUTPUT);
}
bool hasRun = false;
bool dead = false;
bool snakeGame = false;
int buttonTime = 0;
int game = 0;//set more global variables used in void loop
void loop() {//void loop run on loop in arduino
  bool buttonPressed = digitalRead(0);//button state update
  if(started){
    int t = millis();//set t to milliseconds
if(buttonPressed){
  if(t-buttonTime>350){//prevent holding of button(only accept button presses with a 350ms gap between them
  buttonPressed = false;
  }
}
else{
  buttonTime = t;
}
  if(snakeGame){//if snake game is selected
 if(snake.alive){
  snake.d=getDirection(snake.d);//update snake direction
 if(t%400==0){//every tick(400ms)
  if(!hasRun){
snake.update();//call update snake method
hasRun=true;
  }
  }
  else{
hasRun = false;
  }
  }
  else if(!dead){//when snake is dead turn every pixel off
    deathSound();
    dead = true;
    resetFunc(); //call reset
       for(int i=0; i<8;i++){
        for(int j=0; j<8; j++){
          bits[i][j] = '1';//set every bit to 1(off)
        }
      }
      for(int i=0; i<8;i++){
      board[i] = bitsToBytes(bits[i]);//change input to led matrix input
    }   
  }
  }
  else{//asteroid game
    if(t%400==0){//every tick(400ms)
    if(buttonPressed){
    asteroid.shoot();//when button pressed call asteroid shoot method
    }
    buttonPressed=false;
    asteroid.update();//update method
          for(int i=0; i<asteroid.bulletIndex; i++){ 
      asteroid.bullets[i].fall();//have each bullet fall in bullet array --- called outside of update so asteroid and bullet do not swap positions
      }
    }
  }
drawScreen(board);//update board
}
else{
    game+=LR();//when menu is shown change game icon when joystick movement is detected
if(game==-1){//work in other direction too
  game =1;
}
else if(game==2){//work in other direction too
  game = 0;
}
if(game==0){//change icon and game value
snakeGame = false;
unsigned char icon[8][8] = {//asteroids icon
   {'1','1','0','0','1','1','1','1'}, 
   {'1','1','0','0','1','1','1','1'}, 
   {'1','1','1','1','1','1','1','1'},
   {'1','1','1','0','0','1','1','1'}, 
   {'1','1','1','0','0','1','1','1'},   
   {'1','1','0','0','0','0','1','1'}, 
   {'1','1','0','1','1','0','1','1'}, 
   {'1','1','0','0','0','0','1','1'} 

};
      for(int i=0; i<8;i++){
      board[i] = bitsToBytes(icon[i]);//update board inputs
    }  
}
else if(game==1){//change icon and game value
snakeGame = true;
unsigned char icon[8][8] = {//snake game icon
   {'1','1','1','1','1','1','1','1'}, 
   {'1','0','0','0','0','0','0','1'}, 
   {'1','0','1','1','1','1','0','1'},
   {'1','0','1','0','0','0','0','1'}, 
   {'0','0','1','0','1','1','1','1'},   
   {'1','1','1','0','1','0','0','0'}, 
   {'0','0','0','0','1','0','1','0'}, 
   {'1','1','1','1','1','0','0','0'}
};
      for(int i=0; i<8;i++){
      board[i] = bitsToBytes(icon[i]);//update board inputs
    }  
}
if(buttonPressed){
  started = true;//start when button is pressed
}
    drawScreen(board);//update board
}
}

 void  drawScreen(byte buffer2[])//this function draws a 1D array of bytes on led matrix
 { 
   // Turn on each row in series
    for (byte i = 0; i < 8; i++)        // count next row
     {
        digitalWrite(rows[i], HIGH);    //initiate whole row
        for (byte a = 0; a < 8; a++)    // count next row
        {
          // if You set (~buffer2[i] >> a) then You will have positive
          digitalWrite(col[a], (buffer2[i] >> a) & 0x01); // initiate whole column
          
          digitalWrite(col[a], 1);      // reset whole column
        }
        digitalWrite(rows[i], LOW);     // reset whole row
        // otherwise last row will intersect with next row
    }
}