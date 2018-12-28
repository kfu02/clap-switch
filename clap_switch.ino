//created by Kevin Fu, last updated 12/28/18
int input_pin = A1; //pin 2 on the attiny85 (but set for analog input)
int signal_led = 0; //signal_led for debug purposes (currently set to show when 'first' clap detected)
int relay_pin = 1; 

#define CLAPS 2 //number of claps to trigger relay
#define MAX_DIFF 500*CLAPS //span of time for CLAPS claps
#define DECAY 200 //so one clap isn't detected as multiple

int sound_value = 0; 
unsigned long last_clap_time = 0;
int recent_claps = 0;
bool relay_on = false;

void setup() {
  pinMode(input_pin, INPUT);
  pinMode(signal_led, OUTPUT);
  pinMode(relay_pin, OUTPUT);
}

void loop() {
  if(last_clap_time != 0){
    if(millis() - last_clap_time < DECAY){ 
      return; //if clap happened very recently, likely just the same clap ringing, so ignore
    }
    if(millis() - last_clap_time > MAX_DIFF){ 
      recent_claps = 0; //if first clap happened a while ago, likely accidental, so reset
      last_clap_time = 0;
    }
    digitalWrite(signal_led, LOW); 
  }
  else{
    digitalWrite(signal_led, HIGH);
  }
  
  if(recent_claps == CLAPS){ //if CLAPS limit reached
    relay_on = !relay_on;
    digitalWrite(relay_pin, relay_on); //toggle relay
    recent_claps = 0;
    last_clap_time = 0; //reset
    delay(DECAY); //has a problem with switching relay when CLAPS exceeded, somehow
  }        
  else{ //don't detect CLAPS unless it's needed
    sound_value = analogRead(input_pin);
    if(sound_value > 6){ //on clap (analog input above 6 in this case), update recent_claps and lcTime 
      last_clap_time = millis();
      recent_claps++;
    }
  }
}
