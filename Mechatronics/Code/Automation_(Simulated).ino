//In serial grid:
//0 - unchecked
//1 - clear
//2 - box (unchecked)
//3 - box (no light)
//4 - box (light off)
//5 - box (being checked)
//6 - path check
//8 - box placeholder
//9 - car

char carOrientation='N'; //intial orientation of the car is NSEW cardinal directions, with N being away from the starting side
int xStart=7; //Hard coded start position of car; bottom left corner (0,0)
int yStart=0;



int xPos=xStart; //intializes car position
int yPos=yStart;
int layout[8][8]={ //initializes simulated grid
  {1, 1, 1, 1, 1, 1, 1, 9,},
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 2, 0,},
  {0, 0, 2, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 2, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 2, 0, 2, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,}
};
int history[][2]={{xStart,yStart}}; //initializes the "memory" of the path the car has taken
bool onStart=true; //intializes that car begins by traveling to (0,0)
bool checkBox=false; //initializes that car is not actively searching a box
int step=0; //initializes that no actions have occured yet
bool record=true; //initializes that car will record its path
int checksides[]={0,0,0,0}; //initializes that the satate of no sides of a box are known
int boxx; //initializes searched box x and y pos as global variables
int boxy;

void setup() {
  Serial.begin(9600); //simulation is printed to serial
}

void loop() {
  record=true;//in each loop, the car should default to recording its actions
  if(onStart){ //if in the starting phase, traveling to the (0,0) square, move left until the (0,0) square is reached, then end the starting phase
    if(layout[0][0]==9){
      onStart=false;
    } else {
      moveleft();
    }
  } else { //if not in the starting phase:
    if (!checkBox){ //if not checking a box
      if (lookforbox()){ //look for a box in the adjacent grid cells using the distance sensors, and if found, begin searching that box, and determine which sides of the box are accessible to be searched
        checkBox=true;
        boxaccess();
      } else { //if no box is found:
        if (leftclear()){ //if the car can move left (West), and has not already checked that tile, do so
          moveleft();
        } else if(!leftclear()&&upclear()){ //if the car can move up (North), and has not already checked that tile, do so
          moveup();
        } else if(!leftclear()&&!upclear()&&downclear()){ //if the car can move down (South), and has not already checked that tile, do so
          movedown();
        } else if (!leftclear()&&!upclear()&&!downclear()&&rightclear()){ //if the car can move right (East), and has not already checked that tile, do so
          moveright();
        } else if (!leftclear()&&!upclear()&&!downclear()&&!rightclear()){ //if the car has already cheked every adjacent tile:
          record=false; //do not record this movement
          step--; //reduce the record entry number by one (like going back in time/undoing)
          int deltax=xPos-history[step][0];//see if the car moved horizontally to get to the current grid tile (-1 -> entered from East, 1 -> entered from West)
          int deltay=yPos-history[step][1];//see if the car moved vertically to get to the current grid tile (-1 -> entered from South, 1 -> entered from North)
          if (deltax==1){//if entered from the west, move west
            moveleft();
          }
          if (deltax==-1){//if entered from the east, move east
            moveright();
          }
          if (deltay==1){//if entered from the south, move south
            movedown();
          }
          if (deltay==-1){//if entered from the north, move north
            moveup();
          }
        }
      }
    } else { //if checking a box:
      int dstFromBoxx=xPos-boxx; //determine distance from the box
      int dstFromBoxy=yPos-boxy;
      if (dstFromBoxx==1&&dstFromBoxy==0&&checksides[2]==0){//if the car is on the east side of the box, scan for IR while facing west (towards the box), and record if it is lit or not
        ifrW();                                                                                           //scan ifrs
      }
      if (dstFromBoxx==-1&&dstFromBoxy==0&&checksides[3]==0){//if the car is on the west side of the box, scan for IR while facing east (towards the box), and record if it is lit or not
        ifrE();
      }
      if (dstFromBoxx==0&&dstFromBoxy==1&&checksides[0]==0){//if the car is on the north side of the box, scan for IR while facing south (towards the box), and record if it is lit or not
        ifrS();                                                                                           //scan ifrs
      }
      if (dstFromBoxx==0&&dstFromBoxy==-1&&checksides[1]==0){//if the car is on the south side of the box, scan for IR while facing north (towards the box), and record if it is lit or not
        ifrN();
      }
      if (leftsix()){//if there is a path check grid to west, move west
          moveleft();
      } else if(!leftsix()&&upsix()){//if there is a path check grid to the north, move north
        moveup();
      } else if(!leftsix()&&!upsix()&&downsix()){//if there is a path check grid to the south, move south
        movedown();
      } else if (!leftsix()&&!upsix()&&!downsix()&&rightsix()){//if there is a path check grid to the west, move west
        moveright();
      } else if (!leftsix()&&!upsix()&&!downsix()&&!rightsix()){//if there are no path check grids on any side, begin backtracking with the same process as seen at line 65
        record=false;
        step--;
        int deltax=xPos-history[step][0];
        int deltay=yPos-history[step][1];
        if (deltax==1){
          moveleft();
        }
        if (deltax==-1){
          moveright();
        }
        if (deltay==1){
          movedown();
        }
        if (deltay==-1){
          moveup();
        }
      }
      boxchecked();//check if the box has been fully checked, or if it should continue to be searched
    }
  }
  for(int i=7;i>=0;i--){//print out the current grid state, with (0,0) in the bottom left
    for (int j=0;j<7;j++){
      Serial.print(layout[i][j]);
      Serial.print(" ");
    }
    Serial.println(layout[i][7]);
  }
  Serial.print("  ");
  Serial.print(checkBox);//Print whether the car is actively searching a box for IR signals (0 = not searching, 1 = searching)
  Serial.print("  ");
  Serial.print(checksides[0]);//Print the checked-state of each side of the box corresponding to NSEW (0 = unchecked, 1 = checked - no IR, 5 = checked - IR detected)
  Serial.print(checksides[1]);
  Serial.print(checksides[2]);
  Serial.print(checksides[3]);
  Serial.print("  ");
  Serial.println(carOrientation);//Print the facing of the car
  if (record){//if the car is recording this step, increment the step and log the position of the car
    step++;
    history[step][0]=xPos;
    history[step][1]=yPos;
  }
  delay(1000);//delay 1s for loop to allow for readability

}


















void ifrN(){//Checks for IR to the north, if there is a signal, set the side's value to 5, otherwise set it to 1
  if (random(6)<5){ //SIMULATED CODE RANDOMIZES IF A SIDE IS EMITTING IR, REPLACE WITH IR SENSOR CONDITIONAL
    checksides[1]=1;
  } else {
    checksides[1]=5;
  }
}
void ifrS(){//Checks for IR to the south, if there is a signal, set the side's value to 5, otherwise set it to 1
  if (random(6)<5){ //SIMULATED CODE RANDOMIZES IF A SIDE IS EMITTING IR, REPLACE WITH IR SENSOR CONDITIONAL
    checksides[0]=1;
  } else {
    checksides[0]=5;
  }
}
void ifrE(){//Checks for IR to the east, if there is a signal, set the side's value to 5, otherwise set it to 1
  if (random(6)<5){ //SIMULATED CODE RANDOMIZES IF A SIDE IS EMITTING IR, REPLACE WITH IR SENSOR CONDITIONAL
    checksides[3]=1;
  } else {
    checksides[3]=5;
  }
}
void ifrW(){//Checks for IR to the west, if there is a signal, set the side's value to 5, otherwise set it to 1
  if (random(6)<5){ //SIMULATED CODE RANDOMIZES IF A SIDE IS EMITTING IR, REPLACE WITH IR SENSOR CONDITIONAL
    checksides[2]=1;
  } else {
    checksides[2]=5;
  } 
}

void boxchecked(){//checks if a box has been fully checked, either by determining that no sides are emitting, or by finding an emitting side
  int sum=0;
  for (int i=0;i<4;i++){
    sum=sum+checksides[i];//finds the total of the checksides array
  }
  if (sum>4){ //if an IR emitter has been found, stop searching the box, and lower the ladder
    checkBox=false;                                                                                          
    layout[boxy][boxx]=4;//mark the box as searched with IR turned off
    //INSERT CODE TO LOWER LADDER
  }
  if (sum==4){ //if all sides are not emitting, stop searching the box, and set all path tiles as unchecked
    checkBox=false;
    for (int i=0;i<8;i++){
      for (int j=0;j<8;j++){
        if (layout[i][j]==6){
          layout[i][j]=0;
        }
      }
    }
    layout[boxy][boxx]=3;//mark box on grid as searched with no IR
  }
}

void boxaccess(){//intializes inaccessible faces as checked with no signal
  for (int i=0;i<4;i++){
    checksides[i]=0;//NSEW faces: 0-checked 1-no signal 5-signal
  }
  if (boxx==0){//if the box is on the left edge of the grid, the west side is inaccessible
    checksides[3]=1;
  } else if(layout[boxy][boxx-1]>1){//if there is a box or vehicle occupying the space to the left of the box, the west side is inaccessible
    checksides[3]=1;
  }
  if (boxx==7){//if the box is on the right edge of the grid, the east side is inaccessible
    checksides[2]=1;
  } else if(layout[boxy][boxx+1]>1){//if there is a box or vehicle occupying the space to the right of the box, the east side is inaccessible
    checksides[2]=1;
  }
  if (boxy==0){//if the box is on the bottom edge of the grid, the south side is inaccessible
    checksides[1]=1;
  } else if(layout[boxy-1][boxx]>1){//if there is a box or vehicle occupying the space below the box, the south side is inaccessible
    checksides[1]=1;
  }
  if (boxy==7){//if the box is on the top edge of the grid, the north side is inaccessible
    checksides[0]=1;
  } else if(layout[boxy+1][boxx]>1){//if there is a box or vehicle occupying the space above box, the north side is inaccessible
    checksides[0]=1;
  }
  for (int i=0;i<3;i++){//loop through the grid tiles within one space of the box (corners inclusive)
    if (boxx+i-1>=0&&boxx+i-1<=7){
      for (int j=0;j<3;j++){
        if (boxy+j-1>=0&&boxy+j-1<=7){
          if (layout[boxy+j-1][boxx+i-1]<2){//if the tile is clear or unexplored, set it to a path tile
            layout[boxy+j-1][boxx+i-1]=6;
          } else if (layout[boxy+j-1][boxx+i-1]==2||layout[boxy+j-1][boxx+i-1]==3||layout[boxy+j-1][boxx+i-1]==4){//if the tile is another box of any type, set path tiles around that box
            goaround(boxx+i-1, boxy+j-1);
          }
        }
      }
    }
  }
  layout[boxy][boxx]=5;//identify the current box as being checked on the serial grid
}



void goaround(int x, int y){//creates a path around the called grid tile
  for (int i=0;i<3;i++){//loop through the grid tiles within one space of the inputted tile (corners inclusive)
    if (x+i-1>=0&&x+i-1<=7){
      for (int j=0;j<3;j++){
        if (y+j-1>=0&&y+j-1<=7){
          if (layout[y+j-1][x+i-1]<2){//if the tile is clear or unexplored, set it to a path tile
            layout[y+j-1][x+i-1]=6;
          }
        }
      }
    }
  } 
}


bool lookforbox(){//Uses the "radar" distance sensor to check the tiles one to the left, to the right, and in front of the car for boxes, and return true if a box is found, and false otherwise
  if (carOrientation=='N'){//if oriented north, check the NEW directions, and if a box is found, return true
    if (boxN()){
      return true;
    }
    if (boxE()){
      return true;
    }
    if (boxW()){
      return true;
    }
  }
  if (carOrientation=='S'){//if oriented south, check the SEW directions, and if a box is found, return true
    if (boxS()){
      return true;
    }
    if (boxE()){
      return true;
    }
    if (boxW()){
      return true;
    }
  }
  if (carOrientation=='E'){//if oriented east, check the NSW directions, and if a box is found, return true
    if (boxN()){
      return true;
    }
    if (boxS()){
      return true;
    }
    if (boxE()){
      return true;
    }
  }
  if (carOrientation=='W'){//if oriented west, check the NSE directions, and if a box is found, return true
    if (boxN()){
      return true;
    }
    if (boxS()){
      return true;
    }
    if (boxW()){
      return true;
    }
  }
  return false;//return false if no boxes are found
}

bool boxN(){//if a box is detected to the north, return true and assign its position, otherwise return false
  if (yPos<7){
    if (layout[yPos+1][xPos]==2){//REPLACE WITH DISTANCE SENSOR CONDITIONAL
      boxx=xPos;//set box's position
      boxy=yPos+1;
      return true;
    }
  }
  return false;
}
bool boxS(){//if a box is detected to the south, return true and assign its position, otherwise return false
  if (yPos>0){
    if (layout[yPos-1][xPos]==2){//REPLACE WITH DISTANCE SENSOR CONDITIONAL
      boxx=xPos;//set box's position
      boxy=yPos-1;
      return true;
    }
  }
  return false;
}
bool boxE(){//if a box is detected to the east, return true and assign its position, otherwise return false
  if (xPos<7){
    if (layout[yPos][xPos+1]==2){//REPLACE WITH DISTANCE SENSOR CONDITIONAL
      boxx=xPos+1;//set box's position
      boxy=yPos;
      return true;
    }
  }
  return false;
}
bool boxW(){//if a box is detected to the west, return true and assign its position, otherwise return false
  if (xPos>0){
    if (layout[yPos][xPos-1]==2){//REPLACE WITH DISTANCE SENSOR CONDITIONAL
      boxx=xPos-1;//set box's position
      boxy=yPos;
      return true;
    }
  }
  return false;
}

void turnleft(){//turn the car 90 degrees counter clockwise and assign it a new facing accordingly
  //INSERT TURNING CODE
  if(carOrientation=='N'){
    carOrientation='W';
  } else if(carOrientation=='W'){
    carOrientation='S';
  } else if(carOrientation=='S'){
    carOrientation='E';
  } else {
    carOrientation='N';
  }
}
void turnright(){//turn the car 90 degrees clockwise and assign it a new facing accordingly
  //INSERT TURNING CODE
  if(carOrientation=='N'){
    carOrientation='E';
  } else if(carOrientation=='W'){
    carOrientation='N';
  } else if(carOrientation=='S'){
    carOrientation='W';
  } else {
    carOrientation='S';
  }  
}



bool leftclear(){//if there is no box to the west, and the square has not yet been checked, return true, otherwise, return false
  if (xPos!=0){
    if (layout[yPos][xPos-1]==0){ //CHANGE TO IF NO DISTANCE SENSOR IN WEST DIRECTION AND WESTERN GRID TILE UNCHECKED
      return true;
    }
  }
  return false;
}
bool rightclear(){//if there is no box to the east, and the square has not yet been checked, return true, otherwise, return false
  if (xPos!=7){
    if (layout[yPos][xPos+1]==0){ //CHANGE TO IF NO DISTANCE SENSOR IN EAST DIRECTION AND EASTERN GRID TILE UNCHECKED
      return true;
    }
  }
  return false;
}
bool upclear(){//if there is no box to the north, and the square has not yet been checked, return true, otherwise, return false
  if (yPos!=7){
    if (layout[yPos+1][xPos]==0){ //CHANGE TO IF NO DISTANCE SENSOR IN NORTH DIRECTION AND NORTHERN GRID TILE UNCHECKED
      return true;
    }
  }
  return false;
}
bool downclear(){//if there is no box to the south, and the square has not yet been checked, return true, otherwise, return false
  if (yPos!=0){
    if (layout[yPos-1][xPos]==0){ //CHANGE TO IF NO DISTANCE SENSOR IN EAST DIRECTION AND EASTERN GRID TILE UNCHECKED
      return true;
    }
  }
  return false;
}



void moveleft(){//turn to face west, then drive forwards one square
  while (carOrientation!='W'){//turn until facing west
    if (carOrientation=='N'){
      turnleft();
    } else {
      turnright();
    }
  }
  layout[yPos][xPos]=1;//update serial grid to show current square as checked
  xPos=xPos-1;//update position to be one to the west
  layout[yPos][xPos]=9;//update serial grid to show current position of car
}
void moveright(){//turn to face east, then drive forwards one square
  while (carOrientation!='E'){//turn until facing east
    if (carOrientation=='S'){
      turnleft();
    } else {
      turnright();
    }
  }
  layout[yPos][xPos]=1;//update serial grid to show current square as checked
  xPos=xPos+1;//update position to be one to the east
  layout[yPos][xPos]=9;//update serial grid to show current position of car
}
void moveup(){//turn to face north, then drive forwards one square
  while (carOrientation!='N'){//turn until facing north
    if (carOrientation=='E'){
      turnleft();
    } else {
      turnright();
    }
  }
  layout[yPos][xPos]=1;//update serial grid to show current square as checked
  yPos=yPos+1;//update position to be one to the north
  layout[yPos][xPos]=9;//update serial grid to show current position of car
}
void movedown(){//turn to face south, then drive forwards one square
  while (carOrientation!='S'){//turn until facing south
    if (carOrientation=='W'){
      turnleft();
    } else {
      turnright();
    }
  }
  layout[yPos][xPos]=1;//update serial grid to show current square as checked
  yPos=yPos-1;//update position to be one to the south
  layout[yPos][xPos]=9;//update serial grid to show current position of car
}

bool leftsix(){//if there is a path tile to the west and the tile does not contain a box, return true, otherwise return false
  if (xPos!=0){
    if (layout[yPos][xPos-1]==6){ //ADD DISTANCE SENSOR EXPRESSION TO CHECK FOR BOX IN WEST DIRECTION
      return true;
    }
  }
  return false;
}
bool rightsix(){//if there is a path tile to the east and the tile does not contain a box, return true, otherwise return false
  if (xPos!=7){
    if (layout[yPos][xPos+1]==6){ //ADD DISTANCE SENSOR EXPRESSION TO CHECK FOR BOX IN EAST DIRECTION
      return true;
    }
  }
  return false;
}
bool upsix(){//if there is a path tile to the north and the tile does not contain a box, return true, otherwise return false
  if (yPos!=7){
    if (layout[yPos+1][xPos]==6){ //ADD DISTANCE SENSOR EXPRESSION TO CHECK FOR BOX IN NORTH DIRECTION
      return true;
    }
  }
  return false;
}
bool downsix(){//if there is a path tile to the south and the tile does not contain a box, return true, otherwise return false
  if (yPos!=0){
    if (layout[yPos-1][xPos]==6){ //ADD DISTANCE SENSOR EXPRESSION TO CHECK FOR BOX IN SOUTH DIRECTION
      return true;
    }
  }
  return false;
}

