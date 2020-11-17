# Assignment1Report
- The implementation of the algorithm:	
first of all i checked the  absolute value of the gradients(in the first if condition),that means we checked if the gradients between 0 and 1 (also between -1 and 0 because of the abs) or bigger than 1 (also smaller than -1)	
after that i checked if we want to draw from the left to right or the opposite,we begin to draw from the point with the smaller coordinate (according to the case we solve that means if the gradients between 0 and 1 or -1 ans 1 we checked the x coordinate other cases we checked the y coordinate )-That is the SWITCH that we have learned-	
and the algorithm can be extended to cover gradients between 0 and -1 by checking whether y needs to increase or decrease (i.e. dy < 0) -That is the REFLECT- (in the SWITCH cas we checked the dx)

#
- Code:
 
 
void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	
	
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;
	if (abs(dy) < abs(dx))
	{
		if (p1.x < p2.x)
		{
			int x1 = p1.x;
			int y1 = p1.y;
			int yi = 1;
			int e = -dx;
			if (dy < 0)
			{
				yi = -1;
				dy = -dy;
			}
			while (x1 <= p2.x)
			{
				if (e > 0)
				{
					y1 += yi;
					e -= 2 * dx;
				}
				PutPixel(x1, y1, color);
				x1 += 1;
				e += 2 * dy;

			}
		}
		else if (p1.x > p2.x)
		{
			int x2 = p2.x;
			int y2 = p2.y;
			int yi = 1;
			int e;
			dx = p1.x - p2.x;
			dy = p1.y - p2.y;
			e = -dx;
			
			if (dy < 0)
			{
				yi = -1;
				dy = -dy;
			}
			while (x2 <= p1.x)
			{
				if (e > 0)
				{
					y2 += yi;
					e -= 2 * dx;
				}
				PutPixel(x2, y2, color);
				x2 += 1;
				e += 2 * dy;

			}

		}
	}
	else
	{
		if (p1.y < p2.y)
		{

			int x1 = p1.x;
			int y1 = p1.y;
			int xi = 1;
			int e;
			dx = p2.x - p1.x;
			dy = p2.y - p1.y;
			e = -dy;

			if (dx < 0)
			{
				xi = -1;
				dx = -dx;
			}
			while (y1 <= p2.y)
			{
				if (e > 0)
				{
					x1 += xi;
					e -= 2 * dy;
				}
				PutPixel(x1, y1, color);
				y1 += 1;
				e += 2 * dx;

			}
			

		}
		else if (p1.y > p2.y)
		{
			int x2 = p2.x;
			int y2 = p2.y;
			int xi = 1;
			int e;
			dx = p1.x - p2.x;
			dy = p1.y - p2.y;
			e = -dy;

			if (dx < 0)
			{
				xi = -1;
				dx = -dx;
			}
			while (y2 <= p1.y)
			{
				if (e > 0)
				{
					x2 += xi;
					e -= 2 * dy;
				}
				PutPixel(x2, y2, color);
				y2 += 1;
				e += 2 * dx;

			}




		}

	}
	
}

#
- Circle image 
![Circle Image](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/2020-11-17(1).png)
#
- drawing of my own choice
![FOX](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/2020-11-17%20(2)(1).png)
