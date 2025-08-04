#include <Arduino.h>
#include <ESP32Encoder.h>
#include <FT891_CAT.h>
#include <Cat.h>
#include <RotaryEncoder.h>

/*-------------------------------------------------------
   Optical Rotary encoder settings (used for vfo frequency)
--------------------------------------------------------*/
#define PULSE_INPUT_PIN 11// Rotaty Encoder A
#define PULSE_CTRL_PIN 12  // Rotaty Encoder B

#define PIN_IN1 4 // Rotaty Encoder B
#define PIN_IN2 6 // Rotaty Encoder B
#define PIN_IN3 17 // Rotaty Encoder Push

#define TXRX_SWITCH 20

ESP32Encoder Encoder;													 // optical
RotaryEncoder Rotary(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03); // mechanical

void xTaskDecoder(void *arg)
{
	while (1)
	{
		vTaskDelay(1);
		Rotary.tick();
	}
}

void setup()
{
	TaskHandle_t dHandle = NULL;

	Serial.begin(115200);
	while (!Serial)
		;

	pinMode(TXRX_SWITCH, INPUT);
	ESP32Encoder::useInternalWeakPullResistors = puType::none;
	Encoder.attachHalfQuad(PULSE_INPUT_PIN, PULSE_CTRL_PIN);
	xTaskCreate(xTaskDecoder, "xTaskDecoder", 4096, NULL, 2, &dHandle);
	CatInterface.begin();
}


int lastEncoding{0};
int total = 0;
int tx = 0;

int value, currentRxtx = 0;
void loop()
{
	CatInterface.checkCAT();
	
	int count_vfo = Encoder.getCount();
	Encoder.clearCount();
	if (count_vfo)
	{
		int currMillis = millis();
		if ((currMillis - lastEncoding < 3) && abs(count_vfo) < 2)
		{
			total += count_vfo;
		}
		else
		{
			lastEncoding = currMillis;
			total += count_vfo;
			CatInterface.Setft(total);
			total = 0;
		}
	}

	int count_button = (int)Rotary.getPosition();
	Rotary.setPosition(0);
	if (count_button)
	{	// Volume
		CatInterface.Setag(count_button);
	}

	int RemoteRxTx = CatInterface.GetTX();
	if (RemoteRxTx == TX_MAN)
	{
		currentRxtx = TX_MAN;
	}

	if (RemoteRxTx == TX_OFF && currentRxtx != TX_OFF)
	{
		currentRxtx = TX_OFF;
	}

	int rxtx = digitalRead(TXRX_SWITCH);
	if (!rxtx)
	{
		if (currentRxtx == TX_OFF)
		{
			CatInterface.Settx(TX_CAT);
			currentRxtx = TX_CAT;
		}
	}
	else
	{
		if (currentRxtx == TX_CAT)
		{
			CatInterface.Settx(TX_OFF);
			currentRxtx = TX_OFF;
		}
	}
	vTaskDelay(10);
}
