# include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// define sensory input variables (i.e., the commands and START button)
int start_button = 8; 
int paw_it = 5;
int walk_it = 6;
int speak_it = 10;

// define output variables:
int correct_action = 2; // green LED for successful action
int incorrect_action = 4; //red LED for unsuccessful action 
int speaker = 9; // speaker for telling the user which command they need to do 

long action = 0; // we will switch on action 
int action_before = 0; // action before holds the state of the previous command (before new command prompted)
int action_after = 0; // action after holds the state of the completed command (after user was prompted with new command)

// count timer for recording if actions are completed in the alotted time 
unsigned long turn = 500; // 10 seconds to complete the prompted command 
unsigned long time_prompted; // initial time command was given to user 
unsigned long time_elapsed; // measures how much time has passed to check if user completed action quick enough 

// define variables to control game loop 
int repeat = 0; // denotes whether the game is still going on or completed
int started = 0; 
int score = 0; // score to increment according to successful actions 
int total_score = 0; //accumulate score as game goes on 
int action_completed = 0; // bool that gets set to 1 when an action is successful 
int incorrect_action_set[] = {0,0}; 
int incorrect_action1 = 0; 
int incorrect_action2 = 0; 

void setup() {
  // initialize the lcd 
	lcd.init();
  lcd.backlight();
	delay(50);

  // set up the pin modes
  pinMode(correct_action, OUTPUT); 
  pinMode(incorrect_action, OUTPUT); 
  pinMode(paw_it, INPUT); 
  pinMode(walk_it, INPUT); 
  pinMode(speak_it, INPUT); 
  pinMode(start_button, INPUT); 
  pinMode(speaker, OUTPUT); 

}

// function to compare the prompted action with the sensory input from the user 
void actionChecker(int action, int actionPin, String action_str){

  // check the sensory input and compare it with the given action 
  action_before = digitalRead(actionPin);

  // record the time elapsed between prompting the command and completing the action 
  time_prompted = millis(); 
  time_elapsed = millis() - time_prompted; 

  lcd.clear(); 
  lcd.print(action_str); 

  // check for incorrect input 
  if(actionPin == paw_it){
    incorrect_action_set[0] = 6; 
    incorrect_action_set[1] = 10; 
  }
  else if(actionPin == walk_it){
    incorrect_action_set[0] = 5; 
    incorrect_action_set[1] = 10; 
  }
  else if(actionPin == speak_it){
    incorrect_action_set[0] = 5; 
    incorrect_action_set[1] = 6; 
  }

  // check if action was completed before timer runs out 
  while(time_elapsed < turn && action_completed == 0 && !incorrect_action1 && !incorrect_action2){
    // read the action to see if it matches what was prompted
    action_after = digitalRead(actionPin);
    incorrect_action1 = digitalRead(incorrect_action_set[0]); 
    incorrect_action2 = digitalRead(incorrect_action_set[1]);

    // compare the actions before and after to see if action was completed 
    if(action_before != action_after){
      action_completed = 1; // mark the action as completed 
      score = score + 1; // reward a point for a successful action 
      turn = turn - (turn * .05); // decrease the game rate so the time speeds up for each round 
      delay(50);
    }
    time_elapsed = millis() - time_prompted; // update time elapsed 
    //lcd.clear();
    //lcd.print(time_elapsed);
  }
  
  // the game ends when the user reaches 99 points. check the score 
  if(action_completed == 1 && score == 99){

    // tell the user they won
    lcd.clear(); 
    lcd.print("You Won!"); 
    delay(50);
            
    // total up the points
    total_score = score;

    // prompt the user game over with their final score 
    lcd.clear(); 
    lcd.print("Game Over"); 
    delay(100);
    lcd.clear(); 
    lcd.print("Total Score:"); 
    delay(100);
    lcd.clear();
    lcd.print(total_score); 
    delay(50); 

    // reset the game variables 
    turn = 500; 
    score = 0; 
    repeat = 0; 
  } 

  // if the user does not have 99 points, but had a successful action, reward a point and flash green LED  
  else if(action_completed == 1){

    // prompt the user of a successful action 
    lcd.clear(); 
    lcd.print("Correct!"); 
    delay(50);
    // light up greem LED when action is successful 
    digitalWrite(correct_action, HIGH);  
    delay(50);
    digitalWrite(correct_action, LOW);  
  }

  // if the user did not have a successful action, flash the red LED and end the game
  else if(action_completed == 0){
    // prompt the user of a successful action 
    lcd.clear(); 
    lcd.print("Incorrect"); 
    delay(50);
    // light up red LED when action is successful 
    digitalWrite(incorrect_action, HIGH);  
    delay(50); 

    // when an action is unsuccessful, the game ends 
    // total up the points
    total_score = score; 
            
    // prompt the user game over with their final score 
    lcd.clear(); 
    lcd.print("Game Over"); 
    delay(100);
    lcd.clear(); 
    lcd.print("Total Score:"); 
    delay(100);
    lcd.clear();
    lcd.print(total_score); 
    delay(50); 

    // reset the game variables 
    turn = 500; 
    score = 0; 
    repeat = 0;
    delay(50); 
    digitalWrite(incorrect_action, LOW); 
    incorrect_action1 = 0; 
    incorrect_action2 = 0;  
  }
}

// loop to run continuously to run the game  
void loop() {
  
  // display starting message 
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Bark-N-Bop!");
  lcd.setCursor(2,1);
  lcd.print("Press START");  
  delay(50); 

  started = 0; 
  // check if the start button was pressed 
  started = digitalRead(start_button); 

  if(started){
    repeat = 1; // keeps the game going 

    while(repeat != 0){
      
      // randomly prompt the user with the 1 of 3 commands: 
      action = random(1, 4); 
      action_completed = 0; // bool set to false until an action has been completed. when action is completed, action_completed = 1  
      lcd.clear();

      
      // switch on the 3 commands
        switch(action){

        // CASE 1: PAW-IT COMMAND
        case 1: 
          //lcd.clear(); 
          //lcd.print("Paw-It!"); 
          actionChecker(action, paw_it, "Paw-It"); 
          break; 

        // CASE 2: WALK-IT COMMAND
        case 2: 
          //lcd.clear(); 
          //lcd.print("Walk-It!"); 
          actionChecker(action, walk_it, "Walk-It"); 
          break; 

        // CASE 3: SPEAK-IT COMMAND 
        case 3:
          //lcd.clear(); 
          //lcd.print("Speak-It!"); 
          actionChecker(action, speak_it, "Speak-It"); 
          break; 
        }
    }
  }
}













