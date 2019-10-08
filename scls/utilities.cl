float	reverse(int n)
{
	if (n != 0)
		return (1.0f / n);
	return (0);
}

int		get_light(int start, int end, float percentage)
{
	return ((int)((1 - percentage) * start + percentage * end));
}

int		get_color(float3 v, int type)
{
	int     red;
	int     green;
	int     blue;
	int 	start;
	int		end;

	float e;
	e  = (max(max(v.x, v.y), v.z));
	if (e > 1)
		v *= 1.f / e;
	if (type & GRAY)
	{
		float3	c_linear;
		float	y_linear;

		c_linear.x = v.x > 0.04045f ? pow((v.x + 0.055f) / 1.055f, 2.4f) : v.x / 12.92f;
		c_linear.y = v.y > 0.04045f ? pow((v.y + 0.055f) / 1.055f, 2.4f) : v.y / 12.92f;
		c_linear.z = v.z > 0.04045f ? pow((v.z + 0.055f) / 1.055f, 2.4f) : v.z / 12.92f;

		y_linear = 0.2126f * c_linear.x + 0.7152f * c_linear.y + 0.0722f * c_linear.z;
		v.x = y_linear > 0.0031308f ? 1.055f * pow(y_linear, 1.f / 2.4f) - 0.055f  : 12.92f * y_linear;
		v.y = v.x;
		v.z = v.x;
	}
	start = 0;
	end = 0xFFFFFF;
	red = get_light((start >> 16) & 0xFF, (end >> 16) & 0xFF, v.x);
	green = get_light((start >> 8) & 0xFF, (end >> 8) & 0xFF, v.y);
	blue = get_light(start & 0xFF, end & 0xFF, v.z);

	return ((red << 16) | (green << 8) | blue);
}
