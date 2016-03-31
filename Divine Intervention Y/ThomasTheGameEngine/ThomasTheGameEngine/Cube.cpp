//Author:	Mathieu Violette
//Date:		12/6/2014

//		4-------5
//	  / |	  / |
//	0-------1	|
//	|	7---|---6
//	| /		| /
//	3-------2

#include "Cube.h"

Cube::Cube(GLint _programID, float _length, Vec3 _position) : program(_programID), Length(_length), GameObject(_position)
{
	colour = 0;

	colourLocation = glGetUniformLocation(program, "fColor");
	//rotLocation = glGetUniformLocation(program, "RotationMatrix");
	transLocation = glGetUniformLocation(program, "Transform");

	{
		vertices[0][0] = -0.5f * Length; vertices[0][1] = 0.5f * Length; vertices[0][2] = 0.5f * Length;
		vertices[1][0] = 0.5f * Length; vertices[1][1] = 0.5f * Length; vertices[1][2] = 0.5f * Length;
		vertices[2][0] = 0.5f * Length; vertices[2][1] = -0.5f * Length; vertices[2][2] = 0.5f * Length;
		vertices[3][0] = -0.5f * Length; vertices[3][1] = -0.5f * Length; vertices[3][2] = 0.5f * Length;

		vertices[4][0] = -0.5f * Length; vertices[4][1] = 0.5f * Length; vertices[4][2] = -0.5f * Length;
		vertices[5][0] = 0.5f * Length; vertices[5][1] = 0.5f * Length; vertices[5][2] = -0.5f * Length;
		vertices[6][0] = 0.5f * Length; vertices[7][1] = -0.5f * Length; vertices[7][2] = -0.5f * Length;
		vertices[7][0] = -0.5f * Length; vertices[6][1] = -0.5f * Length; vertices[6][2] = -0.5f * Length;
		
	}

	{
		//Face 1			//Face 2			//Face 3			//Face 4			//Face 5			//Face 6
		faces[0] = 0;		faces[4] = 4;		faces[8] = 0;		faces[12] = 1;		faces[16] = 0;		faces[20] = 3;
		faces[1] = 1;		faces[5] = 5;		faces[9] = 3;		faces[13] = 2;		faces[17] = 1;		faces[21] = 2;
		faces[2] = 2;		faces[6] = 6;		faces[10] = 7;		faces[14] = 6;		faces[18] = 5;		faces[22] = 6;
		faces[3] = 3;		faces[7] = 7;		faces[11] = 4;		faces[15] = 5;		faces[19] = 4;		faces[23] = 7;
	}


	
}

Cube::~Cube()
{
	
}

void Cube::Render()
{
	//glUniformMatrix4fv(rotLocation, 1, GL_FALSE, Matrix4::Rotate(transform->rotation).values);
	//glUniform4f(transLocation, transform->position.x, transform->position.y, transform->position.z, 0);

	glUniformMatrix4fv(transLocation, 1, GL_FALSE, transform->toMat4().values);

	DrawCubeSolid();
	DrawCubeOutline();
}

void Cube::Update(UINT32 _deltaTime)
{
	
}

void Cube::DrawCubeOutline()
{
	glUniform1f(colourLocation, -1);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, &faces[4 * 0]);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, &faces[4 * 1]);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, &faces[4 * 2]);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, &faces[4 * 3]);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, &faces[4 * 4]);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, &faces[4 * 5]);
}

void Cube::DrawCubeSolid()
{
	glUniform1f(colourLocation, colour);

	for (int i = 0; i < 6; i++)
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, &faces[4 * i]);

}