# Introduction to Robotics (2023-2024)
Introduction to Robotics Laboratory Homework is an integral part of the 3rd-year curriculum at the Faculty of Mathematics and Computer Science, University of Bucharest. These assignments encompass a wide range of elements, including comprehensive requirements, intricate implementation details, and code and image files.
# Homework 😀
<details>
  <summary><h3>Homework 1: RGB Led Controlled by Three Potentiometers</h3></summary>
  <p><strong>Task requirements:</strong> Control each channel (red, green, and blue) of an RGB LED using three separate potentiometers.This control is achieved using digital electronics, where the Arduino reads the potentiometer values and then sends mapped values to control each of the pins linked to the LED.  </p>
  <p>
    <strong>Components Used:</strong>
    <ul>
      <li>1 RGB LED</li>
      <li>3 potentiometers</li>
      <li>wires and 3 resistors of 330Ω </li>
    </ul>
  </p>
  <p><strong>Setup photo:</strong></p>
  <p><img src="https://github.com/CaruntuRazvan/IntroductionToRobotics/assets/115624498/43815df3-0206-4cf7-b8d2-8b59947a3d6e" alt="Text alternativ al imaginii" height="600" width="600"></p>
  <p><strong>Functionality:</strong> <a href="https://www.youtube.com/shorts/vQ07wmHCx5U" target="_blank">Link video</a></p>
</details>  

<details>
  <summary><h3>Homework 2: Elevator simulator wannabe</h3></summary>
  <p><strong>Task requirements:</strong> This project involves creating a simulation of a 3-floor elevator control system using LEDs, buttons, and a buzzer. Each of the 3 LEDs is meant to correspond to one of the 3 floors, with the LED representing the current floor illuminating. Additionally, another LED (the red one) signifies the elevator's operational status. It will blink while the elevator is in motion and remain steady when it is stationary.
    
The system also incorporates 3 buttons that simulate the call buttons from the 3 different floors. When one of these buttons is pressed, the elevator should simulate movement towards that floor after a brief delay of around 1-2 seconds. Furthermore, the buzzer should produce brief sounds in the following situations: when the elevator arrives at the desired floor (similar to a "cling" sound) and during the elevator doors closing and movement. </p>
  <p>
    <strong>Components Used:</strong>
    <ul>
      <li>4 LEDs</li>
      <li>3 buttons</li>
      <li>a buzzer</li>
      <li>wires and 5 resistors (4 with a resistance of 330 Ω, one for each LED, and 1 with a resistance of 100 Ω for the buzzer)</li>
    </ul>
  </p>
  <p><strong>Setup photo:</strong></p>
  <p><img src="https://github.com/CaruntuRazvan/IntroductionToRobotics/assets/115624498/1b553a53-0c07-4b15-a1a8-0034dfcd55e9" alt="Text alternativ al imaginii" height="600" width="600"></p>
  <p><strong>Functionality:</strong> <a href="https://www.youtube.com/shorts/MaEhQ8o4Uqs" target="_blank">Link video</a></p>
</details>  

<details>
  <summary><h3>Homework 3: 7 Segment Display Drawing</h3></summary>
  <p><strong>Task requirements:</strong> This project uses the joystick to control the segment's position and simulate "drawing" on the display. The transitions between segments should be natural, allowing them to move from their current positions to neighboring ones without passing through any "walls." The initial position should be set on the decimal point (DP). The current position will always blink, regardless of whether the segment is turned on or off. The user can use the joystick to navigate to neighboring positions, as described in the accompanying table for movement directions. A short press of the button will toggle the segment's state from ON to OFF or from OFF to ON. A long press of the button will reset the entire display by turning off all segments and relocating the current position to the decimal point.
 </p>
  <p>
    <strong>Components Used:</strong>
    <ul>
      <li>one 7-Segment Display</li>
      <li>one joystick</li>
      <li>wires and 8 resistors (with a resistance of 220Ω or 330Ω)</li>
    </ul>
  </p>
  <p><strong>Neighbors for 7-Segment Display:</strong></p>
  <p><img src="https://github.com/CaruntuRazvan/IntroductionToRobotics/assets/115624498/33d34a42-cab1-40c3-8227-3751b44228ab" alt="Text alternativ al imaginii" height="175" width="335"></p>
  <p><strong>Setup photo:</strong></p>
  <p><img src="https://github.com/CaruntuRazvan/IntroductionToRobotics/assets/115624498/70781c11-078a-48a2-b27e-325d916abf1c" alt="Text alternativ al imaginii" height="600" width="600"></p>
  <p><strong>Functionality:</strong> <a href="https://www.youtube.com/shorts/ypRhZ_DGU2k" target="_blank">Link video</a></p>
</details>  

<details>
  <summary><h3>Homework 4: Stopwatch Timer</h3></summary>
  <p><strong>Task requirements:</strong> In this project, the goal is to create a stopwatch timer using a 4-digit 7-segment display and 3 buttons. The timer should measure in 10ths of a second and include a save lap function. The initial display shows "000.0". Initiating the timer occurs upon pressing the Start/Stop button. While the timer is running, pressing the lap button stores the current timer value in memory, allowing for up to 4 laps to be saved.Resetting the timer with the reset button has no effect on counting.

If the pause button is pressed, the timer halts, and the lap flag button becomes inactive in Pause Mode.Pressing the reset button restores the timer to "000.0". Following a reset, the flag buttons can be utilized to sequentially navigate through the saved lap times. Each button press transitions to the next saved lap, and continuous pressing facilitates a seamless cycling experience. Resetting in this state clears all flags and reverts the timer display to "000.0". 
 </p>
  <p>
    <strong>Components Used:</strong>
    <ul>
      <li>a 4-digit 7-segment display</li>
      <li>a 74HC595 shift register</li>
      <li>
        3 buttons:
        <ul>
          <li>Start or Stop Button  </li>
<li>Reset Button: reset (when in pause mode) or clear saved laps (when in lap-viewing mode). </li>
<li>Lap Button: save a lap (if in counting mode) and navigate through the last saved laps.</li>
        </ul>
      </li>
      <li>wires and 7 resistors (with a resistance of 220Ω or 330Ω)</li>
    </ul>
</p>

  <p><strong>Electronic scheme:</strong></p>
  <p><img src="https://github.com/CaruntuRazvan/IntroductionToRobotics/assets/115624498/ed013c77-6727-482a-bb8d-2f9a1ba46fbb" alt="Text alternativ al imaginii" height="500" width="500"></p>
  <p><strong>Setup photo:</strong></p>
  <p><img src="https://github.com/CaruntuRazvan/IntroductionToRobotics/assets/115624498/620f6c3a-2820-4d89-9c4c-da82e26aab5a" alt="Text alternativ al imaginii" height="600" width="600"></p>
  <p><strong>Functionality:</strong> <a href="https://www.youtube.com/shorts/zXETFUREEnk" target="_blank">Link video</a></p>
</details>  

<details>
  <summary><h3>Homework 5: Pseudo-Smart Environment Monitor and Logger</h3></summary>
  <p><strong>Task requirements:</strong> In this project, the goal is to create a "Smart Environment Monitor and Logger" using Arduino, incorporating a range of sensors to collect environmental data. The system will store this data in EEPROM, offering visual feedback through an RGB LED, and enabling user interaction via a Serial Menu. The project emphasizes the integration of sensor readings, memory management, Serial Communication, and the overarching goal of constructing a user-friendly menu system. 
 </p>
  <p><strong>Menu structure:</strong>
  <ul>
        <li><strong>1. Sensor Settings</strong>
            <ul>
                <li>1.1 Sensors Sampling Interval.Set the sampling rate for the sensors between 1 and 10 seconds.</li>
                <li>1.2 Ultrasonic Alert Threshold.Set the threshold value for the ultrasonic sensor to trigger an alert.</li>
                <li>1.3 LDR Alert Threshold.Set the threshold value for the LDR sensor to trigger an alert.</li>
                <li>1.4 Back</li>
            </ul>
        </li>
        <li><strong>2. Reset Logger Data</strong>
            <ul>
                <li>2.1 Yes.Confirm to delete all data.</li>
                <li>2.2 No.Cancel the reset operation.</li>
            </ul>
        </li>
        <li><strong>3. System Status</strong>
            <ul>
                <li>3.1 Current Sensor Readings.Continuously print sensor readings at the set sampling rate.</li>
                <li>3.2 Current Sensor Settings. Display the sampling rate and threshold value for all sensors.</li>
                <li>3.3 Display Logged Data. Display the last 10 sensor readings for all sensors.</li>
                <li>3.4 Back</li>
            </ul>
        </li>
        <li><strong>4. RGB LED Control</strong>
            <ul>
                <li>4.1 Manual Color Control.Set the RGB colors manually.</li>
                <li>4.2 LED: Toggle Automatic ON/OFF.Toggle automatic mode for the LED.</li>
                <li>4.3 Back</li>
            </ul>
        </li>
  </ul>  
  </p>
  <p>
    <strong>Components Used:</strong>
    <ul>
      <li>RGB LED</li>
      <li>ultrasonic sensor (HC-SR04)</li>
      <li>photocell/LDR (Light-Dependent Resistor)</li>
      <li>wires and 4 resistors (3 with a resistance of 330 Ω for the RGB LED and 1 with a resistance of 10 kΩ for the photocell)</li>
    </ul>
</p>

  <p><strong>Electronic scheme:</strong></p>
  <p><img src="https://github.com/CaruntuRazvan/IntroductionToRobotics/assets/115624498/154a62d7-60ec-41f0-b9c6-9725d720f1b5" alt="Text alternativ al imaginii" height="450" width="500"></p>
  <p><strong>Setup photo:</strong></p>
  <p><img src="https://github.com/CaruntuRazvan/IntroductionToRobotics/assets/115624498/f8b6b372-7568-439c-8282-d6f3afb0e413" alt="Text alternativ al imaginii" height="600" width="600"></p>
  <p><strong>Functionality:</strong> <a href="https://www.youtube.com/watch?v=IK2Oxy2TLSA" target="_blank">Link video</a></p>
</details>

<details>
  <summary><h3>Homework 6: Mini Matrix Game</h3></summary>
  <p><strong>Task requirements:</strong> The purpose of this homework is to develop a concise game employing an 8x8 matrix with a minimum of three distinct elements: a player (displaying a slow blinking animation), bombs/bullets (exhibiting rapid blinking), and walls (remaining static without any blinking). The primary objective of this project is to establish a foundation for matrix-based gaming. The game design involves the random generation of walls covering 50% to 75% of the matrix, with players tasked to navigate the matrix, strategically destroying walls.
 </p>
  <p><strong>My game:For my game, I generate a randomized map where 50% of the matrix are walls. The player can deploy a bomb by activating the joystick, causing the bomb to detonate in one of the cardinal directions (up, down, left, or right). Subsequent bomb placement is allowed only after the previously deployed bomb has exploded.Throughout the process, an LED continuously blinks from the moment a bomb is placed until its explosion. If the player is within the bomb's blast radius upon detonation, they "die," triggering an animation.Likewise, destroying all walls results in a victory animation, leading to a game restart.</strong>  
 </p>
  <p>
    <strong>Components Used:</strong>
    <ul>
      <li>one joystick</li>
      <li>8x8 LED Matrix</li>
      <li>a MAX7219 driver</li>
      <li>1 LED</li>
      <li>wires and 2 resistors (1 with a resistance of 100 kΩ and 1 with a resistance of 330Ω for the red LED)</li>
    </ul>
</p>

  <p><strong>Setup photo:</strong></p>
  <p><img src="https://github.com/CaruntuRazvan/IntroductionToRobotics/assets/115624498/992f92c4-4130-402c-8105-2b0b573f8d7c" alt="Text alternativ al imaginii" height="600" width="600"></p>
  <p><strong>Functionality:</strong> <a href="https://www.youtube.com/watch?v=Ked8ob7sgOE" target="_blank">Link video</a></p>
</details>  
<details>
  <summary><h3>Homework 7: Mini Matrix Game Checkpoint</h3></summary>
  <p><strong>Task requirements:</strong> The purpose of this homework is to add an intro message and a menu and for the LCD for the previous homework.
 </p>
  <p><strong>My game:</strong> In my game, I have a menu implemented for the LCD, consisting of: "Start Game" where the actual game begins; "Settings" which is divided into LCD brightness and matrix brightness, where I can adjust the matrix and LCD contrast by moving the joystick up and down; and "About" where I display the game name, my name, and my GitHub. When the game starts, I generate a static map where the side walls cannot be destroyed, and my player has 3 lives. The player can place a bomb by pressing the joystick, and the bomb will explode in the up, down, left, or right directions.I have an LED that blinks from the moment a bomb is placed until it explodes.The player can place a new bomb only after the previously placed bomb has exploded. If the player loses all lives during the game, they "die" and an animation and corresponding message will be displayed on the matrix and on the LCD. Similarly, if the player destroys all destructible walls, there will be a winning animation and a message. Pressing the joystick will return to the main menu.
 </p>
    <p><strong>Menu structure:</strong>
  <ul>
        <li><strong>1. Start game</strong>
        </li>
        <li><strong>2. Settings</strong>
            <ul>
                <li>2.1 LCD  brightness  control</li>
                <li>2.2 Matrix brightness control </li>
            </ul>
        </li>
        <li><strong>3. About</strong> (details about creator)
        </li>
  </ul>  
  </p>
  <p>
    <strong>Components Used:</strong>
    <ul>
      <li>one joystick</li>
      <li>one LCD 2x16</li>
      <li>8x8 LED Matrix</li>
      <li>a MAX7219 driver</li>
      <li>one ceramic capacitor and one electrolytic capacitor</li>
      <li>one LED</li>
      <li>one potentiometer</li>
      <li>wires and 3 resistors (1 with a resistance of 100 kΩ, 1 with a resistance of 330Ω for the red LED and 1 with a resistance of 220Ω for LCD)</li>
    </ul>
</p>

  <p><strong>Setup photo:</strong></p>
  <p><img src="https://github.com/CaruntuRazvan/IntroductionToRobotics/assets/115624498/78eecd5f-3c7e-4fbb-8889-a5bc45771274" alt="Text alternativ al imaginii" height="600" width="600"></p>
  <p><strong>Functionality:</strong> <a href="https://www.youtube.com/watch?v=GBHDTK8-MbY" target="_blank">Link video</a></p>
</details> 
