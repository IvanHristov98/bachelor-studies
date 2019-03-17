// става и кост - конструктор
Bone = function(length,body)
{	
	if (typeof body==='undefined')
	{
		this.length = length[2];
		this.body = [custom(new Cuboid([0,0,0],length),{offset:[0,0,0.5]})];
	}
	else
	{
		this.length = length;
		this.body = body;
	}
	this.rot = [0,0,0,0];
};

// става и кост - метод за рисуване
Bone.prototype.draw = function()
{	
	if (this.rot)
	{
		if (this.rot[0]) zRotate(this.rot[0]);	// хоризонтален ъгъл
		if (this.rot[1]) yRotate(this.rot[1]);	// вертикален ъгъл
		if (this.rot[2]) xRotate(this.rot[2]);	// вертикален ъгъл
		if (this.rot[3]) zRotate(this.rot[3]);	// осев ъгъл
	}
	if (this.offset) translate(this.offset); 
	for (var i=0; i<this.body.length; i++)
		this.body[i].draw();
	translate([0,0,this.length]); // преместване в края на костта
};
