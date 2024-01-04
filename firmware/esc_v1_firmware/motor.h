volatile byte sequenceStep = 0; // Stores step in spinning sequence

volatile bool motorUpslope = true; // Used for PWM
volatile bool test2 = false;

// PWM variables
volatile byte period = 249;           // MUST be less than 254
volatile byte duty = 100;
volatile byte nDuty = period - duty;  // Used to store the "inverse" duty, to reduce repeated operations withinm the interupt to calculate this
byte endClampThreshold = 15;           // Stores how close you need to be to either extreme before the PWM duty is clamped to that extreme

// AH_BL, AH_ CL, BH_CL, BH_AL, CH_AL, CH_BL
volatile byte motorPortSteps[] = {B00100100, B00100001, B00001001, B00011000, B00010010, B00000110};

// Other variables
volatile byte cyclesPerRotation = 2;
volatile byte cycleCount = 0;

volatile unsigned int currentRPM = 1000;
volatile unsigned int targetRPM = 0;
volatile byte controlScheme = 0;

bool reverse = false;
volatile bool motorStatus = false; // Stores if the motor is disabled (false) or not

const unsigned int spinUpStartPeriod = 5000;
const unsigned int spinUpEndPeriod = 500;
const byte stepsPerIncrement = 6;
const unsigned int spinUpPeriodDecrement = 10;

// Buzzer period limits
const unsigned int maxBuzzPeriod = 2000;
const unsigned int minBuzzPeriod = 200;



////////////////////////////////////////////////////////////
// Function declarations


void setPWMDuty(byte temp);

bool enableMotor(byte startDuty);
void windUpMotor();
void disableMotor();

void buzz(int periodMicros, int durationMillis);

void AH_BL();
void AH_CL();
void BH_CL();
void BH_AL();
void CH_AL();
void CH_BL();

/* Since we are comparing to relative the positive terminal, our edges have to be reversed

  E.g. If A is rising it will go from being less than the zero to more.
  Since A is connected to the negative terminal, the comparator output will go from 1 to 0

  So although we are looking for a rising edge, our microcontroller will see a falling one

  NOTE THAT ALL THESE WILL ENABLE THE COMPARATOR INTERRUPT!
*/
void BEMF_A_RISING();
void BEMF_A_FALLING();
void BEMF_B_RISING();
void BEMF_B_FALLING();
void BEMF_C_RISING();
void BEMF_C_FALLING();
