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
	backward
};

struct input_data
{
	enum ETransistor current_transistor;	/* Current transistor which is in saturation mode */
	enum EDirection	previous_direction;	/* Direction of the rotary encoder before we reached the current_transistor */
	enum EDirection current_direction;	/* Direction of the rotary encoder the user rotated to before we changed the current_transistor */
}

void interrupt(void)
{
	/* if pin A is HIGH and pin B is LOW -> forward/backward 
	 * if pin A is HIGH and pin B is HIGH -> forward/backward
	 */


}
