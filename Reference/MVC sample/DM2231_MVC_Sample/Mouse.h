#pragma once

class CMouse
{
public:
	CMouse(void);
	~CMouse(void);

	int x, y;
	int last_x, last_y;
	bool bLButtonDown,bRButtonDown;
	bool bLButtonUp,bRButtonUp;

	void Init(void)
	{
		x = -1;
		y = -1;
		last_x = -1;
		last_y = -1;
		bLButtonDown = false;
		bRButtonDown = false;
		bLButtonUp = true;
		bRButtonUp = true;
	};

	void SetMousePos( const int new_x, const int new_y )
	{
		if (this->last_x == -1)
		{
			this->last_x = new_x;
			this->last_y = new_y;
		}
		else
		{
			this->last_x = x;
			this->last_y = y;
		}

		this->x = new_x;
		this->y = new_y;
	}

	int GetDiff_X(void)
	{
		return (x - last_x);
	}

	int GetDiff_Y(void)
	{
		return (y - last_y);
	}
};
