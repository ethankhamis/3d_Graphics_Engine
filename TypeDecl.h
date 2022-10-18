#pragma once

namespace colours
{
	struct BGRAColour
	{
		unsigned char A;
		unsigned char R;
		unsigned char G;
		unsigned char B;
	};
}

namespace object_variables
{

	struct orientation
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
	};

	struct position
	{
		float x = 0;
		float y = 0;
		float z = 0;
	};

};