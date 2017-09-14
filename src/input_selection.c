/*
 * Includes for the program.
 */

/*
 * C naming converence
 * https://stackoverflow.com/questions/1722112/what-are-the-most-common-naming-conventions-in-c
 */

enum ETransistor
{
	Q1,
	Q2,
	Q3,
	Q4
};

enum EDirection
{
	forward,
	backward,
	null
};

struct input_data
{
	enum ETransistor current_transistor;	/* Current transistor which is in saturation mode */
	enum EDirection	previous_direction;	/* Direction of the rotary encoder before we reached the current_transistor */
	enum EDirection current_direction;	/* Direction of the rotary encoder the user rotated to before we changed the current_transistor */
} SInput_data;

void interrupt(void)
{
	/* if pin A is HIGH and pin B is LOW -> forward/backward 
	 * if pin A is HIGH and pin B is HIGH -> forward/backward
	 */

	/*
	 * Once a change takes place we need to verify in what direction the rotary encoder has been turned.
	 * NOTE: define pin_a and pin_b of the rotary encoder
	 */
	if (pin_a == 0 && pin_b == 0)
	{
		SInput_data.current_direction = backward;	
	} else if (pin_a == 0 && pin_b == 1)
	{
		SInput_data.current_direction = forward;
	} else					/* Could not identify the direction */ 
	{
		return;
	}

	/*
	 * If we verified the direction of the rotary encoder successful, we need to turn the current transistor OFF and turn the new one ON.
	 * The rotary encoder can be rotated infinite, but we got only 4 valid inputs.
	 * We need to make a decision based on current transistor AND the direction of the rotary encoder.
	 */
	
	switch (SInput_data.current_direction)
	{
		case forward:
			if (SInput_data.current_transistor == Q4)
			{
				SInput_data.current_transistor = Q1;
				SInput_data.previous_direction = SInput_data.current_direction;
				SInput_data.current_direction = null;
			} else 
			{
				SInput_data.current_transistor += 1;
				SInput_data.previous_direction = current_direction;
				SInput_data.current_direction = null;	
			}
			break;
			
	
		case backward:
			if (SInput_data.current_transistor == Q1)
			{
				SInput_data.current_transistor = Q4;
				SInput_data.previous_direction = SInput_data.previous_direction;
				SInput_data.current_direction = null;
			} else
			{		
				SInput_data.current_transistor -= 1;
				SInput_data.previous_direction = SInput_data.previous_direction;
				SInput_data.current_direction = null;
			}
			break;
	}
}
