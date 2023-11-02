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
unsigned int speaker = 9; // speaker for telling the user which command they need to do 

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

// define variables to control the output sounds for each command 
int tempo = 360;
int wholenote = (60000 * 4) / tempo;
int divider = 0;
int noteDuration = 0;

// Set a timer to limit the user's response time
unsigned long timeout = 10000; // 10 seconds

//define tones
// https://github.com/tsukisan/Arduino/blob/master/WiiClassicSoundboard/Notes.h
// #define NOTE_B0 31
// #define NOTE_C1 33
// #define NOTE_CS1 35
// #define NOTE_D1 37
// #define NOTE_DS1 39
// #define NOTE_E1 41

//define tones
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978
#define REST 0


// define sounds for each command 
int pawMelody[] = {
  NOTE_A5,4, NOTE_DS8,4, NOTE_E5,4,
};

int walkMelody[] = {
  NOTE_G4,4, NOTE_A4,4, NOTE_A5,4,
};

int speakMelody[] = {
  NOTE_A6,4, NOTE_G6,4, NOTE_GS6,4, NOTE_DS6,4,  
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int pawNotes = sizeof(pawMelody) / sizeof(pawMelody[0]) / 2;
int walkNotes = sizeof(walkMelody) / sizeof(walkMelody[0]) / 2;
int speakNotes = sizeof(speakMelody) / sizeof(speakMelody[0]) / 2;


void setup() {
  // initialize the lcd 
	lcd.init();
  lcd.backlight();
	delay(500);

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

  delay(500);
  // check the sensory input and compare it with the given action 
  action_before = digitalRead(actionPin);

  // record the time elapsed between prompting the command and completing the action 
  time_prompted = millis(); 
  time_elapsed = 0; 

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
  while(time_elapsed < timeout && action_completed==0 && !incorrect_action1 && !incorrect_action2){
    // read the action to see if it matches what was prompted
    action_after = digitalRead(actionPin);
    incorrect_action1 = digitalRead(incorrect_action_set[0]); 
    incorrect_action2 = digitalRead(incorrect_action_set[1]);

    // compare the actions before and after to see if action was completed 
    if(action_before != action_after){
      action_completed = 1; // mark the action as completed 
      score = score + 1; // reward a point for a successful action 
      turn = turn - (turn * .05); // decrease the game rate so the time speeds up for each round
      break; 
    }
    time_elapsed = millis() - time_prompted; // update time elapsed 
    
  }
  
  // the game ends when the user reaches 99 points. check the score 
  if(action_completed == 1 && score == 99){

    // tell the user they won
    lcd.clear(); 
    lcd.print("You Won!"); 
    delay(1000);
            
    // total up the points
    total_score = score;

    // prompt the user game over with their final score 
    lcd.clear(); 
    lcd.print("Game Over"); 
    delay(1000);
    lcd.clear(); 
    lcd.print("Total Score:"); 
    delay(1000);
    lcd.clear();
    lcd.print(total_score); 
    delay(1000); 

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
    delay(1000);
    // light up greem LED when action is successful 
    digitalWrite(correct_action, HIGH);  
    delay(1000);
    digitalWrite(correct_action, LOW);  
  }

  // if the user did not have a successful action, flash the red LED and end the game
  else if(action_completed == 0 || incorrect_action1 || incorrect_action2){
    // prompt the user of a successful action 
    lcd.clear(); 
    lcd.print("Incorrect"); 
    delay(1000);
    // light up red LED when action is successful 
    digitalWrite(incorrect_action, HIGH);  
    delay(1000); 

    // when an action is unsuccessful, the game ends 
    // total up the points
    total_score = score; 
            
    // prompt the user game over with their final score 
    lcd.clear(); 
    lcd.print("Game Over"); 
    delay(1000);
    lcd.clear(); 
    lcd.print("Total Score:"); 
    delay(1000);
    lcd.clear();
    lcd.print(total_score); 
    delay(1000); 

    // reset the game variables 
    turn = 500; 
    score = 0; 
    repeat = 0;
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
  delay(500); 

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
          delay(500);
          for (int thisNote = 0; thisNote < pawNotes * 2; thisNote = thisNote + 2) {
              // calculates the duration of each note
              divider = pawMelody[thisNote + 1];
              if (divider > 0) {
                // regular note, just proceed
                noteDuration = (wholenote) / divider;
              } else if (divider < 0) {
                // dotted notes are represented with negative durations!!
                noteDuration = (wholenote) / abs(divider);
                noteDuration *= 1.5; // increases the duration in half for dotted notes
              }

            tone(speaker, pawMelody[thisNote], noteDuration * 0.9); // Play the single note
            delay(noteDuration);  // Delay for the specified time
            noTone(speaker);  // Silence the note that was playing
          }
          actionChecker(action, paw_it, "Paw-It"); 
          break; 

        // CASE 2: WALK-IT COMMAND
        case 2:  
          delay(500);
          for (int thisNote = 0; thisNote < walkNotes; thisNote = thisNote + 2) {
              // calculates the duration of each note
              divider = walkMelody[thisNote + 1];
              if (divider > 0) {
                // regular note, just proceed
                noteDuration = (wholenote) / divider;
              } else if (divider < 0) {
                // dotted notes are represented with negative durations!!
                noteDuration = (wholenote) / abs(divider);
                noteDuration *= 1.5; // increases the duration in half for dotted notes
              }

            tone(speaker, walkMelody[thisNote], noteDuration * 0.9); // Play the single note
            delay(noteDuration);  // Delay for the specified time
            noTone(speaker);  // Silence the note that was playing
          }
          actionChecker(action, walk_it, "Walk-It"); 
          break; 

        // CASE 3: SPEAK-IT COMMAND 
        case 3:
          delay(500);
          for (int thisNote = 0; thisNote < speakNotes; thisNote = thisNote + 2) {
              // calculates the duration of each note
              divider = speakMelody[thisNote + 1];
              if (divider > 0) {
                // regular note, just proceed
                noteDuration = (wholenote) / divider;
              } else if (divider < 0) {
                // dotted notes are represented with negative durations!!
                noteDuration = (wholenote) / abs(divider);
                noteDuration *= 1.5; // increases the duration in half for dotted notes
              }

            tone(speaker, speakMelody[thisNote], noteDuration * 0.9); // Play the single note
            delay(noteDuration);  // Delay for the specified time
            noTone(speaker);  // Silence the note that was playing
          }
          actionChecker(action, speak_it, "Speak-It"); 
          break; 
        }
    }
  }
}













