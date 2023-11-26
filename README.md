# ECE1895-Bop-It

Bark-N-Bop Game
Overview
Bark-N-Bop is an interactive game designed to engage users in a fun and challenging experience. The game prompts users with three commands - Paw-It, Walk-It, and Speak-It. Users need to respond to each command within a given time frame to earn points. The game includes visual and auditory cues to guide users through the commands and provides feedback on their performance.

Components
Libraries Used:
  LiquidCrystal_I2C: Library for controlling I2C character LCDs.
Hardware Components:
  LCD Display: A 20x4 character LCD is used to provide visual feedback and display game-related information.
Buttons:
  Start Button (Pin 8): Initiates the game when pressed.
  Paw-It Button (Pin 5), Walk-It Button (Pin 6), Speak-It Button (Pin 10): Buttons for user input in response to commands.
LEDs:
  Correct Action LED (Pin 2): Green LED indicating successful completion of a command.
  Incorrect Action LED (Pin 4): Red LED indicating an unsuccessful action or the end of the game.
  Speaker (Pin 9): Provides auditory cues for each command and feedback on user performance.

Game Logic
Initialization:
  The LCD is initialized, and pin modes for buttons, LEDs, and the speaker are set.
  Game Loop (loop()): The game starts with a display message on the LCD prompting the user to press the start button.
  
User Input (actionChecker()):
  When the start button is pressed, the game enters a loop where it randomly prompts the user with one of three commands: Paw-It, Walk-It,  or Speak-It.
  The corresponding melody and visual display are provided for each command.
  The user must respond to the prompt by pressing the appropriate button within a time limit.
  
Feedback is given based on the user's performance:
  Correct Action: Green LED lights up, and a "Correct!" message is displayed.
  Incorrect Action: Red LED lights up, and an "Incorrect" message is displayed.
  Game Over: If the user reaches 99 points, a "You Won!" message is displayed, followed by the total score.
  
Game Over:
  After each round, the game checks if the user has reached 99 points. If so, the game ends, displaying the total score.
  The game variables are reset for a new round.
  
Musical Notes and Melodies
  The game uses predefined musical notes and melodies for each command. Melodies are played through the speaker to enhance the gaming experience.

Adjustable Parameters
  turn: Time limit for completing each prompted command (initially set to 10 seconds).
  timeout: Overall time limit for user response (initially set to 10 seconds).
  Melody-related parameters like tempo, wholenote, divider, and noteDuration control the musical aspect of the game.

How to Play
  Connect the hardware components as described above.
  Upload the provided code to your Arduino board.
  Press the start button to initiate the game.
  Respond to each prompted command (Paw-It, Walk-It, Speak-It) by pressing the corresponding button within the time limit.
  Receive feedback on your performance through the LCD display, LEDs, and speaker.
  Try to reach a score of 99 to win the game.

Enjoy playing Bark-N-Bop!
