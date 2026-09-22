#include "VertexBufferLayout.h"
#include <GL/glew.h>
#include"Render.h"
unsigned int VertexBufferElement::getSize()const
{
	switch (type)
	{
	case(GL_FLOAT): return sizeof(float); 
	case(GL_UNSIGNED_INT): return sizeof(unsigned int);
	case (GL_UNSIGNED_BYTE): return sizeof(unsigned char); 
	default: 
		ASSERT(false);
		return 0;

	}
}
