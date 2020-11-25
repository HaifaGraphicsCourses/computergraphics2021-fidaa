# Assignment #1
## part 2:
### 1:

* mesh vertices:	
![printvarts Image](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/printvarts.png)

* the vartices of each face		
![printfaces Image](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/printfaces.png)


### 2:
we found mix/min values of each coordinate x,y,z, over all of the vertices, and according to the max value between max X ,max Y and max Z we set the scale matrix to contain (350/MAx) in the diagonal line.( I chose 350 randomly, and i chose to divide by max to place the max vertix in 350, thats smaller than 1000 and that means that the max coordinate is fitted inside the window, so all the vertices fitted inside the window)	
after that we set the translate matrix to contain the absolute value of minX,minY,minZ, i mean we wanna translate the mesh model abs(minX) steps to the right, by that we change all the x coordinate to be bigger than 0 to fit between 0 and 1000(we added ans(min) to all the vertices,the smaller vertix minx now become 0).
the same for y and z.	
now we  translate ans scale the object by multiply each vertix : 
Scaling Matrix *Translate Matrix * the verix 	
now  all the coordinates of the vertices are well placed inside the screen space, they are not negative, smaller than 1,or larger than the screen.

### 3:
* the mesh image:	
![mesh2.3](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/mesh2.3.png)	


### 4: 
* GUI image:	
![GUI Image](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/GUI%20image.png)

we computed the transformation matrix according to all the transformation matrices:		
transformation matrix= (Scale in the world frame)matrix * (Rotate around z axis in the world frame)matrix *  (Rotate around y axis in the world frame)matrix * (Rotate around x axis in the world frame)matrix * (Translate in the world frame)matrix *(Scale in the model frame)matrix * (Rotate around z axis in the model frame)matrix *  (Rotate around y axis in the model frame)matrix * (Rotate around x axis in the model frame)matrix * (Translate in the model frame)matrix

		transformation mat= Scaleworls mat * Rotateworld mat * Translateworld mat * Scalemodel mat * Rotatemodel mat * Translatemodel mat
        Rotateworls mat= RotateZ mat* RotateY mat * Rotate X mat 
        same for Rotate model mat
   
   
   -all the matrices based on the input of the user in the GUI-	(we take the input and tern it to matrices with the help of glm and set it in the mesh class)
   
   now for every vertix:
   transformation matrix * (the base matrix that we ) * vertix vector	
   finally we draw with the help of Draw line that we did in part 1 	
   
### 5:
   * translating in the model frame and then rotating in the world frame:
   ![1img](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/1img.png)	
   
###### VS	
   * translating in the world frame and then rotating in the local frame:
   ![2img](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/2img.png)
   
   
   -its the same input for the two images, but we can see that the output is not the same.